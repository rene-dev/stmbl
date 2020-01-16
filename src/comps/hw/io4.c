#include "commands.h"
#include "hal.h"
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_conf.h"
#include "hw/hw.h"
#include "main.h"
#include "common.h"

HAL_COMP(io);

HAL_PIN(fan);

HAL_PIN(state);
HAL_PIN(fault);

//outputs
HAL_PIN(out0);
HAL_PIN(out1);
HAL_PIN(out2);

HAL_PIN(out_freq);
HAL_PIN(out_arr);
HAL_PIN(out_cnt);
HAL_PIN(out_ccr1);

HAL_PIN(in0);    //input 0, analog
HAL_PIN(in1);    //input 1, analog
HAL_PIN(ind0);   //input 0, digital
HAL_PIN(ind1);   //input 1, digital
HAL_PIN(ind0n);  //input 0 inverted
HAL_PIN(ind1n);  //input 0 inverted
HAL_PIN(th0);    //voltage threshold in0
HAL_PIN(th1);    //voltage threshold in1

HAL_PIN(CTX);
HAL_PIN(CRX);
HAL_PIN(C12);
HAL_PIN(C36);
HAL_PIN(C54);
HAL_PIN(C78);
HAL_PIN(cmd_remap);

HAL_PIN(swd_remap);
HAL_PIN(DIO);
HAL_PIN(CK);

//rj45 leds
HAL_PIN(fb0g);
HAL_PIN(fb0y);

HAL_PIN(fb1g);
HAL_PIN(fb1y);

HAL_PIN(cmdg);
HAL_PIN(cmdy);

HAL_PIN(fb0);
HAL_PIN(fb1);
HAL_PIN(fbd0);
HAL_PIN(fbd1);
HAL_PIN(fbd0n);
HAL_PIN(fbd1n);
HAL_PIN(fbth0);
HAL_PIN(fbth1);

//a,b,z inputs of fb0 and fb1
HAL_PIN(fb0a);
HAL_PIN(fb0b);
HAL_PIN(fb0z);

HAL_PIN(fb1a);
HAL_PIN(fb1b);
HAL_PIN(fb1z);

HAL_PIN(fbsd);  //fb shutdown

HAL_PIN(master_arr);
HAL_PIN(clock_scale);

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct io_pin_ctx_t *pins = (struct io_pin_ctx_t *)pin_ptr;
  PIN(th0)                  = 12.0;
  PIN(th1)                  = 12.0;
  PIN(fbth0)                = 2.5;
  PIN(fbth1)                = 2.5;
  PIN(out_freq)             = 15000;
  PIN(master_arr) = TIM3->ARR;
  PIN(clock_scale) = 1.0;
}

static void hw_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct io_ctx_t * ctx = (struct io_ctx_t *)ctx_ptr;
  struct io_pin_ctx_t *pins = (struct io_pin_ctx_t *)pin_ptr;

  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;

  //**** ADC3 for analog input and fb temperature
  //TODO: ADC calibration?
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // out1, out2 pwm tim9
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9);

  if(PIN(swd_remap) > 0) {
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  }

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
  ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;  //data converted will be shifted to right
  ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;   //Input voltage is converted into a 12bit number giving a maximum value of 4096
  ADC_InitStructure.ADC_ContinuousConvMode   = DISABLE;              //the conversion is continuous, the input data is converted more than once
  ADC_InitStructure.ADC_ExternalTrigConv     = TIM_MASTER_ADC;       //trigger on rising edge of TIM_MASTER oc
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
  ADC_InitStructure.ADC_NbrOfConversion      = 1;       //ADC_ANZ;//I think this one is clear :p
  ADC_InitStructure.ADC_ScanConvMode         = ENABLE;  //The scan is configured in one channel
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_Init(ADC3, &ADC_InitStructure);
  ADC_InjectedSequencerLengthConfig(ADC3, 4);
  ADC_InjectedChannelConfig(ADC3, ADC_Channel_10, 1, ADC_SampleTime_144Cycles);
  ADC_InjectedChannelConfig(ADC3, ADC_Channel_11, 2, ADC_SampleTime_144Cycles);
  ADC_InjectedChannelConfig(ADC3, ADC_Channel_12, 3, ADC_SampleTime_144Cycles);
  ADC_InjectedChannelConfig(ADC3, ADC_Channel_13, 4, ADC_SampleTime_144Cycles);
  ADC_Cmd(ADC3, ENABLE);
  ADC_SoftwareStartInjectedConv(ADC3);
  //**** ADC3 end

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  //fan
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  //red
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  //yellow
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  //green
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  //in1 led
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  //in0 led
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  //out0
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  // //out1
  // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  // GPIO_Init(GPIOE, &GPIO_InitStructure);

  // //out2
  // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  // GPIO_Init(GPIOE, &GPIO_InitStructure);

  RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;

  TIM9->ARR = 186000000 / 300 / 15000; // 15000Hz 
  TIM9->PSC = 300 - 1; // 186e6 / 300 = 620000Hz max freq
  TIM9->CCMR1 = TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; // pwm mode 1
  TIM9->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E; // cc1, cc2 enable
  TIM9->CCR1 = 0;
  TIM9->CCR2 = 0;
  TIM9->CR1 = TIM_CR1_ARPE; // preloade
  TIM9->CR1 |= TIM_CR1_CEN; // counter enable

  //fb0 green
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  //fb0 yellow
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  //fb1 green
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  //fb1 yellow
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  //cmd
  // GPIO_InitStructure.GPIO_Pin   = CMD_C_EN_PIN;
  // GPIO_Init(CMD_C_EN_PORT, &GPIO_InitStructure);
  //
  // GPIO_InitStructure.GPIO_Pin   = CMD_D_EN_PIN;
  // GPIO_Init(CMD_D_EN_PORT, &GPIO_InitStructure);

  //cmd yellow
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  //cmd green
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  //fb 5v enable
  GPIO_SetBits(GPIOC, GPIO_Pin_13);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  if(PIN(cmd_remap) > 0) {
    // CMD EN
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin   = CMD_A_EN_PIN;
    GPIO_Init(CMD_A_EN_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = CMD_B_EN_PIN;
    GPIO_Init(CMD_B_EN_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = CMD_C_EN_PIN;
    GPIO_Init(CMD_C_EN_PORT, &GPIO_InitStructure);

    GPIO_SetBits(CMD_A_EN_PORT, CMD_A_EN_PIN);
    GPIO_SetBits(CMD_B_EN_PORT, CMD_B_EN_PIN);
    GPIO_SetBits(CMD_C_EN_PORT, CMD_C_EN_PIN);
  }
}

#include "../shared/hw_math.h"
#define ARES 4096.0
#define AREF 3.3

static void frt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct io_pin_ctx_t *pins = (struct io_pin_ctx_t *)pin_ptr;
  if(PIN(clock_scale) > 1.01){
    TIM3->ARR = PIN(master_arr) + 1;
  }
  else if(PIN(clock_scale) < 0.99){
    TIM3->ARR = PIN(master_arr) - 1;
  }
  else{
    TIM3->ARR = PIN(master_arr);
  }
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct io_ctx_t * ctx = (struct io_ctx_t *)ctx_ptr;
  struct io_pin_ctx_t *pins = (struct io_pin_ctx_t *)pin_ptr;
  uint32_t red              = 0;
  uint32_t green            = 0;
  uint32_t yellow           = 0;

  //TODO: unit conversion
  //TODO: check if adc sample complete?
  float in0 = V2(ADC2V(ADC_GetInjectedConversionValue(ADC3, ADC_InjectedChannel_1)), 3.3, 1000.0, 10000.0, 1000.0);
  float in1 = V2(ADC2V(ADC_GetInjectedConversionValue(ADC3, ADC_InjectedChannel_2)), 3.3, 1000.0, 10000.0, 1000.0);
  PIN(in0)  = in0;
  PIN(in1)  = in1;

  if(in0 > PIN(th0) + 0.1) {
    PIN(ind0)  = 1.0;
    PIN(ind0n) = 0.0;
    GPIO_SetBits(GPIOE, GPIO_Pin_1);
  } else if(in0 < PIN(th0) - 0.1) {
    PIN(ind0)  = 0.0;
    PIN(ind0n) = 1.0;
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);
  }

  if(in1 > PIN(th1) + 0.1) {
    PIN(ind1)  = 1.0;
    PIN(ind1n) = 0.0;
    GPIO_SetBits(GPIOE, GPIO_Pin_0);
  } else if(in1 < PIN(th1) - 0.1) {
    PIN(ind1)  = 0.0;
    PIN(ind1n) = 1.0;
    GPIO_ResetBits(GPIOE, GPIO_Pin_0);
  }


  in0      = V3(ADC2V(ADC_GetInjectedConversionValue(ADC3, ADC_InjectedChannel_3)), 10000.0, 1000.0);
  in1      = V3(ADC2V(ADC_GetInjectedConversionValue(ADC3, ADC_InjectedChannel_4)), 10000.0, 1000.0);
  PIN(fb0) = in0;
  PIN(fb1) = in1;

  ADC_SoftwareStartInjectedConv(ADC3);

  if(in0 > PIN(fbth0) + 0.1) {
    PIN(fbd0)  = 1.0;
    PIN(fbd0n) = 0.0;
  } else if(in0 < PIN(fbth0) - 0.1) {
    PIN(fbd0)  = 0.0;
    PIN(fbd0n) = 1.0;
  }

  if(in1 > PIN(fbth1) + 0.1) {
    PIN(fbd1)  = 1.0;
    PIN(fbd1n) = 0.0;
  } else if(in1 < PIN(fbth1) - 0.1) {
    PIN(fbd1)  = 0.0;
    PIN(fbd1n) = 1.0;
  }

  if(PIN(swd_remap) > 0) {
    PIN(DIO) = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13);
    PIN(CK)  = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14);
  }

  switch((state_t)PIN(state)) {
    case DISABLED:
      red    = 0;
      green  = 0;
      yellow = 1;
      break;

    case ENABLED:
      red    = 0;
      green  = 1;
      yellow = 0;
      break;

    case PHASING:
      red    = 0;
      green  = 1;
      yellow = 1;
      break;

    case SOFT_FAULT:
      red    = BLINK((int)PIN(fault));
      green  = 0;
      yellow = 0;
      break;

    case HARD_FAULT:
      red    = BLINK((int)PIN(fault));
      green  = BLINK((int)PIN(fault));
      yellow = BLINK((int)PIN(fault));
      break;

    case LED_TEST:
      red    = 1;
      green  = 1;
      yellow = 1;
      break;
  }

  if(red > 0)
    GPIO_SetBits(GPIOD, GPIO_Pin_3);
  else
    GPIO_ResetBits(GPIOD, GPIO_Pin_3);

  if(yellow > 0)
    GPIO_SetBits(GPIOD, GPIO_Pin_4);
  else
    GPIO_ResetBits(GPIOD, GPIO_Pin_4);

  if(green > 0)
    GPIO_SetBits(GPIOD, GPIO_Pin_5);
  else
    GPIO_ResetBits(GPIOD, GPIO_Pin_5);

  if(PIN(out0) > 0)
    GPIO_SetBits(GPIOE, GPIO_Pin_4);
  else
    GPIO_ResetBits(GPIOE, GPIO_Pin_4);

  // if(PIN(out1) > 0)
  //   GPIO_SetBits(GPIOE, GPIO_Pin_5);
  // else
  //   GPIO_ResetBits(GPIOE, GPIO_Pin_5);

  // if(PIN(out2) > 0)
  //   GPIO_SetBits(GPIOE, GPIO_Pin_6);
  // else
  //   GPIO_ResetBits(GPIOE, GPIO_Pin_6);

  TIM9->ARR = 186000000 / 300 / MAX(PIN(out_freq), 10);
  TIM9->CCR1 = CLAMP(PIN(out1), 0, 1) * TIM9->ARR;
  TIM9->CCR2 = CLAMP(PIN(out2), 0, 1) * TIM9->ARR;

  PIN(out_cnt) = TIM9->CNT;
  PIN(out_arr) = TIM9->ARR;
  PIN(out_ccr1) = TIM9->CCR1;

  if(PIN(fb0g) > 0)
    GPIO_SetBits(GPIOD, GPIO_Pin_8);
  else
    GPIO_ResetBits(GPIOD, GPIO_Pin_8);

  if(PIN(fb0y) > 0)
    GPIO_SetBits(GPIOD, GPIO_Pin_9);
  else
    GPIO_ResetBits(GPIOD, GPIO_Pin_9);

  if(PIN(fb1g) > 0)
    GPIO_SetBits(GPIOE, GPIO_Pin_7);
  else
    GPIO_ResetBits(GPIOE, GPIO_Pin_7);

  if(PIN(fb1y) > 0)
    GPIO_SetBits(GPIOE, GPIO_Pin_8);
  else
    GPIO_ResetBits(GPIOE, GPIO_Pin_8);

  // if(PIN(cmdc_en) > 0){
  //    GPIO_SetBits(CMD_C_EN_PORT, CMD_C_EN_PIN);
  //    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  //    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  //    GPIO_InitStructure.GPIO_Pin = CMD_C_PIN;
  //    GPIO_Init(CMD_C_PORT, &GPIO_InitStructure);
  //    if(PIN(cmdc) > 0){
  //       GPIO_SetBits(CMD_C_PORT, CMD_C_PIN);
  //    }
  //    else{
  //       GPIO_ResetBits(CMD_C_PORT, CMD_C_PIN);
  //    }
  // }
  // else{
  //    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  //    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  //    GPIO_InitStructure.GPIO_Pin = CMD_C_PIN;
  //    GPIO_Init(CMD_C_PORT, &GPIO_InitStructure);
  //    GPIO_ResetBits(CMD_C_EN_PORT, CMD_C_EN_PIN);
  //    PIN(cmdc) = (CMD_C_PORT->IDR & CMD_C_PIN) > 0;
  // }
  //
  // if(PIN(cmdd_en) > 0){
  //    GPIO_SetBits(CMD_D_EN_PORT, CMD_D_EN_PIN);
  //    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  //    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  //    GPIO_InitStructure.GPIO_Pin = CMD_C_PIN;
  //    GPIO_Init(CMD_D_PORT, &GPIO_InitStructure);
  //    if(PIN(cmdd) > 0){
  //       GPIO_SetBits(CMD_D_PORT, CMD_D_PIN);
  //    }
  //    else{
  //       GPIO_ResetBits(CMD_D_PORT, CMD_D_PIN);
  //    }
  // }
  // else{
  //    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  //    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  //    GPIO_InitStructure.GPIO_Pin = CMD_D_PIN;
  //    GPIO_Init(CMD_D_PORT, &GPIO_InitStructure);
  //    GPIO_ResetBits(CMD_D_EN_PORT, CMD_D_EN_PIN);
  //    PIN(cmdd) = (CMD_D_PORT->IDR & CMD_D_PIN) > 0;
  // }

  if(PIN(cmdy) > 0)
    GPIO_SetBits(GPIOD, GPIO_Pin_6);
  else
    GPIO_ResetBits(GPIOD, GPIO_Pin_6);

  if(PIN(cmdg) > 0)
    GPIO_SetBits(GPIOD, GPIO_Pin_7);
  else
    GPIO_ResetBits(GPIOD, GPIO_Pin_7);

  if(PIN(fan) > 0)
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
  else
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);

  if(PIN(fbsd) > 0)
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
  else
    GPIO_SetBits(GPIOC, GPIO_Pin_13);

  PIN(CRX) = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0);
  PIN(CTX) = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
  PIN(C12) = GPIO_ReadInputDataBit(CMD_A_PORT, CMD_A_PIN);
  PIN(C36) = GPIO_ReadInputDataBit(CMD_B_PORT, CMD_B_PIN);
  PIN(C54) = GPIO_ReadInputDataBit(CMD_C_PORT, CMD_C_PIN);
  PIN(C78) = GPIO_ReadInputDataBit(CMD_D_PORT, CMD_D_PIN);

  PIN(fb0a) = GPIO_ReadInputDataBit(FB0_A_PORT, FB0_A_PIN);
  PIN(fb0b) = GPIO_ReadInputDataBit(FB0_B_PORT, FB0_B_PIN);
  PIN(fb0z) = GPIO_ReadInputDataBit(FB0_Z_PORT, FB0_Z_PIN);

  PIN(fb1a) = GPIO_ReadInputDataBit(FB1_A_PORT, FB1_A_PIN);
  PIN(fb1b) = GPIO_ReadInputDataBit(FB1_B_PORT, FB1_B_PIN);
  PIN(fb1z) = GPIO_ReadInputDataBit(FB1_Z_PORT, FB1_Z_PIN);
}

hal_comp_t io_comp_struct = {
    .name      = "io",
    .nrt       = 0,
    .rt        = rt_func,
    .frt       = frt_func,
    .nrt_init  = nrt_init,
    .hw_init   = hw_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = 0,
    .pin_count = sizeof(struct io_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
