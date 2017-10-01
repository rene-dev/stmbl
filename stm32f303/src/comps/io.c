#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f3xx_hal.h"
#include "f3hw.h"

HAL_COMP(io);

HAL_PIN(led);

HAL_PIN(iu);
HAL_PIN(iv);
HAL_PIN(iw);

HAL_PIN(u);
HAL_PIN(v);
HAL_PIN(w);
HAL_PIN(udc);
HAL_PIN(udc_pwm);

HAL_PIN(in0);
HAL_PIN(in1);
HAL_PIN(in2);

HAL_PIN(hv_temp);
HAL_PIN(temp_v);
HAL_PIN(mot_temp);

uint32_t adc_12_buf[10];
uint32_t adc_34_buf[10];

struct io_ctx_t{
  float u_offset;
  float v_offset;
  float w_offset;
};

#define ARES 4096.0// analog resolution, 12 bit
#define ADC(a) ((a) / ARES * AREF)

#define HV_TEMP_PULLUP 10000
#define HV_R(a) (HV_TEMP_PULLUP / (AREF / (a) - 1))

#define MOT_TEMP_PULLUP 10000
#define MOT_TEMP_PULLMID 51000
#define MOT_TEMP_PULLDOWN 10000
#define MOT_TEMP_REF 15.26
#define MOT_REF(a) ((a) * (MOT_TEMP_PULLMID + MOT_TEMP_PULLDOWN) / MOT_TEMP_PULLDOWN)
#define MOT_R(a) (MOT_TEMP_PULLUP / (MOT_TEMP_REF / (a) - 1))

#define ARES 4096.0// analog resolution, 12 bit

#define VOLT(a) ((a) / (ARES) * (AREF) / (VDIVDOWN) * ((VDIVUP) + (VDIVDOWN)))
//#define TEMP(a) (log10f((a) * (AREF) / (ARES) * (TPULLUP) / ((AREF) - (a) * (AREF) / (ARES))) * (-53.0) + 290.0)

#define SHUNT_GAIN 16.0

#define AMP(a, gain) (((a) * AREF / ARES / (gain) - AREF / (SHUNT_PULLUP + SHUNT_SERIE) * SHUNT_SERIE) / (SHUNT * SHUNT_PULLUP) * (SHUNT_PULLUP + SHUNT_SERIE))


float r2temp(float r){
  r = r / 1000;
  const int step = 10;
  const int start = -10;
  //-10..100
  const float temp[] = {271.7, 158.2, 95.23, 59.07, 37.64, 24.59, 16.43, 11.21, 7.798, 5.518, 3.972, 2.902};

  for(int i = 1; i < ARRAY_SIZE(temp); i++){
    if(temp[i] < r){
      float a = temp[i - 1];
      float b = temp[i];
      return(-(r - b) / (a - b) * step + i * step + start);
    }
  }
  return(temp[ARRAY_SIZE(temp)] + step);
}

static void nrt_init(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  // struct io_ctx_t * ctx = (struct io_ctx_t *)ctx_ptr;
  // struct io_pin_ctx_t * pins = (struct io_pin_ctx_t *)pin_ptr;
  GPIO_InitTypeDef GPIO_InitStruct;
  //PA8 LED
  /*GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);*/

  /**TIM1 GPIO Configuration
  PA8     ------> IN0
  PA9     ------> IN1
  PA10    ------> IN2
  */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  DMA1_Channel1->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA1_Channel1->CPAR = (uint32_t)&(ADC12_COMMON->CDR);
  DMA1_Channel1->CMAR = (uint32_t)adc_12_buf;
  DMA1_Channel1->CNDTR = 6;
  DMA1_Channel1->CCR = DMA_CCR_MINC | DMA_CCR_PL_0 | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
  ADC1->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
  DMA1_Channel1->CCR |= DMA_CCR_EN;

  //   ADC12_COMMON->CCR |= ADC12_CCR_MDMA_1;

  DMA2_Channel5->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA2_Channel5->CPAR = (uint32_t)&(ADC34_COMMON->CDR);
  DMA2_Channel5->CMAR = (uint32_t)adc_34_buf;
  DMA2_Channel5->CNDTR = 6;
  DMA2_Channel5->CCR = DMA_CCR_MINC | DMA_CCR_PL_0 | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
  ADC3->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
  DMA2_Channel5->CCR |= DMA_CCR_EN;

  //   ADC34_COMMON->CCR |= ADC34_CCR_MDMA_1;
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  struct io_ctx_t * ctx = (struct io_ctx_t *)ctx_ptr;
  struct io_pin_ctx_t * pins = (struct io_pin_ctx_t *)pin_ptr;

  while(!(DMA1->ISR & DMA_ISR_TCIF1)){}
  while(!(DMA2->ISR & DMA_ISR_TCIF5)){}

  DMA1->IFCR = DMA_IFCR_CTCIF1;
  DMA2->IFCR = DMA_IFCR_CTCIF5;

  uint32_t a12 = adc_12_buf[0] + adc_12_buf[1] + adc_12_buf[2] + adc_12_buf[3] + adc_12_buf[4];
  uint32_t a34 = adc_34_buf[0] + adc_34_buf[1] + adc_34_buf[2] + adc_34_buf[3] + adc_34_buf[4];

  if(ctx->u_offset == 0){
    ctx->w_offset = AMP((float)(a12 & 0xFFFF) / 5.0, SHUNT_GAIN);
    ctx->u_offset = AMP((float)(a12 >> 16) / 5.0, SHUNT_GAIN);
    ctx->v_offset = AMP((float)(a34 & 0xFFFF) / 5.0, SHUNT_GAIN);
  }

  PIN(iw) = -AMP((float)(a12 & 0xFFFF) / 5.0, SHUNT_GAIN) + ctx->w_offset; // 1u
  PIN(iu) = -AMP((float)(a12 >> 16) / 5.0, SHUNT_GAIN) + ctx->u_offset;
  PIN(iv) = -AMP((float)(a34 & 0xFFFF) / 5.0, SHUNT_GAIN) + ctx->v_offset;
  PIN(w) = VOLT(adc_12_buf[5] & 0xFFFF) * 0.05 + PIN(w) * 0.95; // 0.6u
  PIN(v) = VOLT(adc_12_buf[5] >> 16) * 0.05 + PIN(v) * 0.95;
  PIN(u) = VOLT(adc_34_buf[5] & 0xFFFF) * 0.05 + PIN(u) * 0.95;
  PIN(udc) = VOLT(adc_34_buf[5] >> 16) * 0.05 + PIN(udc) * 0.95;
  PIN(udc_pwm) = PIN(udc) / 2.0;

  PIN(temp_v) = ADC(adc_34_buf[4] >> 16);
  PIN(hv_temp) = r2temp(HV_R(ADC(adc_34_buf[4] >> 16))) * 0.01 + PIN(hv_temp) * 0.99; // 5.5u
  PIN(mot_temp) = MOT_R(MOT_REF(ADC(adc_34_buf[0] >> 16))); // 1.4u
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, PIN(led) > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET); // 0.1u

  PIN(in0) = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8);
  PIN(in1) = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9);
  PIN(in2) = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10);
}

hal_comp_t io_comp_struct = {
  .name = "io",
  .nrt = 0,
  .rt = rt_func,
  .frt = 0,
  .nrt_init = nrt_init,
  .rt_start = 0,
  .frt_start = 0,
  .rt_stop = 0,
  .frt_stop = 0,
  .ctx_size = sizeof(struct io_ctx_t),
  .pin_count = sizeof(struct io_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
