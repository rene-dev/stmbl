#include "yaskawa_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f4xx_conf.h"
#include "hw/hw.h"
#include "yaskawa_crc16.h"

HAL_COMP(yaskawa);

HAL_PIN(pos);
HAL_PIN(error);
HAL_PIN(error_sum);
HAL_PIN(dump);
HAL_PIN(len);
HAL_PIN(off);

HAL_PIN(len2);
HAL_PIN(off2);
HAL_PIN(probe2);

HAL_PIN(len3);
HAL_PIN(len4);
HAL_PIN(len5);

HAL_PIN(probe3);
HAL_PIN(probe4);
HAL_PIN(probe5);

HAL_PIN(send);
HAL_PIN(crc_ok);
HAL_PIN(crc_error);

//TODO: use context
static volatile uint32_t txbuf[128];
static int pos;
static int dfdf;
static volatile char m_data[150];
static volatile char m_data2[150];
static volatile uint16_t tim_data[300];
static DMA_InitTypeDef DMA_InitStructuretx;
static DMA_InitTypeDef DMA_InitStructurerx;
static uint8_t yaskawa_reply[14];
//uint8_t yaskara_reply_len;

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct yaskawa_ctx_t *ctx = (struct yaskawa_ctx_t *)ctx_ptr;
  struct yaskawa_pin_ctx_t *pins = (struct yaskawa_pin_ctx_t *)pin_ptr;

  PIN(len)  = 15;
  PIN(off)  = 64;
  PIN(len3) = 57;
  PIN(len4) = 58;
  PIN(len5) = 59;
}

static void hw_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct yaskawa_ctx_t *ctx = (struct yaskawa_ctx_t *)ctx_ptr;
  // struct yaskawa_pin_ctx_t *pins = (struct yaskawa_pin_ctx_t *)pin_ptr;

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
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//TODO: default to AF?
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(FB0_Z_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(FB0_Z_PORT, FB0_Z_PIN_SOURCE, FB0_ENC_TIM_AF);

  RCC_APB1PeriphClockCmd(FB0_ENC_TIM_RCC, ENABLE);


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

  DMA_InitStructuretx.DMA_Channel            = DMA_Channel_7;
  DMA_InitStructuretx.DMA_PeripheralBaseAddr = (uint32_t)&FB0_Z_PORT->BSRRL;  //TODO: change
  DMA_InitStructuretx.DMA_Memory0BaseAddr    = (uint32_t)&txbuf;
  DMA_InitStructuretx.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructuretx.DMA_BufferSize         = pos;
  DMA_InitStructuretx.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructuretx.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructuretx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructuretx.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Word;
  DMA_InitStructuretx.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructuretx.DMA_Priority           = DMA_Priority_VeryHigh;
  DMA_InitStructuretx.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructuretx.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructuretx.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructuretx.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;

  DMA_InitStructurerx.DMA_Channel            = DMA_Channel_2;
  DMA_InitStructurerx.DMA_PeripheralBaseAddr = (uint32_t)&FB0_ENC_TIM->CCR3;  //TODO: change
  DMA_InitStructurerx.DMA_Memory0BaseAddr    = (uint32_t)&tim_data;
  DMA_InitStructurerx.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructurerx.DMA_BufferSize         = ARRAY_SIZE(tim_data);
  DMA_InitStructurerx.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructurerx.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructurerx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructurerx.DMA_MemoryDataSize     = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructurerx.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructurerx.DMA_Priority           = DMA_Priority_VeryHigh;
  DMA_InitStructurerx.DMA_FIFOMode           = DMA_FIFOMode_Disable;
  DMA_InitStructurerx.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructurerx.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructurerx.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;

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

  FB0_ENC_TIM->CR1 &= ~TIM_CR1_CEN;
  FB0_ENC_TIM->ARR = 65535;
  FB0_ENC_TIM->CNT = 3300;
  FB0_ENC_TIM->CR1 |= TIM_CR1_CEN;  // enable tim

  DMA_Cmd(DMA1_Stream7, DISABLE);
  DMA_DeInit(DMA1_Stream7);
  DMA_Init(DMA1_Stream7, &DMA_InitStructurerx);

  DMA_Cmd(DMA2_Stream1, DISABLE);
  DMA_DeInit(DMA2_Stream1);
  DMA_Init(DMA2_Stream1, &DMA_InitStructuretx);
  dfdf = 0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct yaskawa_ctx_t *ctx      = (struct yaskawa_ctx_t *)ctx_ptr;
  struct yaskawa_pin_ctx_t *pins = (struct yaskawa_pin_ctx_t *)pin_ptr;

  while(FB0_ENC_TIM->CNT < 3300) {
  }

  int count = ARRAY_SIZE(tim_data) - DMA1_Stream7->NDTR;
  DMA_Cmd(DMA1_Stream7, DISABLE);

  uint16_t bit_time = 15;

  if(count > 80) {
    int pol          = 0;
    int read_counter = 0;
    int counter      = 0;

    for(int i = 0; i < ARRAY_SIZE(yaskawa_reply); i++) {
      yaskawa_reply[i] = 0;
    }

    for(int i = 1; i < count; i++) {
      if(tim_data[i + 1] - tim_data[i] < bit_time) {
        counter++;
      } else if(counter == 10) {
        read_counter = i + 1;
        pol          = 0;
        break;
      } else {
        counter = 0;
      }
    }

    int write_counter2 = 0;
    for(int i = read_counter; i < count; i++) {
      if(tim_data[i + 1] - tim_data[i] < bit_time) {
        i++;
        if(tim_data[i + 1] - tim_data[i] < bit_time) {
          //data[write_counter++] = '0' + pol;
        } else {
          //error
          PIN(error) = 1.0;
          break;
        }

      } else {
        pol = 1 - pol;
        //data[write_counter++] = '0' + pol;
      }

      if(pol == 1) {
        counter++;
        m_data[write_counter2] = '1';
        yaskawa_reply[write_counter2 / 8] |= 1 << (7 - (write_counter2 % 8));
        write_counter2++;
      } else if(counter == 5) {
        counter = 0;
        // unstuff
      } else if(counter == 6) {
        // hldc
        m_data[write_counter2++] = 'H';
        break;
      } else {
        counter                  = 0;
        m_data[write_counter2++] = '0';
      }
    }

    yaskawa_crc16_t crc;
    yaskawa_crc16_t data = (yaskawa_reply[13] & 0xff) | (yaskawa_reply[12] << 8);
    crc                  = yaskawa_crc16_init();
    crc                  = yaskawa_crc16_update(crc, yaskawa_reply, 12);
    crc                  = yaskawa_crc16_finalize(crc);
    if(data == crc) {
      PIN(crc_ok)
      ++;
    } else {
      PIN(crc_error)
      ++;
    }

    m_data[write_counter2] = 0;

    uint32_t pos = 0;
    //extract position data
    for(int i = 0; i < PIN(len); i++) {
      pos += (m_data[i + (int)PIN(off)] == '1') << i;
    }

    uint32_t probe = 0;
    for(int i = 0; i < PIN(len2); i++) {
      probe += (m_data[i + (int)PIN(off2)] == '1') << i;
    }

    PIN(pos)    = (float)pos / (float)(1 << (int)PIN(len)) * M_PI * 2.0 - M_PI;
    PIN(probe2) = probe;

    PIN(probe3) = m_data[(int)PIN(len3)] == '1';
    PIN(probe4) = m_data[(int)PIN(len4)] == '1';
    PIN(probe5) = m_data[(int)PIN(len5)] == '1';
    PIN(error)  = 0.0;

    if(dfdf < 1) {
      for(int i = 0; i < ARRAY_SIZE(m_data2); i++) {
        m_data2[i] = m_data[i];
      }
      dfdf = 1;
    }
  } else {
    PIN(error) = 1.0;
    // error
  }

  // PIN(send) = send;

  FB0_Z_TXEN_PORT->BSRRL = FB0_Z_TXEN_PIN;  //TX enable
  FB0_Z_PORT->MODER &= ~GPIO_MODER_MODER14_1;
  FB0_Z_PORT->MODER |= GPIO_MODER_MODER14_0;  //set tx pin to output

  DMA_Cmd(DMA2_Stream1, DISABLE);
  DMA_DeInit(DMA2_Stream1);
  DMA_Init(DMA2_Stream1, &DMA_InitStructuretx);
  DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF7);
  DMA_Cmd(DMA2_Stream1, ENABLE);  //transmit request

  TIM8->CR1 &= ~TIM_CR1_CEN;  // disable tim
  TIM8->ARR  = 20;            // 168 / 2 / (9 + 1) = 8.4MHz
  TIM8->DIER = TIM_DIER_UDE;  // cc3 dma
  // TIM8->CCMR2 = 0; // cc3 output
  // TIM8->CCR3 = 1;
  TIM8->CNT = 0;
  TIM8->CR1 |= TIM_CR1_CEN;

  DMA_Cmd(DMA1_Stream7, DISABLE);
  DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);
  DMA_Cmd(DMA1_Stream7, ENABLE);

  FB0_ENC_TIM->CR1 &= ~TIM_CR1_CEN;
  FB0_ENC_TIM->CCMR2 = TIM_CCMR2_CC3S_0;                                // cc3 input ti3
  FB0_ENC_TIM->CCER  = TIM_CCER_CC3E | TIM_CCER_CC3P | TIM_CCER_CC3NP;  // cc3 en, rising edge, falling edge
  FB0_ENC_TIM->ARR   = 65535;
  FB0_ENC_TIM->DIER  = TIM_DIER_CC3DE;  // cc3 dma
  FB0_ENC_TIM->CNT   = 0;
  FB0_ENC_TIM->CCR3  = 0;

  while(!(DMA2->LISR & DMA_FLAG_TCIF1))
    ;  //wait for request

  FB0_Z_TXEN_PORT->BSRRH = FB0_Z_TXEN_PIN;     //TX disable
  FB0_Z_PORT->MODER &= ~GPIO_MODER_MODER14_0;  //set tx pin to af
  FB0_Z_PORT->MODER |= GPIO_MODER_MODER14_1;

  FB0_ENC_TIM->CR1 |= TIM_CR1_CEN;  // enable tim

  if(PIN(error) > 0.0) {
    PIN(error_sum)
    ++;
  }
}

static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct yaskawa_ctx_t *ctx      = (struct yaskawa_ctx_t *)ctx_ptr;
  struct yaskawa_pin_ctx_t *pins = (struct yaskawa_pin_ctx_t *)pin_ptr;
  if(RISING_EDGE(PIN(dump))) {
    for(int i = 0; i < 14; i++) {
      for(int j = 0; j < 8; j++) {
        printf("%c", m_data2[i * 8 + j]);
      }
      printf("|");
    }
    printf("\n");
    dfdf = 0;
  }
}

hal_comp_t yaskawa_comp_struct = {
    .name      = "yaskawa",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct yaskawa_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
