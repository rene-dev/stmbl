#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"

HAL_COMP(encf);

HAL_PIN(error);
HAL_PIN(dma);  //dma transfers
HAL_PIN(dump);

HAL_PIN(pos);
HAL_PIN(turns);
HAL_PIN(com_pos);
HAL_PIN(index);

volatile uint16_t tim_data[160];

#pragma pack(1)
typedef struct{
  uint32_t flag0 : 4; // 0101
  uint32_t bat : 2;
  uint32_t flag1 : 3; // 101
  uint32_t no_index : 1; 
  uint32_t flag2 : 1; // 0
  uint32_t pos_lo : 6;
  uint32_t flag3 : 2; // 10
  uint32_t pos_hi : 16;
  uint32_t flag4 : 2; // 10
  uint32_t turns : 16;
  uint32_t flag5 : 2; // 10
  uint32_t com_pos : 10;
  uint32_t flag6 : 7; // 0000011
  uint32_t crc : 5;
  uint32_t flag7 : 3; // 000
} fanuc_t;

union {
  uint8_t enc_data[10];
  fanuc_t fanuc;
} data;

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct encf_ctx_t *ctx = (struct encf_ctx_t *)ctx_ptr;
  // struct encf_pin_ctx_t * pins = (struct encf_pin_ctx_t *)pin_ptr;
  GPIO_InitTypeDef GPIO_InitStruct;

  //TX enable
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  //TIM CH1 A
  GPIO_InitStruct.GPIO_Pin   = FB0_A_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(FB0_A_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(FB0_A_PORT, FB0_A_PIN_SOURCE, FB0_ENC_TIM_AF);

  //TIM rx dma
  //fb0 rx auf 12: tim4_ch1 dma1_0_2
  //fb1 rx auf 12: tim1_ch1 dma2_3_6
  DMA_InitTypeDef DMA_InitStruct;
  DMA_InitStruct.DMA_Channel            = DMA_Channel_2;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&FB0_ENC_TIM->CCR1;
  DMA_InitStruct.DMA_Memory0BaseAddr    = (uint32_t)&tim_data;
  DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize         = ARRAY_SIZE(tim_data);
  DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_MemoryDataSize     = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority           = DMA_Priority_VeryHigh;
  DMA_InitStruct.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStruct.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStruct.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Cmd(DMA1_Stream0, DISABLE);
  DMA_DeInit(DMA1_Stream0);
  DMA_Init(DMA1_Stream0, &DMA_InitStruct);

  //timer setup
  RCC_APB1PeriphClockCmd(FB0_ENC_TIM_RCC, ENABLE);
  FB0_ENC_TIM->CR1 &= ~TIM_CR1_CEN; 
  FB0_ENC_TIM->CCMR1 = TIM_CCMR1_CC1S_0; // cc1 input ti1
  FB0_ENC_TIM->CCER = TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC1NP; // cc1 en, rising edge, falling edge
  FB0_ENC_TIM->ARR = 65535;
  FB0_ENC_TIM->DIER = TIM_DIER_CC1DE; // enable cc1 dma reeuest

  //SPI is used to generate request
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  SPI_InitTypeDef SPI_InitTypeDefStruct;
  SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_High;
  SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_Init(SPI3, &SPI_InitTypeDefStruct);
  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  SPI_Cmd(SPI3, ENABLE);

  GPIO_SetBits(GPIOD, GPIO_Pin_15);  //tx enable
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct encf_ctx_t *ctx      = (struct encf_ctx_t *)ctx_ptr;
  struct encf_pin_ctx_t *pins = (struct encf_pin_ctx_t *)pin_ptr;

  uint32_t count = ARRAY_SIZE(tim_data) - DMA1_Stream0->NDTR;
  PIN(dma) = count;

  for(int i = 0; i < 10; i++){
    data.enc_data[i] = 0;
  }
  
  uint8_t bits_sum = 0;
  for(int i = 1; i < count; i++){//each capture form dma
    //1 bit = 82 ticks (1.024e-6)/(1/82e6)
    //calculate time between edges
    uint16_t diff = tim_data[i] - tim_data[i-1];
    //number of bits to set
    int bits = (float)diff/(float)81;
    if(i%2 == 0){//line starts high, set every even numbered captures to 1
      for(int j = bits_sum; j < bits + bits_sum; j++){
        data.enc_data[j/8] |= (1 << j%8);
      }
    }
    bits_sum += bits;
  }
  if(bits_sum > 50){
    uint32_t pos;
    pos = data.fanuc.pos_lo + (data.fanuc.pos_hi << 6);
    PIN(pos) = ((float)pos * 2.0 * M_PI / (1<<22)) - M_PI;
    PIN(turns) = data.fanuc.turns;
    pos = data.fanuc.com_pos;
    PIN(com_pos) = (pos * 2.0 * M_PI / 1024) - M_PI;
    PIN(index) = data.fanuc.no_index;
    PIN(error) = 0;
  }else{
    PIN(error) = 1;
  }
  //reset timer
  FB0_ENC_TIM->CNT = 0;
  FB0_ENC_TIM->CCR1 = 0;
  FB0_ENC_TIM->CR1 |= TIM_CR1_CEN; // enable tim

  //send request, 1/(42e6/32)*11 = 8.4uS
  SPI3->DR = 0xffe;
  //start DMA
  DMA_Cmd(DMA1_Stream0, DISABLE);
  DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
  DMA_Cmd(DMA1_Stream0, ENABLE);
}

static void nrt_func(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct encf_ctx_t *ctx = (struct encf_ctx_t *)ctx_ptr;
  struct encf_pin_ctx_t * pins = (struct encf_pin_ctx_t *)pin_ptr;
  if(RISING_EDGE(PIN(dump))) {
    for(int i = 0; i < 80; i++){
      if(data.enc_data[i/8] & (1 << i%8)){
        printf("1");
      }else{
        printf("0");
      }
    }
    printf("\n");
  }
}

hal_comp_t encf_comp_struct = {
    .name      = "encf",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct encf_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
