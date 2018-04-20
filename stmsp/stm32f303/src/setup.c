#include "stm32f3xx_hal.h"
#include "adc.h"
#include "opamp.h"
// #include "tim.h"
#include "stmsp_f3hw.h"
#include "hal.h"


// void TIM8_UP_IRQHandler() {
//   GPIOA->BSRR |= GPIO_PIN_9;
//   __HAL_TIM_CLEAR_IT(&htim8, TIM_IT_UPDATE);
//   hal_run_rt();
//   if(__HAL_TIM_GET_FLAG(&htim8, TIM_IT_UPDATE) == SET) {
//     hal_stop();
//     hal.hal_state = RT_TOO_LONG;
//   }
//   GPIOA->BSRR |= GPIO_PIN_9 << 16;
// }
  TIM_HandleTypeDef htim1;

void DMA1_Channel1_IRQHandler() {
  DMA1->IFCR = DMA_IFCR_CTCIF1;
  DMA2->IFCR = DMA_IFCR_CTCIF5;

  hal_run_rt();
  //  if(__HAL_DMA_GET_FLAG(&hdma1, DMA_FLAG_TC1) == SET){
  //     hal_stop();
  //     hal.hal_state = RT_TOO_LONG;
  //  }
  // if(__HAL_TIM_GET_FLAG(&hdma1, DMA_FLAG_TC1) == SET) {
  //   hal_stop();
  //   hal.hal_state = RT_TOO_LONG;
  // }
  // reset
  IWDG->KR = 0xAAAA;
}

static void MX_TIM1_Init(void) {

  __HAL_RCC_TIM1_CLK_ENABLE();
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance               = TIM1;
  htim1.Init.Prescaler         = 0;
  htim1.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED3;
  htim1.Init.Period            = PWM_RES;
  htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  // htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  // htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_OC_Init(&htim1);

  sMasterConfig.MasterOutputTrigger  = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_UPDATE;
  sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

  sConfigOC.OCMode       = TIM_OCMODE_PWM1;
  sConfigOC.Pulse        = PWM_RES / 2;
  sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);

  HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);

  sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime         = 0;
  sBreakDeadTimeConfig.BreakState       = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter      = 0;
  sBreakDeadTimeConfig.Break2State      = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity   = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter     = 0;
  sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);

  GPIO_InitTypeDef GPIO_InitStruct;
  /**TIM1 GPIO Configuration    
  PB13     ------> TIM1_CH1N
  PB14     ------> TIM1_CH2N
  PA8     ------> TIM1_CH1
  PA9     ------> TIM1_CH2 
  */
  GPIO_InitStruct.Pin       = GPIO_PIN_13 | GPIO_PIN_14;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void hw_setup(){
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_ADC4_Init();

  MX_OPAMP2_Init();
  MX_OPAMP3_Init();
  MX_OPAMP4_Init();

  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED);

  HAL_OPAMP_SelfCalibrate(&hopamp2);
  HAL_OPAMP_SelfCalibrate(&hopamp3);
  HAL_OPAMP_SelfCalibrate(&hopamp4);

  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_RTC_ENABLE();

  CRC_HandleTypeDef hcrc;
  hcrc.Instance                     = CRC;
  hcrc.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;

  __HAL_RCC_CRC_CLK_ENABLE();

  if(HAL_CRC_Init(&hcrc) != HAL_OK) {
    Error_Handler();
  }

  if(HAL_OPAMP_Start(&hopamp2) != HAL_OK) {
    Error_Handler();
  }
  if(HAL_OPAMP_Start(&hopamp3) != HAL_OK) {
    Error_Handler();
  }
  if(HAL_OPAMP_Start(&hopamp4) != HAL_OK) {
    Error_Handler();
  }

  HAL_ADC_Start(&hadc1);
  HAL_ADC_Start(&hadc2);
  HAL_ADC_Start(&hadc3);
  HAL_ADC_Start(&hadc4);

  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  GPIO_InitTypeDef GPIO_InitStruct;

  // GPIO_InitStruct.Pin   = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  // GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  // GPIO_InitStruct.Pull  = GPIO_NOPULL;
  // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  // HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
}

void hal_setup(){
  hal_init(RT_FREQ > 0 ? 1.0 / RT_FREQ : 0.0, FRT_FREQ > 0 ? 1.0 / FRT_FREQ : 0.0);

  hal_parse("debug_level 1");

  hal_parse("load term");
  hal_parse("load sim");
  hal_parse("load io");
  hal_parse("load dq_sp");
  hal_parse("load idq");
  hal_parse("load tle");
  hal_parse("load pole");
  hal_parse("load map");
  hal_parse("load vel");
  hal_parse("load vel");
  hal_parse("load ypid");
  // hal_parse("load rl"));
  hal_parse("load hv");
  hal_parse("load curpid");

  hal_parse("io0.rt_prio = 1.0");  
  hal_parse("sim0.rt_prio = 2.0");  
  hal_parse("tle0.rt_prio = 2.0");
  hal_parse("map0.rt_prio = 3.0");
  hal_parse("vel0.rt_prio = 4.0");
  hal_parse("vel1.rt_prio = 4.0");
  hal_parse("pole0.rt_prio = 4.0");
  hal_parse("dq_sp0.rt_prio = 5.0");
  hal_parse("ypid0.rt_prio = 6.0");
  hal_parse("trc0.rt_prio = 6.5");
  hal_parse("curpid0.rt_prio = 7.0");
  hal_parse("idq0.rt_prio = 8.0");  
  hal_parse("rl0.rt_prio = 8.0");
  hal_parse("hv0.rt_prio = 9.0");
  
  hal_parse("term0.rt_prio = 10");
  // hal_parse("sim0.rt_prio = 10.0");

  hal_parse("term0.send_step = 10.0");
  hal_parse("term0.gain0 = 20.0");
  hal_parse("term0.gain1 = 20.0");
  hal_parse("term0.gain2 = 1.0");
  hal_parse("term0.gain3 = 1.0");
  hal_parse("term0.gain4 = 10.0");
  hal_parse("term0.gain5 = 10.0");
  hal_parse("term0.gain6 = 10.0");
  hal_parse("term0.gain7 = 10.0");

  hal_parse("hv0.a = idq0.a");
  hal_parse("hv0.b = idq0.b");
  hal_parse("hv0.udc = io0.dc_link");
  hal_parse("hv0.hv_temp = io0.hv_temp");

  hal_parse("curpid0.id_fb = dq_sp0.d");
  hal_parse("curpid0.iq_fb = dq_sp0.q");
  hal_parse("idq0.d = curpid0.ud");
  hal_parse("idq0.q = curpid0.uq");
  hal_parse("dq_sp0.a = io0.ia");
  hal_parse("dq_sp0.b = io0.ib");

  hal_parse("curpid0.rd = 1.6");
  hal_parse("curpid0.rq = 1.6");
  hal_parse("curpid0.ld = 0.0038");
  hal_parse("curpid0.lq = 0.0038");
  hal_parse("curpid0.psi = 0.005");
  hal_parse("curpid0.kp = 1.0");
  hal_parse("curpid0.ki = 0.0005");
  hal_parse("curpid0.ff = 1");
  hal_parse("curpid0.kind = 0");
  hal_parse("curpid0.max_cur = 3.1");
  hal_parse("curpid0.pwm_volt = io0.dc_link");

  hal_parse("pole0.p = 50.0");
  // hal_parse("io0.led = term0.con");

  hal_parse("dq_sp0.pos = pole0.cpos");
  hal_parse("idq0.pos = pole0.cpos");
  hal_parse("pole0.pos = map0.pos_out2");
  hal_parse("map0.pos_in = tle0.pos");
  hal_parse("map0.freq = 0.1");
  hal_parse("curpid0.cmd_mode = 1");
  hal_parse("vel1.pos_in = map0.pos_out2");
  hal_parse("ypid0.pos_ext_cmd = vel0.pos_out");
  hal_parse("ypid0.vel_ext_cmd = vel0.vel");
  hal_parse("ypid0.pos_fb = map0.pos_out2");
  hal_parse("ypid0.vel_fb = vel1.vel");
  hal_parse("curpid0.iq_cmd = ypid0.out");
  // hal_parse("trc0.in = ypid0.out");
  // hal_parse("curpid0.iq_cmd = trc0.out");
  // hal_parse("trc0.pos = map0.pos_out2");
  // hal_parse("trc0.mult = 200");
  
  hal_parse("ypid0.max_vel = 50");
  hal_parse("ypid0.max_acc = 10000");
  hal_parse("ypid0.max_out = 3");
  hal_parse("ypid0.pos_p = 100");
  hal_parse("ypid0.vel_p = 0.05");
  hal_parse("ypid0.vel_i = 0.05");
  hal_parse("ypid0.vel_ff = 1.0");
  
  hal_parse("term0.wave0 = vel0.pos_out");
  hal_parse("term0.wave1 = map0.pos_out2");
  hal_parse("term0.wave2 = vel0.vel");
  hal_parse("term0.wave3 = vel1.vel");
  
  hal_parse("vel0.w = 3000");
  hal_parse("vel1.w = 3000");

  hal_parse("vel0.en = 1");
  hal_parse("vel1.en = 1");
  hal_parse("ypid0.enable = 1");
  hal_parse("curpid0.en = 1");
  hal_parse("hv0.en = 1");

  hal_parse("rl0.ua_fb = hv0.a_fb");
  hal_parse("rl0.ub_fb = hv0.b_fb");
  hal_parse("rl0.ia_fb = io0.ia");
  hal_parse("rl0.ib_fb = io0.ib");
  hal_parse("term0.wave4 = rl0.ra");
  hal_parse("term0.wave5 = rl0.rb");
  hal_parse("term0.wave0 = rl0.t");
  hal_parse("term0.wave1 = rl0.state");
  
  hal_parse("hv0.a = rl0.ua");
  hal_parse("hv0.b = rl0.ub");
  
  hal_parse("flashloadconf");
  hal_parse("loadconf");
  
  hal_parse("debug_level 0");

  // reset
  // IWDG->KR = 0xAAAA;

  // hal_parse("flashloadconf");
  // hal_parse("loadconf");

  hal_parse("start");

  // hal parse config
  // hal_init_nrt();
  // error foo

  // hal_start();


  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
}