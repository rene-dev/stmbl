#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"
#include <string.h>
#include <endat.h>

HAL_COMP(endat);



HAL_PIN(pos);
HAL_PIN(mpos);

HAL_PIN(f1);
HAL_PIN(crc);
HAL_PIN(shift);
HAL_PIN(timer);
HAL_PIN(swap);
HAL_PIN(skip);
HAL_PIN(bytes);

HAL_PIN(error);
HAL_PIN(state);

HAL_PIN(endat_error);
HAL_PIN(endat_warning);
HAL_PIN(endat_state);
HAL_PIN(pos_len);
HAL_PIN(mpos_len);
HAL_PIN(pos_res);
HAL_PIN(type);
HAL_PIN(max_vel);

HAL_PIN(req);

HAL_PIN(print_time);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct endat_ctx_t *ctx = (struct endat_ctx_t *)ctx_ptr;
  struct endat_pin_ctx_t *pins = (struct endat_pin_ctx_t *)pin_ptr;
  GPIO_InitTypeDef GPIO_InitStruct;

  //TX enable for mosi
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  //tx enable for clock
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  //SPI3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  
  SPI3->CR1 |= SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_BIDIMODE | SPI_BaudRatePrescaler_32;

  //PC12 spi3 mosi
  //PC10 spi3 clock
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_SetBits(GPIOD, GPIO_Pin_10);  //clock tx enable

  // DMA_InitTypeDef dma_rx_config;
  // dma_rx_config.DMA_Channel            = DMA_Channel_0;
  // dma_rx_config.DMA_PeripheralBaseAddr = (uint32_t)&FB0_SPI->DR;
  // dma_rx_config.DMA_Memory0BaseAddr    = (uint32_t)&tim_data;
  // dma_rx_config.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  // dma_rx_config.DMA_BufferSize         = ARRAY_SIZE(tim_data);
  // dma_rx_config.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  // dma_rx_config.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  // dma_rx_config.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  // dma_rx_config.DMA_MemoryDataSize     = DMA_PeripheralDataSize_HalfWord;
  // dma_rx_config.DMA_Mode               = DMA_Mode_Normal;
  // dma_rx_config.DMA_Priority           = DMA_Priority_VeryHigh;
  // dma_rx_config.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  // dma_rx_config.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  // dma_rx_config.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  // dma_rx_config.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;

  // DMA_Cmd(DMA1_Stream2, DISABLE);
  // DMA_DeInit(DMA1_Stream2);
  // DMA_Init(DMA1_Stream2, &dma_rx_config);

  PIN(pos_len) = 18; // 17
  PIN(mpos_len) = 12; // 15
  PIN(endat_state) = 13;
  PIN(swap) = 1;
  PIN(skip) = 10;
  PIN(bytes) = 7;
}

union{
  uint64_t data;
  uint8_t dataa[8];
} df, df1, df2;

endat_cmd_t req;

struct endat_ctx_t {
  endat_data_t data;
};

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct endat_ctx_t *ctx      = (struct endat_ctx_t *)ctx_ptr;
  struct endat_pin_ctx_t *pins = (struct endat_pin_ctx_t *)pin_ptr;

  req = 0;
  uint8_t addr = 0; 

  ctx->data.pos_bits = PIN(pos_len);
  ctx->data.mpos_bits = PIN(mpos_len);

  switch((int)PIN(endat_state)){
    case 0: // reset error
      req = ENDAT_RESET;
    break;

    case 1: // select mem state
      req = ENDAT_SELECT_MEM;
      addr = ENDAT_MEM_STATE;
    break;

    case 2: // read error
      req = ENDAT_READ_ADDR;
      addr = ENDAT_ADDR_ERROR;
    break;

    case 3: // read warning
      req = ENDAT_READ_ADDR;
      addr = ENDAT_ADDR_WARNING;
    break;

    case 4: // select mem 0
      req = ENDAT_SELECT_MEM;
      addr = ENDAT_MEM_PARAM0;
    break;

    case 5: // read len
      req = ENDAT_READ_ADDR;
      addr = ENDAT_ADDR_POS_LEN;
    break;

    case 6: // read type
      req = ENDAT_READ_ADDR;
      addr = ENDAT_ADDR_TYPE;
    break;

    case 7: // select mem 1
      req = ENDAT_SELECT_MEM;
      addr = ENDAT_MEM_PARAM1;
    break;

    case 8: // read multi
      req = ENDAT_READ_ADDR;
      addr = ENDAT_ADDR_MULTITURN;
    break;

    case 9: // read res low
      req = ENDAT_READ_ADDR;
      addr = ENDAT_ADDR_RES_LOW;
    break;

    case 10: // read res high
      req = ENDAT_READ_ADDR;
      addr = ENDAT_ADDR_RES_HIGH;
    break;

    case 11: // select mem 2
      req = ENDAT_SELECT_MEM;
      addr = ENDAT_MEM_PARAM2;
    break;

    case 12: // read max vel
      req = ENDAT_READ_ADDR;
      addr = ENDAT_ADDR_MAX_VEL;
    break;

    case 13: // read pos
      req = ENDAT_READ_POS;
    break;
  }

  if(PIN(req) > 0){
    req = PIN(req);
  }

  SPI3->CR1 &= ~SPI_CR1_SPE;//disable spi
  SPI3->CR1 = SPI_CR1_LSBFIRST | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_BIDIMODE | SPI_BaudRatePrescaler_32;

  uint32_t bits = endat_tx(req, addr, 0, df.dataa, &(ctx->data));
  df2.data = df.data;

  GPIO_SetBits(GPIOD, GPIO_Pin_15);//tx enable
  SPI3->CR1 |= SPI_CR1_BIDIOE;//enable output
  SPI3->CR1 |= SPI_CR1_SPE;//enable spi

  for(int i = 0; i < (bits + 7) / 8; i++){
    SPI3->DR = df.dataa[i];
    while(!(SPI3->SR & SPI_SR_TXE));
    while(SPI3->SR & SPI_SR_BSY);
  }
  
  SPI3->CR1 &= ~SPI_CR1_BIDIOE;//disable output, this activates the clock
  GPIO_ResetBits(GPIOD, GPIO_Pin_15);//tx disable

  SPI3->CR1 &= ~SPI_CR1_SPE;//disable spi
  if(PIN(swap) > 0.0){ 
    SPI3->CR1 = SPI_CR1_LSBFIRST | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_BIDIMODE | SPI_BaudRatePrescaler_32;
  }
  else{
    SPI3->CR1 = SPI_CR1_LSBFIRST | SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_BIDIMODE | SPI_BaudRatePrescaler_32;
  }
  SPI3->CR1 |= SPI_CR1_SPE;//enable spi

  df.data = 0;
  for(int i = 0; i < MIN(sizeof(df.data), PIN(bytes)); i++){
    while(!(SPI3->SR & SPI_SR_RXNE));
    df.dataa[i] = SPI3->DR;
  }

  df1.data = df.data >> (int)PIN(skip);
  PIN(shift) = PIN(skip);
  while((df1.data & 1) == 0 && PIN(shift) < 32.0){
    df1.data = df1.data >> 1;
    PIN(shift)++;
  }

  uint32_t ret = endat_rx(df1.dataa, MIN(sizeof(df1.data), PIN(bytes)) * 8, &(ctx->data));

  switch((int) PIN(endat_state)){
    case 12:
      if(ctx->data.error_bit){
        PIN(endat_state) = 0;
        PIN(error) = 1;
        PIN(state) = 0;
      }
      else{
        PIN(state) = 1;
        PIN(error) = 0;
      }

      if(ret == 0){
        PIN(endat_state) = 0;
        PIN(error) = 1;
        PIN(state) = 0;
      }
    break;

    default:
      PIN(state) = 0;
      if(ret == 0){
        PIN(endat_state) = 0;
      }
      else{
        PIN(endat_state)++;
      }
  }

  PIN(mpos) = ctx->data.mpos;
  PIN(f1) = ctx->data.error_bit;
  PIN(crc) = ctx->data.crc;
  PIN(pos_len) = ctx->data.pos_bits;
  PIN(mpos_len) = ctx->data.mpos_bits;
  PIN(pos_res) = ctx->data.pos_res;
  PIN(type) = ctx->data.fb_type;
  PIN(endat_error) = ctx->data.error.reg;
  PIN(endat_warning) = ctx->data.warning.reg;
  PIN(max_vel) = ctx->data.max_vel;

  if(ctx->data.pos_bits){
    PIN(pos) = mod(ctx->data.pos * 2.0 * M_PI / (1 << ctx->data.pos_bits));
  }

  //TODO: wait for busy flag?
  SPI3->CR1 &= ~SPI_CR1_SPE;//disable spi

  if(PIN(print_time) > 0.0){
    PIN(timer) += period;
  }
}

static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct endat_ctx_t *ctx      = (struct endat_ctx_t *)ctx_ptr;
  struct endat_pin_ctx_t *pins = (struct endat_pin_ctx_t *)pin_ptr;

  if(PIN(timer) > PIN(print_time)) {
    PIN(timer) = 0.0;

    uint64_t data = df.data;
    uint64_t reqdata = df2.data;
    uint64_t pos1 = ctx->data.pos;
    uint64_t pos2 = ctx->data.mpos;
    uint64_t crc = ctx->data.crc;
    uint32_t shift = PIN(shift);

    printf("req: ");
    switch(req){
      case ENDAT_SELECT_MEM:
        printf("ENDAT_SELECT_MEM\n");
      break;

      case ENDAT_READ_ADDR:
        printf("ENDAT_READ_ADDR\n");
      break;

      case ENDAT_WRITE_ADDR:
        printf("ENDAT_WRITE_ADDR\n");
      break;

      case ENDAT_READ_POS:
        printf("ENDAT_READ_POS\n");
      break;

      default:
        printf("unkonwn req\n");
    }
    
    printf("req data: ");
    for(int i = 0; i < 64; i++){
      if(reqdata & 1){
        printf("1");
      }
      else{
        printf("0");
      }
      reqdata >>= 1;
      if(i == 8 - 1){
        printf("-");
      }
      if(i == 8 + 8 - 1){
        printf("-");
      }
      if(i == 8 + 8 + 16 - 1){
        printf("-");
      }
      if(i == 8 + 8 + 16 + 5 - 1){
        printf("-");
      }
    }
    printf("\n");

    printf("data: ");
    for(int i = 0; i < 32; i++){
      if(data & 1){
        printf("1");
      }
      else{
        printf("0");
      }
      data >>= 1;
      if(i == shift - 1){
        printf("-");
      }
      if(i == shift + 2 - 1){
        printf("-");
      }
      if(i == shift + 2 + ctx->data.pos_bits - 1){
        printf("-");
      }
      if(i == shift + 2 + ctx->data.pos_bits + ctx->data.mpos_bits - 1){
        printf("-");
      }
    }
    printf("\n");
    printf("pos1: ");
    for(int i = 0; i < 64; i++){
      if(pos1 & 1){
        printf("1");
      }
      else{
        printf("0");
      }
      pos1 >>= 1;
      if(i == ctx->data.pos_bits - 1){
        printf("-");
      }
    }
    printf("\n");
    printf("pos2: ");
    for(int i = 0; i < 64; i++){
      if(pos2 & 1){
        printf("1");
      }
      else{
        printf("0");
      }
      pos2 >>= 1;
      if(i == ctx->data.mpos_bits - 1){
        printf("-");
      }
    }
    printf("\n");

    printf("crc:  ");
    for(int i = 0; i < 64; i++){
      if(crc & 1){
        printf("1");
      }
      else{
        printf("0");
      }
      crc >>= 1;
      if(i == 5 - 1){
        printf("-");
      }
    }
    printf("\n\n");
  }
}

hal_comp_t endat_comp_struct = {
    .name      = "endat",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct endat_ctx_t),
    .pin_count = sizeof(struct endat_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
