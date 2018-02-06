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
volatile char data[150];
volatile char m_data[150];
volatile uint16_t tim_data[300];
DMA_InitTypeDef DMA_InitStructurerx;

static void hw_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct yaskawa_ctx_t *ctx = (struct yaskawa_ctx_t *)ctx_ptr;
  struct yaskawa_pin_ctx_t *pins = (struct yaskawa_pin_ctx_t *)pin_ptr;

  PIN(len) = 13;
  PIN(off) = 66;
  data[0] = 'B';
  data[1] = 0;

  GPIO_InitTypeDef GPIO_InitStruct;

  //TX enable
  GPIO_InitStruct.GPIO_Pin   = FB0_Z_TXEN_PIN;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
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



  while(FB0_ENC_TIM->CNT < 3300){

  }

  FB0_Z_TXEN_PORT->BSRRL = FB0_Z_TXEN_PIN;
  
  int count = ARRAY_SIZE(tim_data) - DMA1_Stream7->NDTR;
  DMA_Cmd(DMA1_Stream7, DISABLE);


  uint16_t bit_time = 15;
  FB0_Z_TXEN_PORT->BSRRH = FB0_Z_TXEN_PIN;

  if(count > 80){
    int pol = 0;
    int read_counter = 0;
    int write_counter = 0;
    int counter = 0;

    for(int i = 1; i < count; i++){
      if(tim_data[i + 1] - tim_data[i] < bit_time){
        counter++;
      }
      else if(counter == 10){
        read_counter = i + 1;
        pol = 0;
        break;
      }
      else{
        counter = 0;
      }
    }

    for(int i = read_counter; i < count; i++){
      if(tim_data[i + 1] - tim_data[i] < bit_time){
        i++;
        if(tim_data[i + 1] - tim_data[i] < bit_time){
          data[write_counter++] = '0' + pol;
        }
        else{
          //error
          PIN(error) = 1.0;
        }

      }
      else{
        pol = 1 - pol;
        data[write_counter++] = '0' + pol;
      }
    }
    data[write_counter] = 0;

    int write_counter2 = 0;
    counter = 0;
    for(int i = 0; i < write_counter; i++){
      if(data[i] == '1'){
        counter++;
        m_data[write_counter2++] = '1';
      }
      else if(counter == 5){
        counter = 0;
        // unstuff
      }
      else if(counter == 6){
        // hldc
        m_data[write_counter2++] = 'H';
        break;
      }
      else{
        counter = 0;
        m_data[write_counter2++] = '0';
      }
    }
    m_data[write_counter2] = 0;


    uint32_t pos = 0;
    //extract position data
    for(int i = 0; i < PIN(len); i++) {
      pos += (m_data[i + (int)PIN(off)] == '1') << i;
    }
    
    PIN(pos)   = (float)pos / (float)(1 << (int)PIN(len)) * M_PI * 2.0 - M_PI;

  }
  else{
    PIN(error) = 1.0;
    // error
  }


  
}

static void nrt_func(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  // struct yaskawa_ctx_t *ctx      = (struct yaskawa_ctx_t *)ctx_ptr;
  struct yaskawa_pin_ctx_t *pins = (struct yaskawa_pin_ctx_t *)pin_ptr;
  if(RISING_EDGE(PIN(dump))) {
    for(int i = 0; i < 14; i++){
      for(int j = 0; j < 8; j++){
        printf("%c", m_data[i * 8 + j]);
      }
      printf("|");
    }
    printf("\n");
    // for(int i = 0; i < PIN(pos); i++){
    //   printf("%i, ", tim_data[i + 1] - tim_data[i]);
    // }
    // printf(";\n");
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
