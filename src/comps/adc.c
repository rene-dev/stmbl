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

HAL_COMP(adc);

HAL_PIN(sin);   //sin output
HAL_PIN(cos);   //cos output
HAL_PIN(sin3);  //sin output, last quater only
HAL_PIN(cos3);  //cos output, last quater only
HAL_PIN(quad);  //quadrant of sin/cos

HAL_PIN(sin1);  //sin output
HAL_PIN(cos1);  //cos output

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

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
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

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct adc_ctx_t *ctx      = (struct adc_ctx_t *)ctx_ptr;
  struct adc_pin_ctx_t *pins = (struct adc_pin_ctx_t *)pin_ptr;

  float si0[ADC_GROUPS];
  float co0[ADC_GROUPS];
  uint32_t sii0, coi0;

#ifdef FB1
  float co1[ADC_GROUPS];
  float si1[ADC_GROUPS];
  uint32_t sii1, coi1;
#endif

  float s_o = PIN(sin_offset);
  float c_o = PIN(cos_offset);
  float s_g = PIN(sin_gain);
  float c_g = PIN(cos_gain);
  float s;
  float c;

  volatile uint32_t *ADC_DMA_Buffer;

  // if(DMA_GetCurrentMemoryTarget(DMA2_Stream0) == 0){
  //    ADC_DMA_Buffer = ADC_DMA_Buffer1;
  // }
  // else{
  ADC_DMA_Buffer = ADC_DMA_Buffer0;
  // }
  int flip;
  int n = PIN(res_mode);
  for(int i = 0; i < ADC_GROUPS; i++) {
    if(n > 0 && i % (2 * n) >= n) {
      flip = -1;
    } else {
      flip = 1;
    }
    si0[i] = 0;
    co0[i] = 0;
#ifdef FB1
    si1[i] = 0;
    co1[i] = 0;
#endif
    //ADC dual mode puts both channels in one word, right aligned.
    for(int j = 0; j < ADC_OVER_FB0; j++) {
      sii0 += ADC_DMA_Buffer[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] & 0x0000ffff;
      coi0 += ADC_DMA_Buffer[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] >> 16;
    }
#ifdef FB1
    for(int j = ADC_OVER_FB0; j < ADC_OVER_FB0 + ADC_OVER_FB1; j++) {
      sii1 += ADC_DMA_Buffer[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] & 0x0000ffff;
      coi1 += ADC_DMA_Buffer[i * (ADC_OVER_FB0 + ADC_OVER_FB1) + j] >> 16;
    }
#endif

    si0[i] = s_g * V_DIFF(si0[i], ADC_OVER_FB0) + s_o;
    co0[i] = c_g * V_DIFF(co0[i], ADC_OVER_FB0) + c_o;
#ifdef FB1
    si1[i] = s_g * V_DIFF(si1[i], ADC_OVER_FB1) + s_o;
    co1[i] = c_g * V_DIFF(co1[i], ADC_OVER_FB1) + c_o;
#endif
  }

  //copy dma buffer for plotting TODO: use dual mode, for zero copy
  if(ctx->send == 0) {
    memcpy((void *)(ctx->txbuf_raw), (void *)ADC_DMA_Buffer, ADC_SAMPLES_IN_RT * 4);
    ctx->send = 1;
  }

  PIN(sin3) = si0[ADC_GROUPS - 1];
  PIN(cos3) = co0[ADC_GROUPS - 1];
#ifdef FB1
  PIN(sin1) = si1[ADC_GROUPS - 1];
  PIN(cos1) = co1[ADC_GROUPS - 1];
#endif
  // if(PIN(res_en) > 0.0) {
  //   s = (si0[3] - si0[2] + si0[1] - si0[0]) / 4.0;
  //   c = (co0[3] - co0[2] + co0[1] - co0[0]) / 4.0;
  // } else {
  //   s = (si0[3] + si0[2] + si0[1] + si0[0]) / 4.0;
  //   c = (co0[3] + co0[2] + co0[1] + co0[0]) / 4.0;
  // }

  if(si0[3] >= 0) {
    if(co0[3] > 0)
      PIN(quad) = 1;
    else
      PIN(quad) = 2;
  } else {
    if(co0[3] > 0)
      PIN(quad) = 4;
    else
      PIN(quad) = 3;
  }
  PIN(sin) = s;
  PIN(cos) = c;
}


static void nrt_func(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
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

    buf[0] = 255;
    for(int k = 0; k < ADC_SAMPLES_IN_RT; k++) {  //each sample
      for(int i = 0; i < TERM_NUM_WAVES; i++) {   //each wave
        tmp        = (ctx->txbuf[i][k] + PINA(offset, i)) * PINA(gain, i) + 128;
        buf[i + 1] = CLAMP(tmp, 1, 254);
      }
      buf[8 + 1] = 0;

      if(USB_CDC_is_connected()) {
        USB_VCP_send_string(buf);
      }
    }


    buf[0] = 0xfe;  //trigger servoterm
    buf[1] = 0x00;
    if(USB_CDC_is_connected()) {
      USB_VCP_send_string(buf);
    }

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
