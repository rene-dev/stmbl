#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f3xx_hal.h"
#include "f3hw.h"

HAL_COMP(io);

HAL_PIN(led);

HAL_PIN(oc1);
HAL_PIN(oc2);
HAL_PIN(ena);
HAL_PIN(enb);

HAL_PIN(hv_temp);
HAL_PIN(dc_link);
HAL_PIN(bemf0);
HAL_PIN(bemf1);
HAL_PIN(in0);
HAL_PIN(in1);
HAL_PIN(iap);
HAL_PIN(ian);
HAL_PIN(ibp);
HAL_PIN(ibn);
HAL_PIN(ip);
HAL_PIN(in);
HAL_PIN(ia);
HAL_PIN(ib);

// uint32_t adc_12_buf[80];
// uint32_t adc_34_buf[80];

#pragma pack(1)
struct adc_12_t{
  uint16_t dc_link;
  uint16_t shunt_low0;
  uint16_t bemf0;
  uint16_t bemf1;
  uint16_t in0;
  uint16_t in1;
  uint16_t hv_temp;
  uint16_t shunt_low1;
};

#pragma pack(1)
struct adc_34_t{
  uint16_t shunt_a0;
  uint16_t shunt_b0;
  uint16_t shunt_a1;
  uint16_t shunt_b1;
  uint16_t shunt_a2;
  uint16_t shunt_b2;
  uint16_t shunt_a3;
  uint16_t shunt_b3;
};

volatile struct adc_12_t adc_12_buf[20];
volatile struct adc_34_t adc_34_buf[20];

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

#define SHUNT_GAIN 8.0

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
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
   
  DMA1_Channel1->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA1_Channel1->CPAR = (uint32_t)&(ADC12_COMMON->CDR);
  DMA1_Channel1->CMAR = (uint32_t)adc_12_buf;
  DMA1_Channel1->CNDTR = 80;
  DMA1_Channel1->CCR = DMA_CCR_MINC | DMA_CCR_PL_0 | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
  ADC1->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
  DMA1_Channel1->CCR |= DMA_CCR_TCIE;
  DMA1_Channel1->CCR |= DMA_CCR_EN;

  //   ADC12_COMMON->CCR |= ADC12_CCR_MDMA_1;

  DMA2_Channel5->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA2_Channel5->CPAR = (uint32_t)&(ADC34_COMMON->CDR);
  DMA2_Channel5->CMAR = (uint32_t)adc_34_buf;
  DMA2_Channel5->CNDTR = 80;
  DMA2_Channel5->CCR = DMA_CCR_MINC | DMA_CCR_PL_0 | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
  ADC3->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
  DMA2_Channel5->CCR |= DMA_CCR_EN;

  //   ADC34_COMMON->CCR |= ADC34_CCR_MDMA_1;
}

static void rt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
  struct io_ctx_t * ctx = (struct io_ctx_t *)ctx_ptr;
  struct io_pin_ctx_t * pins = (struct io_pin_ctx_t *)pin_ptr;

  uint32_t dc_link = 0;
  uint32_t hv_temp = 0;
  uint32_t bemf0 = 0;
  uint32_t bemf1 = 0;
  uint32_t in0 = 0;
  uint32_t in1 = 0;
  uint32_t iap = 0;
  uint32_t ian = 0;
  uint32_t ibp = 0;
  uint32_t ibn = 0;
  uint32_t ip = 0;
  uint32_t in = 0;

  for(int i = 0; i < 10; i++){
    dc_link += adc_12_buf[2 * i].dc_link + adc_12_buf[2 * i + 1].dc_link;
    hv_temp += adc_12_buf[2 * i].hv_temp + adc_12_buf[2 * i + 1].hv_temp;
    in0 += adc_12_buf[2 * i].in0 + adc_12_buf[2 * i + 1].in0;
    in1 += adc_12_buf[2 * i].in1 + adc_12_buf[2 * i + 1].in1;
    bemf0 += adc_12_buf[2 * i].bemf0 + adc_12_buf[2 * i + 1].bemf0;
    bemf1 += adc_12_buf[2 * i].bemf1 + adc_12_buf[2 * i + 1].bemf1;
    ian += adc_34_buf[2 * i].shunt_a0 + adc_34_buf[2 * i].shunt_a1 + adc_34_buf[2 * i].shunt_a2 + adc_34_buf[2 * i].shunt_a3;
    iap += adc_34_buf[2 * i + 1].shunt_a0 + adc_34_buf[2 * i + 1].shunt_a1 + adc_34_buf[2 * i + 1].shunt_a2 + adc_34_buf[2 * i + 1].shunt_a3;
    ibn += adc_34_buf[2 * i].shunt_b0 + adc_34_buf[2 * i].shunt_b1 + adc_34_buf[2 * i].shunt_b2 + adc_34_buf[2 * i].shunt_b3;
    ibp += adc_34_buf[2 * i + 1].shunt_b0 + adc_34_buf[2 * i + 1].shunt_b1 + adc_34_buf[2 * i + 1].shunt_b2 + adc_34_buf[2 * i + 1].shunt_b3;
    in += adc_12_buf[2 * i].shunt_low0 + adc_12_buf[2 * i].shunt_low1;
    ip += adc_12_buf[2 * i + 1].shunt_low0 + adc_12_buf[2 * i + 1].shunt_low1;
  }


  PIN(hv_temp) = 258.0 - hv_temp / 20.0 * 3.3 / ARES * 114.4;
  PIN(dc_link) = dc_link / 20.0 * 3.3 / ARES * (20.0 + 1.0) / 1.0;
  PIN(bemf0) = bemf0 / 20.0 * 3.3 / ARES * (20.0 + 1.0) / 1.0;
  PIN(bemf1) = bemf1 / 20.0 * 3.3 / ARES * (20.0 + 1.0) / 1.0;
  PIN(in0) = in0 / 20.0 * 3.3 / ARES * (10.0 + 1.5) / 1.5;
  PIN(in1) = in1 / 20.0 * 3.3 / ARES * (10.0 + 1.5) / 1.5;
  
  PIN(iap) = AMP(iap / 40.0, 8.0);
  PIN(ian) = AMP(ian / 40.0, 8.0);
  PIN(ibp) = AMP(ibp / 40.0, 8.0);
  PIN(ibn) = AMP(ibn / 40.0, 8.0);
  PIN(ip) = ip / 20.0 * 3.3 / ARES;
  PIN(in) = in / 20.0 * 3.3 / ARES;
  PIN(ia) = PIN(iap) - PIN(ian);
  PIN(ib) = PIN(ibp) - PIN(ibn);

  if(PIN(dc_link) > 10.0){
    TIM1->CCR1 = PIN(oc1) / PIN(dc_link) * 720.0 + 720.0;
    TIM1->CCR2 = PIN(oc2) / PIN(dc_link) * 720.0 + 720.0;
  }
  else{
    TIM1->CCR1 = 720;
    TIM1->CCR2 = 720;
  }

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, PIN(led) > 0.0 ? GPIO_PIN_SET : GPIO_PIN_RESET); // 0.1u

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
