#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f3xx_hal.h"
#include "common.h"

HAL_COMP(ls);

//process data from LS 
HAL_PIN(d_cmd);
HAL_PIN(q_cmd);
HAL_PIN(pos);
HAL_PIN(vel);
HAL_PIN(en);

// config data from LS
HAL_PIN(mode);
HAL_PIN(r);
HAL_PIN(l);
HAL_PIN(psi);
HAL_PIN(cur_p);
HAL_PIN(cur_i);
HAL_PIN(cur_ff);
HAL_PIN(cur_ind);
HAL_PIN(max_y);
HAL_PIN(max_cur);

// process data to LS
HAL_PIN(dc_volt);
HAL_PIN(d_fb);
HAL_PIN(q_fb);

// state data to LS
HAL_PIN(hv_temp);
HAL_PIN(mot_temp);
HAL_PIN(core_temp);
HAL_PIN(fault);
HAL_PIN(y);
HAL_PIN(u_fb);
HAL_PIN(v_fb);
HAL_PIN(w_fb);

// misc
HAL_PIN(pwm_volt);
HAL_PIN(crc_error);


packet_to_hv_t packet_to_hv;
packet_from_hv_t packet_from_hv;
uint32_t timeout = 99999;

volatile uint8_t rxbuf[sizeof(packet_to_hv) * 2];
uint32_t rxpos = 0;//UART rx buffer position
int32_t datapos = -1;

f3_config_data_t config;
f3_state_data_t state;
uint8_t addr = 0;

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct ls_ctx_t * ctx = (struct ls_ctx_t *)ctx_ptr;
  // struct ls_pin_ctx_t * pins = (struct ls_pin_ctx_t *)pin_ptr;
  
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Peripheral clock enable */
  __HAL_RCC_USART3_CLK_ENABLE();
   
  UART_HandleTypeDef huart3;
  huart3.Instance = USART3;
  huart3.Init.BaudRate = DATABAUD;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_8;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart3);
  USART3->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
 
  /**USART3 GPIO Configuration    
  PB10     ------> USART3_TX
  PB11     ------> USART3_RX 
  */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
   
  __HAL_RCC_DMA1_CLK_ENABLE();
  
  //TX DMA
  DMA1_Channel2->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA1_Channel2->CPAR = (uint32_t)&(USART3->TDR);
  DMA1_Channel2->CMAR = (uint32_t)&packet_from_hv;
  DMA1_Channel2->CNDTR = sizeof(packet_from_hv);
  DMA1_Channel2->CCR = DMA_CCR_MINC | DMA_CCR_DIR;// | DMA_CCR_PL_0 | DMA_CCR_PL_1
  DMA1->IFCR = DMA_IFCR_CTCIF2 | DMA_IFCR_CHTIF2 | DMA_IFCR_CGIF2;
	
  packet_from_hv.head.start = 255;
  packet_from_hv.head.key = 0;
   
  //RX DMA
  DMA1_Channel3->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA1_Channel3->CPAR = (uint32_t)&(USART3->RDR);
  DMA1_Channel3->CMAR = (uint32_t)rxbuf;
  DMA1_Channel3->CNDTR = sizeof(rxbuf);
  DMA1_Channel3->CCR = DMA_CCR_MINC | DMA_CCR_CIRC;// | DMA_CCR_PL_0 | DMA_CCR_PL_1
  DMA1->IFCR = DMA_IFCR_CTCIF3 | DMA_IFCR_CHTIF3 | DMA_IFCR_CGIF3;
  DMA1_Channel3->CCR |= DMA_CCR_EN;
   
  config.pins.mode = 0.0;
  config.pins.r = 0.0;
  config.pins.l = 0.0;
  config.pins.psi = 0.0;
  config.pins.cur_p = 0.0;
  config.pins.cur_i = 0.0;
  config.pins.cur_ff = 0.0;
  config.pins.cur_ind = 0.0;
  config.pins.max_y = 0.0;
  config.pins.max_cur = 0.0;
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct ls_ctx_t * ctx = (struct ls_ctx_t *)ctx_ptr;
  struct ls_pin_ctx_t * pins = (struct ls_pin_ctx_t *)pin_ptr;

  //next received packet will be written to rxbufferpos
  uint32_t rxbufferpos = sizeof(rxbuf) - DMA1_Channel3->CNDTR;
  //how many packets we have the the rx buffer for processing
  uint32_t rxavailable = (rxbufferpos - rxpos + sizeof(rxbuf)) % sizeof(rxbuf);
   
  // TODO: sin = 0.5
  if(config.pins.mode == 0){// 90°
    PIN(pwm_volt) = PIN(dc_volt) / M_SQRT2 * 0.95;
  }else if(config.pins.mode == 1){// 120°
    PIN(pwm_volt) = PIN(dc_volt) / M_SQRT3 * 0.95;
  }else if(config.pins.mode == 2){// 180°
    PIN(pwm_volt) = PIN(dc_volt) * 0.95;
  }else{
    PIN(pwm_volt) = 0.0;
  }
      
  state.pins.u_fb = PIN(u_fb);
  state.pins.v_fb = PIN(v_fb);
  state.pins.w_fb = PIN(w_fb);
  state.pins.hv_temp = PIN(hv_temp);
  state.pins.mot_temp = PIN(mot_temp);
  state.pins.core_temp = PIN(core_temp);
  state.pins.fault = PIN(fault);
  state.pins.y = PIN(y);
      
  for(int i = 0;i < rxavailable;i++){
    uint8_t buf = rxbuf[(rxpos)%sizeof(rxbuf)];
    if(buf == 255){ //start condition
      datapos = 0;
      ((uint8_t*)&packet_to_hv)[datapos++] = (uint8_t)buf;
      packet_from_hv.data.dc_volt = PIN(dc_volt);
      packet_from_hv.data.pwm_volt = PIN(pwm_volt);
      packet_from_hv.data.d_fb =  PIN(d_fb);
      packet_from_hv.data.q_fb =  PIN(q_fb);
      packet_from_hv.data.addr = addr;
      packet_from_hv.data.value = state.data[addr++];
      addr %= sizeof(state) / 4;
         
      buff_packet((packet_header_t*)&packet_from_hv, sizeof(from_hv_t));
      DMA1_Channel2->CCR &= (uint16_t)(~DMA_CCR_EN);
      DMA1_Channel2->CNDTR = sizeof(packet_from_hv);
      DMA1_Channel2->CCR |= DMA_CCR_EN;
    }else if(datapos >= 0 && datapos < sizeof(packet_to_hv_t)){//append data
      ((uint8_t*)&packet_to_hv)[datapos++] = (uint8_t)buf;
    }
    if(datapos == sizeof(packet_to_hv_t)){//all data received
      datapos = -1;
      if(unbuff_packet((packet_header_t*)&packet_to_hv, sizeof(to_hv_t))){
        PIN(en) = packet_to_hv.data.enable;
        PIN(d_cmd) = packet_to_hv.data.d_cmd;
        PIN(q_cmd) = packet_to_hv.data.q_cmd;
        PIN(pos) = packet_to_hv.data.pos;
        PIN(vel) = packet_to_hv.data.vel;
        uint8_t a = packet_to_hv.data.addr;
        a = CLAMP(a, 0, sizeof(config) / 4);
        config.data[a] = packet_to_hv.data.value; // TODO: first enable after complete update
        timeout = 0; //reset timeout
      }else{
        PIN(crc_error)++;
      }

    }
      
    rxpos++;
    rxpos = rxpos % sizeof(rxbuf);
  }
   
  PIN(mode) = config.pins.mode;
  PIN(r) = config.pins.r;
  PIN(l) = config.pins.l;
  PIN(psi) = config.pins.psi;
  PIN(cur_p) = config.pins.cur_p;
  PIN(cur_i) = config.pins.cur_i;
  PIN(cur_ff) = config.pins.cur_ff;
  PIN(cur_ind) = config.pins.cur_ind;
  PIN(max_y) = config.pins.max_y;
  PIN(max_cur) = config.pins.max_cur;
   
  if(timeout > 10){//disable driver
    PIN(en) = 0.0;
  }else{
    timeout ++;
  }
}

hal_comp_t ls_comp_struct = {
  .name = "ls",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = nrt_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = 0,
  .pin_count = sizeof(struct ls_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
