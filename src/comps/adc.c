#include "adc_comp.h"
#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "setup.h"
#include "usbd_cdc_if.h"

#define INPUT_REF (OP_REF * OP_R_OUT_LOW / (OP_R_OUT_HIGH + OP_R_OUT_LOW))
#define INPUT_GAIN (OP_R_FEEDBACK / OP_R_INPUT * OP_R_OUT_LOW / (OP_R_OUT_HIGH + OP_R_OUT_LOW))
#define V_DIFF(ADC, OVER) ((((float)(ADC)) / (float)(OVER) / ADC_RES * ADC_REF - INPUT_REF) / INPUT_GAIN)

#define TERM_NUM_WAVES 8

extern volatile uint32_t ADC_DMA_Buffer0[ADC_SAMPLES_IN_RT];  //240
extern volatile uint32_t ADC_DMA_Buffer1[ADC_SAMPLES_IN_RT];

HAL_COMP(adc);

HAL_PIN(sin0);   //sin output
HAL_PIN(cos0);   //cos output
HAL_PIN(sin0l);  //sin output, last group only
HAL_PIN(cos0l);  //cos output, last group only
HAL_PIN(quad);   //quadrant of sin/cos
HAL_PIN(amp0);   

HAL_PIN(sin1);   //sin output
HAL_PIN(cos1);   //cos output
HAL_PIN(sin1l);  //sin output, last group only
HAL_PIN(cos1l);  //cos output, last group only
HAL_PIN(amp1);   

HAL_PIN(res_mode);  //polarity flip mode for resolvers

HAL_PIN(sin_gain);
HAL_PIN(cos_gain);

HAL_PIN(sin_offset);
HAL_PIN(cos_offset);

HAL_PIN(send_step);  //15.0;

HAL_PINA(offset, 8);
HAL_PINA(gain, 8);

struct adc_ctx_t {
  volatile float txbuf[8][ADC_SAMPLES_IN_RT];
  volatile uint32_t txbuf_raw[ADC_SAMPLES_IN_RT];
  uint32_t txpos;
  uint32_t send_counter;   //send_step counter
  volatile uint32_t send;  //send buffer state 0=filling, 1=sending
};

static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct adc_ctx_t *ctx      = (struct adc_ctx_t *)ctx_ptr;
  struct adc_pin_ctx_t *pins = (struct adc_pin_ctx_t *)pin_ptr;
  PINA(gain, 0)              = 150;
  PINA(gain, 1)              = 150;
  PINA(gain, 2)              = 150;
  PINA(gain, 3)              = 150;
  PINA(gain, 4)              = 80;
  PIN(sin_gain)              = 1.0;
  PIN(cos_gain)              = 1.0;
  ctx->txpos                 = 0;
  ctx->send_counter          = 0;
  ctx->send                  = 0;
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct adc_ctx_t *ctx      = (struct adc_ctx_t *)ctx_ptr;
  struct adc_pin_ctx_t *pins = (struct adc_pin_ctx_t *)pin_ptr;

  //scaled values for each group
  float si0[ADC_GROUPS];
  float co0[ADC_GROUPS];
  //integral per group
  uint32_t sii0;
  uint32_t coi0;
  //scaled, all groups
  float sin0all = 0.0;
  float cos0all = 0.0;
#ifdef FB1
  float co1[ADC_GROUPS];
  float si1[ADC_GROUPS];
  uint32_t sii1;
  uint32_t coi1;
  //scaled, all groups
  float sin1all = 0.0;
  float cos1all = 0.0;
#endif

  float s_o = PIN(sin_offset);
  float c_o = PIN(cos_offset);
  float s_g = PIN(sin_gain);
  float c_g = PIN(cos_gain);

  volatile uint32_t *ADC_DMA_Buffer;

  if(DMA_GetCurrentMemoryTarget(DMA2_Stream0)) {
    ADC_DMA_Buffer = ADC_DMA_Buffer0;
  } else {
    ADC_DMA_Buffer = ADC_DMA_Buffer1;
  }
  int flip;
  int n = PIN(res_mode);


  for(int i = 0; i < ADC_GROUPS; i++) {  //each adc sampling group
    if(n > 0 && i % (2 * n) >= n) {
      flip = -1;
    } else {
      flip = 1;
    }
    sii0 = 0;
    coi0 = 0;
    for(int j = 0; j < ADC_OVER_FB0; j++) {  //each adc sample of fb0
      sii0 += ADC_DMA_Buffer[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] & 0x0000ffff;
      coi0 += ADC_DMA_Buffer[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] >> 16;
    }
    si0[i] = flip * s_g * V_DIFF(sii0, ADC_OVER_FB0) + s_o;
    co0[i] = flip * c_g * V_DIFF(coi0, ADC_OVER_FB0) + c_o;
    sin0all += si0[i];
    cos0all += co0[i];
#ifdef FB1
    sii1 = 0;
    coi1 = 0;
    for(int j = ADC_OVER_FB0; j < ADC_OVER_FB0 + ADC_OVER_FB1; j++) {  //each adc sample of fb1
      sii1 += ADC_DMA_Buffer[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] & 0x0000ffff;
      coi1 += ADC_DMA_Buffer[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] >> 16;
    }
    si1[i] = s_g * V_DIFF(sii1, ADC_OVER_FB1) + s_o;
    co1[i] = c_g * V_DIFF(coi1, ADC_OVER_FB1) + c_o;
    sin1all += si1[i];
    cos1all += co1[i];
#endif
  }
  if(ctx->send == 0) {
    memcpy((void *)(ctx->txbuf_raw), (void *)ADC_DMA_Buffer, ADC_SAMPLES_IN_RT * 4);
    ctx->send = 1;
  }

  float s = V_DIFF(ADC_DMA_Buffer[0] & 0x0000ffff, 1);
  float c = V_DIFF(ADC_DMA_Buffer[0] >> 16, 1);

  PIN(sin0l) = si0[ADC_GROUPS - 1];
  PIN(cos0l) = co0[ADC_GROUPS - 1];
  PIN(sin0)  = sin0all / (float)ADC_GROUPS;
  PIN(cos0)  = cos0all / (float)ADC_GROUPS;
  PIN(amp0) = PIN(amp0) * 0.9 + sqrtf(s * s + c * c) * 0.1;
#ifdef FB1
  s = V_DIFF(ADC_DMA_Buffer[ADC_OVER_FB0] & 0x0000ffff, 1);
  c = V_DIFF(ADC_DMA_Buffer[ADC_OVER_FB0] >> 16, 1);
  PIN(sin1l) = si1[ADC_GROUPS - 1];
  PIN(cos1l) = co1[ADC_GROUPS - 1];
  PIN(sin1)  = sin1all / (float)ADC_GROUPS;
  PIN(cos1)  = cos1all / (float)ADC_GROUPS;
  PIN(amp1) = PIN(amp1) * 0.9 + sqrtf(s * s + c * c) * 0.1;
#endif

  // if(PIN(res_en) > 0.0) {
  //   s = (si0[3] - si0[2] + si0[1] - si0[0]) / 4.0;
  //   c = (co0[3] - co0[2] + co0[1] - co0[0]) / 4.0;
  // } else {
  //   s = (si0[3] + si0[2] + si0[1] + si0[0]) / 4.0;
  //   c = (co0[3] + co0[2] + co0[1] + co0[0]) / 4.0;
  // }

  //calculate quadrant for sin/cos interpolation
  if(si0[ADC_GROUPS - 1] >= 0) {
    if(co0[ADC_GROUPS - 1] > 0)
      PIN(quad) = 1;
    else
      PIN(quad) = 2;
  } else {
    if(co0[ADC_GROUPS - 1] > 0)
      PIN(quad) = 4;
    else
      PIN(quad) = 3;
  }
}


static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct adc_ctx_t *ctx      = (struct adc_ctx_t *)ctx_ptr;
  struct adc_pin_ctx_t *pins = (struct adc_pin_ctx_t *)pin_ptr;

  int tmp = 0;
  uint8_t buf[TERM_NUM_WAVES + 3];

  int n = PIN(res_mode);  //n gruppen pro halbwelle 1-12
  int flip;
  if(ctx->send == 1 && ctx->send_counter++ >= PIN(send_step) - 1 && PIN(send_step) > 0) {
    ctx->send_counter = 0;
    for(int i = 0; i < ADC_GROUPS; i++) {  //each adc sampling group
      if(n > 0 && i % (2 * n) >= n) {
        flip = -1;
      } else {
        flip = 1;
      }
      for(int j = 0; j < ADC_OVER_FB0; j++) {  //each adc sample of fb0
        ctx->txbuf[0][ctx->txpos] = flip * V_DIFF(ctx->txbuf_raw[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] & 0x0000ffff, 1);
        ctx->txbuf[1][ctx->txpos] = flip * V_DIFF(ctx->txbuf_raw[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] >> 16, 1);
        ctx->txbuf[2][ctx->txpos] = 0;
        ctx->txbuf[3][ctx->txpos] = 0;
        ctx->txbuf[4][ctx->txpos] = flip;
        ctx->txpos++;
      }
#ifdef FB1
      for(int j = ADC_OVER_FB0; j < ADC_OVER_FB0 + ADC_OVER_FB1; j++) {  //each adc sample of fb1
        ctx->txbuf[0][ctx->txpos] = 0;
        ctx->txbuf[1][ctx->txpos] = 0;
        ctx->txbuf[2][ctx->txpos] = V_DIFF(ctx->txbuf_raw[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] & 0x0000ffff, 1);
        ctx->txbuf[3][ctx->txpos] = V_DIFF(ctx->txbuf_raw[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] >> 16, 1);
        ctx->txbuf[4][ctx->txpos] = flip;
        ctx->txpos++;
      }
#endif
    }

    ctx->txpos = 0;
    buf[0]     = 255;                             //start of waves
    for(int k = 0; k < ADC_SAMPLES_IN_RT; k++) {  //each sample
      for(int i = 0; i < TERM_NUM_WAVES; i++) {   //each wave
        tmp        = (ctx->txbuf[i][k] + PINA(offset, i)) * PINA(gain, i) + 128;
        buf[i + 1] = CLAMP(tmp, 1, 254);
      }
      cdc_tx(buf, 9);
    }
    buf[0] = 0xfe;  //trigger servoterm
    cdc_tx(buf, 1);
    ctx->send = 0;
  }
}

hal_comp_t adc_comp_struct = {
    .name      = "adc",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct adc_ctx_t),
    .pin_count = sizeof(struct adc_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
