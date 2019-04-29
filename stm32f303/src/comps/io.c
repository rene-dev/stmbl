#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "stm32f3xx_hal.h"
#include "f3hw.h"
#include "common.h"

HAL_COMP(io);

HAL_PIN(led);

//phase current
HAL_PIN(iu);
HAL_PIN(iv);
HAL_PIN(iw);
//total current
HAL_PIN(iabs);

//phase voltage
HAL_PIN(u);
HAL_PIN(v);
HAL_PIN(w);
//dclink voltage
HAL_PIN(udc);

//driver temoerature
HAL_PIN(hv_temp);
//motor temperature
HAL_PIN(mot_temp);

//ADC offset outputs
HAL_PIN(uo);
HAL_PIN(vo);
HAL_PIN(wo);

//DAC value for comperators
HAL_PIN(dac);

//comperator outputs
HAL_PIN(cu);
HAL_PIN(cv);
HAL_PIN(cw);

//enable in
HAL_PIN(hv_en);

//fault output
HAL_PIN(fault);
HAL_PIN(ignore_fault_pin);

HAL_PIN(brk_present);
HAL_PIN(brk);


volatile uint32_t adc_12_buf[6];
volatile uint32_t adc_34_buf[6];

struct io_ctx_t {
  float u_offset;
  float v_offset;
  float w_offset;
  float overtemp_error;
  float overvoltage_error;
  float overcurrent_error;
  float fault_pin_error;
  uint32_t offset_count;
  uint32_t hv_temp;
  uint32_t mot_temp;
  uint32_t fault;
  uint32_t enabled;
};

#define ARES 4096.0  // analog resolution, 12 bit
#define ADC(a) ((a) / ARES * AREF)

#define HV_TEMP_PULLUP 10000
#define HV_R(a) (HV_TEMP_PULLUP / (AREF / (a)-1))

#define MOT_TEMP_PULLUP 10000
#define MOT_TEMP_PULLMID 51000
#define MOT_TEMP_PULLDOWN 10000
#define MOT_TEMP_REF 15.26
#define MOT_REF(a) ((a) * (MOT_TEMP_PULLMID + MOT_TEMP_PULLDOWN) / MOT_TEMP_PULLDOWN)
#define MOT_R(a) (MOT_TEMP_PULLUP / (MOT_TEMP_REF / (a)-1))

#define VOLT(a) ((a) / (ARES) * (AREF) / (VDIVDOWN) * ((VDIVUP) + (VDIVDOWN)))
//#define TEMP(a) (log10f((a) * (AREF) / (ARES) * (TPULLUP) / ((AREF) - (a) * (AREF) / (ARES))) * (-53.0) + 290.0)

#define SHUNT_GAIN 16.0

#define AMP(a, gain) (((a)*AREF / ARES / (gain)-AREF / (SHUNT_PULLUP + SHUNT_SERIE) * SHUNT_SERIE) / (SHUNT * SHUNT_PULLUP) * (SHUNT_PULLUP + SHUNT_SERIE))


float r2temp(float r) {
  r               = r / 1000;
  const int step  = 10;
  const int start = -10;
  //-10..100
  const float temp[] = {271.7, 158.2, 95.23, 59.07, 37.64, 24.59, 16.43, 11.21, 7.798, 5.518, 3.972, 2.902};

  for(int i = 1; i < ARRAY_SIZE(temp); i++) {
    if(temp[i] < r) {
      float a = temp[i - 1];
      float b = temp[i];
      return (-(r - b) / (a - b) * step + i * step + start);
    }
  }
  return (temp[ARRAY_SIZE(temp) - 1] + step);  // TODO fix
}

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct io_ctx_t *ctx      = (struct io_ctx_t *)ctx_ptr;
  struct io_pin_ctx_t *pins = (struct io_pin_ctx_t *)pin_ptr;

  PIN(brk_present) = 0.0; 
  PIN(brk) = 0.0;

  GPIO_InitTypeDef GPIO_InitStruct;
  //LED
  GPIO_InitStruct.Pin   = LED_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

  // BRK
  GPIO_InitStruct.Pin   = BRK_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BRK_PORT, &GPIO_InitStruct);

  if(HAL_GPIO_ReadPin(BRK_PORT, BRK_PIN)){ // BRK circuit detected
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(BRK_PORT, &GPIO_InitStruct);
    PIN(brk_present) = 1.0;
  }

  DMA1_Channel1->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA1_Channel1->CPAR  = (uint32_t) & (ADC12_COMMON->CDR);
  DMA1_Channel1->CMAR  = (uint32_t)adc_12_buf;
  DMA1_Channel1->CNDTR = ARRAY_SIZE(adc_12_buf);
  DMA1_Channel1->CCR   = DMA_CCR_MINC | DMA_CCR_PL_0 | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
  ADC1->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
  DMA1_Channel1->CCR |= DMA_CCR_EN;

  //   ADC12_COMMON->CCR |= ADC12_CCR_MDMA_1;

  DMA2_Channel5->CCR &= (uint16_t)(~DMA_CCR_EN);
  DMA2_Channel5->CPAR  = (uint32_t) & (ADC34_COMMON->CDR);
  DMA2_Channel5->CMAR  = (uint32_t)adc_34_buf;
  DMA2_Channel5->CNDTR = ARRAY_SIZE(adc_34_buf);
  DMA2_Channel5->CCR   = DMA_CCR_MINC | DMA_CCR_PL_0 | DMA_CCR_MSIZE_1 | DMA_CCR_PSIZE_1 | DMA_CCR_CIRC;
  ADC3->CFGR |= ADC_CFGR_DMAEN | ADC_CFGR_DMACFG;
  DMA2_Channel5->CCR |= DMA_CCR_EN;

  //   ADC34_COMMON->CCR |= ADC34_CCR_MDMA_1;

  ctx->offset_count      = 0;
  ctx->u_offset          = 0.0;
  ctx->v_offset          = 0.0;
  ctx->w_offset          = 0.0;
  ctx->fault             = NO_ERROR;
  ctx->overtemp_error    = 0;
  ctx->overvoltage_error = 0;
  ctx->overcurrent_error = 0;
  ctx->fault_pin_error   = 0;
  ctx->hv_temp           = 0;
  ctx->mot_temp          = 0;
  ctx->enabled           = 0;


#ifdef HV_EN_PIN
  GPIO_InitStruct.Pin   = HV_EN_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HV_EN_PORT, &GPIO_InitStruct);
#endif

#ifdef HV_FAULT_PIN
  GPIO_InitStruct.Pin  = HV_FAULT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(HV_FAULT_PORT, &GPIO_InitStruct);
#endif
  PIN(dac) = 0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct io_ctx_t *ctx      = (struct io_ctx_t *)ctx_ptr;
  struct io_pin_ctx_t *pins = (struct io_pin_ctx_t *)pin_ptr;

  while(!(DMA1->ISR & DMA_ISR_TCIF1)) {
  }
  while(!(DMA2->ISR & DMA_ISR_TCIF5)) {
  }

  DMA1->IFCR = DMA_IFCR_CTCIF1;
  DMA2->IFCR = DMA_IFCR_CTCIF5;

  uint32_t a12 = adc_12_buf[0] + adc_12_buf[1] + adc_12_buf[2] + adc_12_buf[3] + adc_12_buf[4];
  uint32_t a34 = adc_34_buf[0] + adc_34_buf[1] + adc_34_buf[2] + adc_34_buf[3] + adc_34_buf[4];

  if(ctx->offset_count < 100) {
    ctx->offset_count++;
  } else if(ctx->offset_count < 100 + 100) {
    ctx->w_offset += AMP((float)(a12 & 0xFFFF) / 5.0, SHUNT_GAIN) / 100.0;
    ctx->u_offset += AMP((float)(a12 >> 16) / 5.0, SHUNT_GAIN) / 100.0;
    ctx->v_offset += AMP((float)(a34 & 0xFFFF) / 5.0, SHUNT_GAIN) / 100.0;
    ctx->offset_count++;
  } else if(ctx->offset_count < 100 + 100 + 1) {
    if(ABS(ctx->u_offset) > 5.0 || ABS(ctx->v_offset) > 5.0 || ABS(ctx->w_offset) > 5.0) {
      ctx->fault = HV_CURRENT_OFFSET_FAULT;
    }
    ctx->offset_count++;
  } else {
    PIN(uo)       = ctx->u_offset;
    PIN(vo)       = ctx->v_offset;
    PIN(wo)       = ctx->w_offset;
    PIN(iw)       = -AMP((float)(a12 & 0xFFFF) / 5.0, SHUNT_GAIN) + ctx->w_offset;  // 1u
    PIN(iu)       = -AMP((float)(a12 >> 16) / 5.0, SHUNT_GAIN) + ctx->u_offset;
    PIN(iv)       = -AMP((float)(a34 & 0xFFFF) / 5.0, SHUNT_GAIN) + ctx->v_offset;
    PIN(w)        = VOLT(adc_12_buf[5] & 0xFFFF) * 0.05 + PIN(w) * 0.95;  // 0.6u
    PIN(v)        = VOLT(adc_12_buf[5] >> 16) * 0.05 + PIN(v) * 0.95;
    PIN(u)        = VOLT(adc_34_buf[5] & 0xFFFF) * 0.05 + PIN(u) * 0.95;
    PIN(udc)      = VOLT(adc_34_buf[5] >> 16) * 0.05 + PIN(udc) * 0.95;
    PIN(iabs)     = MAX3(ABS(PIN(iu)), PIN(iv), PIN(iw));
    ctx->hv_temp  = adc_34_buf[0];
    ctx->mot_temp = adc_34_buf[3];

    if(err_filter(&(ctx->overtemp_error), 5.0, 0.001, PIN(hv_temp) > ABS_MAX_TEMP)) {
      ctx->fault = HV_TEMP_ERROR;
    }

    if(err_filter(&(ctx->overvoltage_error), 5.0, 0.001, PIN(udc) > ABS_MAX_VOLT)) {
      ctx->fault = HV_VOLT_ERROR;
    }

    if(err_filter(&(ctx->overcurrent_error), 5.0, 0.001, PIN(iabs) > ABS_MAX_CURRENT * 0.95)) {
      ctx->fault = HV_OVERCURRENT_RMS;
    }

    if(PIN(iabs) > ABS_MAX_CURRENT) {
      ctx->fault = HV_OVERCURRENT_PEAK;
    }

    PIN(fault) = ctx->fault;

    if(PIN(hv_en) > 0.0) {
      if(!ctx->enabled) {  //rising edge of enable
        //set timer master out enable
        TIM8->BDTR |= TIM_BDTR_MOE;
#ifdef HV_EN_PIN
        //set driver enable pin
        HAL_GPIO_WritePin(HV_EN_PORT, HV_EN_PIN, GPIO_PIN_SET);
#endif
        ctx->enabled = 1;
      }
      if(ctx->fault == NO_ERROR) {
#ifdef HV_FAULT_PIN
        //read fault pin from driver
        if(PIN(ignore_fault_pin) <= 0.0 && err_filter(&(ctx->fault_pin_error), 5.0, 0.01, HAL_GPIO_ReadPin(HV_FAULT_PORT, HV_FAULT_PIN) == HV_FAULT_POLARITY)) {
          ctx->fault = HV_FAULT_ERROR;
        }
#endif
        //Master out enable is cleared by timer break input.
        //Timer break input is connected to comperators
        if(!(TIM8->BDTR & TIM_BDTR_MOE)) {
          ctx->fault = HV_OVERCURRENT_HW;
        }
      } else {
        ctx->fault_pin_error = 0;
#ifdef HV_EN_PIN
        //clear driver enable pin
        HAL_GPIO_WritePin(HV_EN_PORT, HV_EN_PIN, GPIO_PIN_RESET);
#endif
      }
    } else {
      ctx->enabled = 0;
      ctx->fault   = NO_ERROR;
#ifdef HV_EN_PIN
      //clear driver enable pin
      HAL_GPIO_WritePin(HV_EN_PORT, HV_EN_PIN, GPIO_PIN_RESET);
#endif
    }

    if(PIN(brk) > 0.0) {
      HAL_GPIO_WritePin(BRK_PORT, BRK_PIN, GPIO_PIN_RESET);
    }
    else{
      HAL_GPIO_WritePin(BRK_PORT, BRK_PIN, GPIO_PIN_SET);
    }
  }

  //dac output for comperators
  DAC1->DHR12R1 = CLAMP((uint32_t)PIN(dac), 0, 4095);

  //comperator outputs for debugging
  PIN(cu) = (COMP1->CSR & COMP_CSR_COMPxOUT) > 0;
  PIN(cv) = (COMP2->CSR & COMP_CSR_COMPxOUT) > 0;
  PIN(cw) = (COMP4->CSR & COMP_CSR_COMPxOUT) > 0;
}

void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct io_ctx_t *ctx      = (struct io_ctx_t *)ctx_ptr;
  struct io_pin_ctx_t *pins = (struct io_pin_ctx_t *)pin_ptr;

  uint32_t led = (uint32_t)PIN(led);

  if(hal.hal_state != HAL_OK2) {
    led = 2;
  }

  HAL_GPIO_WritePin(LED_PORT, LED_PIN, BLINK(led) > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);

  PIN(hv_temp)  = r2temp(HV_R(ADC(ctx->hv_temp >> 16))) * 0.01 + PIN(hv_temp) * 0.99;
  PIN(mot_temp) = MOT_R(MOT_REF(ADC(ctx->mot_temp >> 16)));
}

hal_comp_t io_comp_struct = {
    .name      = "io",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct io_ctx_t),
    .pin_count = sizeof(struct io_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
