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

HAL_PIN(req);
HAL_PIN(pos_len);
HAL_PIN(mpos_len);
HAL_PIN(bytes);
HAL_PIN(tx_bits);
HAL_PIN(tx_bytes);
HAL_PIN(p1);
HAL_PIN(p2);

HAL_PIN(pos);
HAL_PIN(mpos);

HAL_PIN(f1);
HAL_PIN(crc);
HAL_PIN(shift);
HAL_PIN(timer);
HAL_PIN(swap);
HAL_PIN(scip);


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
  PIN(req) = ENDAT_READ_POS;
  PIN(pos_len) = 18; // 17
  PIN(mpos_len) = 12; // 15
  PIN(swap) = 1;
  PIN(scip) = 10;
  PIN(bytes) = 7;
}

union{
    uint64_t data;
    uint8_t dataa[8];
  } df, df1;


struct endat_ctx_t {
  endat_data_t data;
};

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct endat_ctx_t *ctx      = (struct endat_ctx_t *)ctx_ptr;
  struct endat_pin_ctx_t *pins = (struct endat_pin_ctx_t *)pin_ptr;

  SPI3->CR1 &= ~SPI_CR1_SPE;//disable spi

  SPI3->CR1 = SPI_CR1_LSBFIRST | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_BIDIMODE | SPI_BaudRatePrescaler_32;

  uint32_t bits = endat_tx((uint8_t) PIN(req), (uint8_t) PIN(p1), (uint16_t) PIN(p2), df.dataa, &(ctx->data));
  PIN(tx_bits) = bits;
  PIN(tx_bytes) = (bits + 7) / 8;

  GPIO_SetBits(GPIOD, GPIO_Pin_15);//tx enable
  SPI3->CR1 |= SPI_CR1_BIDIOE;//enable output
  SPI3->CR1 |= SPI_CR1_SPE;//enable spi

  for(int i = 0; i < (int)PIN(tx_bytes); i++){
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

  df1.data = df.data >> (int)PIN(scip);
  PIN(shift) = PIN(scip);
  while((df1.data & 1) == 0 && PIN(shift) < 32.0){
    df1.data = df1.data >> 1;
    PIN(shift)++;
  }

  ctx->data.pos_bits = PIN(pos_len); // todo
  ctx->data.mpos_bits = PIN(mpos_len);

  endat_rx(df1.dataa, MIN(sizeof(df1.data), PIN(bytes)) * 8, &(ctx->data));

  PIN(mpos) = ctx->data.mpos;
  PIN(f1) = ctx->data.error_bit;
  PIN(crc) = ctx->data.crc;

  if(ctx->data.pos_bits){
    PIN(pos) = mod(ctx->data.pos * 2.0 * M_PI / (1 << ctx->data.pos_bits));
  }

  //TODO: wait for busy flag?
  SPI3->CR1 &= ~SPI_CR1_SPE;//disable spi

  PIN(timer) += period;
}

static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct endat_ctx_t *ctx      = (struct endat_ctx_t *)ctx_ptr;
  struct endat_pin_ctx_t *pins = (struct endat_pin_ctx_t *)pin_ptr;

  if(PIN(timer) > 0.5) {
    PIN(timer) = 0.0;
    int i = 0;

    uint64_t data = df.data;
    uint64_t pos1 = ctx->data.pos;
    uint64_t pos2 = ctx->data.mpos;
    uint64_t crc = ctx->data.crc;
    uint32_t shift = PIN(shift);


    printf("data: ");
    for(int i = 0; i < 64; i++){
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
      if(i == shift + 2 + ctx->data.pos_bits + ctx->data.mpos_bits + 5 - 1){
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
