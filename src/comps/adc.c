#include "commands.h"
#include "hal.h"
#include "math.h"
#include "defines.h"
#include "angle.h"
#include "setup.h"
#include "usbd_cdc_if.h"

#define INPUT_REF (OP_REF * OP_R_OUT_LOW / (OP_R_OUT_HIGH + OP_R_OUT_LOW))
#define INPUT_GAIN (OP_R_FEEDBACK / OP_R_INPUT * OP_R_OUT_LOW / (OP_R_OUT_HIGH + OP_R_OUT_LOW))
#define V_DIFF(ADC) ((((float)ADC) / (float)ADC_ANZ / ADC_RES * ADC_REF - INPUT_REF) / INPUT_GAIN)
#define V_DIFF2(ADC) (((ADC) / ADC_RES * ADC_REF - INPUT_REF) / INPUT_GAIN)

#define TERM_NUM_WAVES 8

HAL_COMP(adc);

HAL_PIN(sin);  //sin output
HAL_PIN(cos);  //cos output
HAL_PIN(sin3);  //sin output, last quater only
HAL_PIN(cos3);  //cos output, last quater only
HAL_PIN(quad);  //quadrant of sin/cos

HAL_PIN(res_en);  //flip polarity for resolvers

HAL_PIN(sin_gain);
HAL_PIN(cos_gain);

HAL_PIN(sin_offset);
HAL_PIN(cos_offset);

HAL_PIN(send_step);  //15.0;

HAL_PINA(offset, 8);
HAL_PINA(gain, 8);

struct adc_ctx_t {
  volatile float txbuf[8][PID_WAVES * ADC_ANZ];
  uint32_t txpos;
  uint32_t send_counter;  //send_step counter
  uint32_t send;  //send buffer state
};

static void nrt_init(volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct adc_ctx_t *ctx      = (struct adc_ctx_t *)ctx_ptr;
  struct adc_pin_ctx_t *pins = (struct adc_pin_ctx_t *)pin_ptr;
  PINA(gain, 0)              = 200;
  PINA(gain, 1)              = 200;
  PIN(sin_gain)              = 1.0;
  PIN(cos_gain)              = 1.0;
  ctx->txpos                 = 0;
  ctx->send_counter          = 0;  //send_step counter
  ctx->send                  = 1;  //send buffer state
}

static void rt_func(float period, volatile void *ctx_ptr, volatile hal_pin_inst_t *pin_ptr) {
  struct adc_ctx_t *ctx      = (struct adc_ctx_t *)ctx_ptr;
  struct adc_pin_ctx_t *pins = (struct adc_pin_ctx_t *)pin_ptr;

  float si[PID_WAVES];
  float co[PID_WAVES];
  uint32_t sii;
  uint32_t coi;

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
  for(int i = 0; i < PID_WAVES; i++) {
    sii = 0;
    coi = 0;
    for(int j = 0; j < ADC_ANZ; j++) {
      //ADC dual mode puts both channels in one word, right aligned.
      sii += ADC_DMA_Buffer[i * ADC_ANZ + j] & 0x0000ffff;
      coi += ADC_DMA_Buffer[i * ADC_ANZ + j] >> 16;
      if(ctx->send == 0) {  // TODO: move V_DIFF2 to nrt, this is too slow
        ctx->txbuf[0][ctx->txpos] = (((i == 0 || i == 2) && (PIN(res_en) > 0.0)) ? -1.0 : 1.0) * V_DIFF2(ADC_DMA_Buffer[i * ADC_ANZ + j] & 0x0000ffff);
        ctx->txbuf[1][ctx->txpos] = (((i == 0 || i == 2) && (PIN(res_en) > 0.0)) ? -1.0 : 1.0) * V_DIFF2(ADC_DMA_Buffer[i * ADC_ANZ + j] >> 16);
        ctx->txpos++;
      }
    }
    si[i] = s_g * V_DIFF(sii) + s_o;
    co[i] = c_g * V_DIFF(coi) + c_o;
  }
  if(ctx->send == 0) {
    ctx->send  = 1;
    ctx->txpos = 0;
  }

  PIN(sin3) = si[3];
  PIN(cos3) = co[3];

  if(PIN(res_en) > 0.0) {
    s = (si[3] - si[2] + si[1] - si[0]) / 4.0;
    c = (co[3] - co[2] + co[1] - co[0]) / 4.0;
  } else {
    s = (si[3] + si[2] + si[1] + si[0]) / 4.0;
    c = (co[3] + co[2] + co[1] + co[0]) / 4.0;
  }

  if(si[3] >= 0) {
    if(co[3] > 0)
      PIN(quad) = 1;
    else
      PIN(quad) = 2;
  } else {
    if(co[3] > 0)
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
  if(ctx->send_counter++ >= PIN(send_step) - 1 && PIN(send_step) > 0) {
    int tmp = 0;
    uint8_t buf[TERM_NUM_WAVES + 3];

    buf[0] = 255;
    for(int k = 0; k < PID_WAVES * ADC_ANZ; k++) {
      for(int i = 0; i < TERM_NUM_WAVES; i++) {
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
    ctx->send_counter = 0;
    ctx->send         = 0;
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
