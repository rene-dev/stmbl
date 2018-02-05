#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"

#define NUM_OF_SAMPLES_Y 800

HAL_COMP(yaskawa);

HAL_PIN(pos);
HAL_PIN(error);
HAL_PIN(dump);
HAL_PIN(len);
HAL_PIN(off);

//TODO: use context
volatile uint32_t rxbuf[NUM_OF_SAMPLES_Y + 1];
volatile uint32_t txbuf[128];
int pos;
char data[150];
uint16_t tim_data[120];
DMA_InitTypeDef DMA_InitStructurerx;

static void hw_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct yaskawa_ctx_t *ctx = (struct yaskawa_ctx_t *)ctx_ptr;
  struct yaskawa_pin_ctx_t *pins = (struct yaskawa_pin_ctx_t *)pin_ptr;

  PIN(len) = 16;
  PIN(off) = 60;

  GPIO_InitTypeDef GPIO_InitStruct;

  //TX enable
  GPIO_InitStruct.GPIO_Pin   = FB0_Z_TXEN_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(FB0_Z_TXEN_PORT, &GPIO_InitStruct);
  GPIO_ResetBits(FB0_Z_TXEN_PORT, FB0_Z_TXEN_PIN);

  //TX
  GPIO_InitStruct.GPIO_Pin   = FB0_Z_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(FB0_Z_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(FB0_Z_PORT, FB0_Z_PIN_SOURCE, FB0_ENC_TIM_AF);

  RCC_APB1PeriphClockCmd(FB0_ENC_TIM_RCC, ENABLE);
  FB0_ENC_TIM->CCMR2 |= TIM_CCMR2_CC3S_0;
  FB0_ENC_TIM->CCER |= TIM_CCER_CC3E | TIM_CCER_CC3P | TIM_CCER_CC3NP;
  FB0_ENC_TIM->ARR = 65535;
  FB0_ENC_TIM->DIER |= TIM_DIER_CC3DE;


  //TIM8 triggers DMA for request and reply
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period            = 20;  // 168 / (20 + 1) = 8MHz
  TIM_TimeBaseStructure.TIM_Prescaler         = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM8, ENABLE);
  TIM_DMACmd(TIM8, TIM_DMA_Update, ENABLE);
  TIM_Cmd(TIM8, ENABLE);

  DMA_InitStructurerx.DMA_Channel            = DMA_Channel_7;  //TIM8 up
  DMA_InitStructurerx.DMA_PeripheralBaseAddr = (uint32_t)&FB0_Z_PORT->IDR;
  DMA_InitStructurerx.DMA_Memory0BaseAddr    = (uint32_t)&rxbuf;
  DMA_InitStructurerx.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructurerx.DMA_BufferSize         = NUM_OF_SAMPLES_Y;
  DMA_InitStructurerx.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructurerx.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructurerx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructurerx.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Word;
  DMA_InitStructurerx.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructurerx.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructurerx.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructurerx.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructurerx.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructurerx.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;

  //manchaster
  //0 -> 01
  //1 -> 10
  //SYNC                framing                         framing
  //0101010101010101010 01111110 111110 111110 111110 1 01111110;
  //010101... are to sync receiver
  //01111110 is hdlc start and end flag for framing
  //0 is appended after 5 ones => transmitted data is 0xffff
  //reply has the same format
  const char request[] = "0101010101010101010 01111110 111110 111110 111110 1 01111110";
  char request_m[sizeof(request) * 2];
  uint32_t tim_a = FB0_Z_PIN << 16;
  uint32_t tim_b = FB0_Z_PIN;

  //encode in manchaster
  int j = 0;
  for(int i = 0; request[i]; i++) {
    if(request[i] == '0') {
      request_m[j++] = '0';
      request_m[j++] = '1';
    } else if(request[i] == '1') {
      request_m[j++] = '1';
      request_m[j++] = '0';
    }
  }
  request_m[j] = '\0';

  //build txbuf for dma
  for(int i = 0; request_m[i]; i++) {
    if(request_m[i] == '0') {
      txbuf[pos++] = tim_a;
    } else if(request_m[i] == '1') {
      txbuf[pos++] = tim_b;
    }
  }
  txbuf[pos++] = tim_a;
  txbuf[pos++] = tim_a;
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct yaskawa_ctx_t *ctx      = (struct yaskawa_ctx_t *)ctx_ptr;
  struct yaskawa_pin_ctx_t *pins = (struct yaskawa_pin_ctx_t *)pin_ptr;
  // DMA2-Config
  DMA_Cmd(DMA2_Stream1, DISABLE);
  DMA_DeInit(DMA2_Stream1);
  DMA_InitTypeDef DMA_InitStructure;
  DMA_InitStructure.DMA_Channel            = DMA_Channel_7;  //TIM8 ch1
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&FB0_Z_PORT->BSRRL;  //TODO: change
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&txbuf;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize         = pos;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);

  DMA_Cmd(DMA1_Stream7, DISABLE);
  DMA_DeInit(DMA1_Stream7);
  DMA_InitStructure.DMA_Channel            = DMA_Channel_2;  //TIM8 ch1
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&FB0_ENC_TIM->CCR3;  //TODO: change
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&tim_data;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize         = ARRAY_SIZE(tim_data);
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream7, &DMA_InitStructure);

  TIM8->ARR              = 20;  // 168 / (20 + 1) = 8MHz
  FB0_Z_TXEN_PORT->BSRRL = FB0_Z_TXEN_PIN;  //TX enable
  FB0_Z_PORT->MODER &= ~GPIO_MODER_MODER14_1;
  FB0_Z_PORT->MODER |= GPIO_MODER_MODER14_0;  //set tx pin to output
  DMA_Cmd(DMA2_Stream1, DISABLE);
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  DMA_Cmd(DMA2_Stream1, ENABLE);  //transmit request

  while(!(DMA2->LISR & DMA_FLAG_TCIF1))
    ;  //wait for request

  
  

  
  
  FB0_Z_TXEN_PORT->BSRRH = FB0_Z_TXEN_PIN;  //TX disable
  FB0_Z_PORT->MODER &= ~GPIO_MODER_MODER14_0;  //set tx pin to af
  FB0_Z_PORT->MODER |= GPIO_MODER_MODER14_1;
  FB0_ENC_TIM->CR1 &= ~TIM_CR1_CEN; 
  FB0_ENC_TIM->CNT = 0;
  FB0_ENC_TIM->CCR3 = 0;
  FB0_ENC_TIM->CR1 |= TIM_CR1_CEN; 
  
  DMA_Cmd(DMA1_Stream7, DISABLE);
  DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);
  DMA_Cmd(DMA1_Stream7, ENABLE);

  // pos = 0;
  
  // DMA_Cmd(DMA2_Stream1, DISABLE);
  // DMA_DeInit(DMA2_Stream1);
  // TIM8->ARR = 6;  // 168 / (6 + 1) = 24MHz
  // DMA_Init(DMA2_Stream1, &DMA_InitStructurerx);

  // uint32_t error = 0;

  //GPIOB->BSRRH = GPIO_Pin_3;  //messpin
  //start DMA
  // DMA_Cmd(DMA2_Stream1, DISABLE);
  // DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  // DMA_Cmd(DMA2_Stream1, ENABLE);

  // while(DMA_GetFlagStatus(DMA2_Stream1, DMA_FLAG_TCIF1) == RESET);
  // DMA_Cmd(DMA2_Stream1, DISABLE);
  // DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);
  // DMA_Cmd(DMA2_Stream1, ENABLE);

// while(DMA_GetFlagStatus(DMA2_Stream1, DMA_FLAG_TCIF1) == RESET){
//   if(FB0_ENC_TIM->SR & TIM_SR_CC3IF){
//     tim_data[pos++] = FB0_ENC_TIM->CCR3;
//   }
// }
// int df = 0;
// for(int i = 0; i < PIN(off); i++){
//   if(FB0_ENC_TIM->SR & TIM_SR_CC3IF){
//     tim_data[df++] = FB0_ENC_TIM->CCR3;
//   }
// }
//     PIN(pos) = df;

while(FB0_ENC_TIM->CNT < 1600){

}
  DMA_Cmd(DMA1_Stream7, DISABLE);

PIN(pos) = DMA1_Stream7->NDTR;



  // //DMA2-Config
  

  // uint32_t error = 0;

  // //GPIOB->BSRRH = GPIO_Pin_3;  //messpin
  // //start DMA
  
  // //wait for DMA transfer complete
  
  // //GPIOB->BSRRL = GPIO_Pin_3;  //messpin

  // uint32_t current;
  // uint32_t last       = (rxbuf[0] & FB0_Z_PIN);
  // uint32_t first_edge = 0;
  // //find any edge
  // for(int j = 1; j < 20; j++) {
  //   current = (rxbuf[j] & FB0_Z_PIN);
  //   if(current != last) {
  //     first_edge = j;
  //     break;
  //   }
  //   last = current;
  // }

  // last = (rxbuf[first_edge] & FB0_Z_PIN);
  // //align edge
  // for(int j = first_edge + 3; j < 20; j += 3) {
  //   current = (rxbuf[j] & FB0_Z_PIN);
  //   if(current == last && last != 0) {
  //     first_edge = j;
  //     break;
  //   }
  //   last = current;
  // }

  // if(first_edge == 0 || first_edge == 19) {
  //   error++;
  // }

  // first_edge %= 6;

  // uint32_t bit      = 0;  // 2 manchaster bits = 1 bit
  // uint32_t ones     = 0;  // received ones, needed for framing and unstuffing
  // uint32_t j        = 0;  // unstuffing and start offset
  // int pos_length    = PIN(len);  // position length
  // int pos_offset    = PIN(off);  // position offset
  // int decode_length = pos_length + pos_offset + 1;
  // //decode manchaster and unstuff
  // for(int i = j; i < (decode_length + j); i++) {
  //   bit = 0;
  //   if(rxbuf[i * 6 + first_edge + 1] & FB0_Z_PIN) {
  //     bit += 2;
  //   }
  //   if(rxbuf[i * 6 + first_edge + 4] & FB0_Z_PIN) {
  //     bit += 1;
  //   }
  //   switch(bit) {
  //     case 1:  //01 -> manchaster encoded 0
  //       if(ones == 5) {  // 0 after 5 ones => stuffed data
  //         j++;
  //       } else {
  //         data[i - j] = '0';
  //       }
  //       ones = 0;
  //       break;
  //     case 2:  //10 -> manchaster encoded 1
  //       if(ones == 5) {  // 1 after 5 ones => start of packet
  //         j    = ++i;
  //         ones = 0;
  //       } else {
  //         ones++;
  //         data[i - j] = '1';
  //       }
  //       break;
  //     case 0:  //00 -> manchaster alignment error
  //     case 3:  //11 -> manchaster alignment error
  //     default:  //obsolete
  //       data[i - j] = 'X';
  //       error++;
  //       break;
  //   }
  // }
  // data[100] = 0;  // terminate string

  // uint32_t pos = 0;
  // //extract position data
  // for(int i = 0; i < pos_length; i++) {
  //   pos += (data[i + pos_offset] == '1') << i;
  // }

  // if(error) {
  //   PIN(error) = 1.0;
  // } else {
  //   PIN(error) = 0.0;
  //   //PIN(pos)   = (float)pos / (float)(1 << pos_length) * M_PI * 2.0 - M_PI;
  // }
}

static void nrt_func(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct yaskawa_ctx_t *ctx      = (struct yaskawa_ctx_t *)ctx_ptr;
  struct yaskawa_pin_ctx_t *pins = (struct yaskawa_pin_ctx_t *)pin_ptr;
  if(RISING_EDGE(PIN(dump))) {
    //printf("%s\n", data);
    for(int i = 0; i < ARRAY_SIZE(tim_data) - 1; i++){
      printf("%i, ", tim_data[i + 1] - tim_data[i]);
    }
    printf(";\n");
  }
}

hal_comp_t yaskawa_comp_struct = {
    .name      = "yaskawa",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = 0,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct yaskawa_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
