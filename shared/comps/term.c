#include "commands.h"
#include "hal.h"
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include "usbd_cdc_if.h"
#include "angle.h"

#define TERM_NUM_WAVES 8
#define TERM_BUF_SIZE 8

HAL_COMP(term);

HAL_PINA(wave, 8);
HAL_PINA(offset, 8);
HAL_PINA(gain, 8);
HAL_PIN(send_step);
HAL_PIN(con);

struct term_ctx_t {
  float wave_buf[TERM_BUF_SIZE][TERM_NUM_WAVES];
  uint32_t send_counter;
  uint32_t write_pos;
  uint32_t read_pos;
};


static void nrt_init(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  // struct term_ctx_t * ctx = (struct sim_ctx_t *)ctx_ptr;
  struct term_pin_ctx_t *pins = (struct term_pin_ctx_t *)pin_ptr;

  PIN(send_step) = 50;
  for(int i = 0; i < TERM_NUM_WAVES; i++) {
    PINA(gain, i) = 10;
  }
}

static void rt_func(float period, void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct term_ctx_t *ctx      = (struct term_ctx_t *)ctx_ptr;
  struct term_pin_ctx_t *pins = (struct term_pin_ctx_t *)pin_ptr;

  if(ctx->send_counter++ >= PIN(send_step) - 1) {
    for(int i = 0; i < TERM_NUM_WAVES; i++) {
      ctx->wave_buf[ctx->write_pos][i] = PINA(wave, i);
    }

    ctx->write_pos++;
    ctx->write_pos %= TERM_BUF_SIZE;
    ctx->send_counter = 0;
  }
}

static void nrt_func(void *ctx_ptr, hal_pin_inst_t *pin_ptr) {
  struct term_ctx_t *ctx      = (struct term_ctx_t *)ctx_ptr;
  struct term_pin_ctx_t *pins = (struct term_pin_ctx_t *)pin_ptr;

  int tmp = 0;
  unsigned char buf[TERM_NUM_WAVES + 2];

  buf[0]                  = 255;
  buf[TERM_NUM_WAVES + 1] = 0;

  unsigned int wp = ctx->write_pos;
  unsigned int bc = 0;

  while(ctx->read_pos != wp) {
    bc++;

    for(int i = 0; i < TERM_NUM_WAVES; i++) {
      tmp        = (int)((ctx->wave_buf[ctx->read_pos][i] + PINA(offset, i)) * PINA(gain, i) + 128);
      buf[i + 1] = CLAMP(tmp, 1, 254);
    }

    ctx->read_pos++;
    ctx->read_pos %= TERM_BUF_SIZE;

    buf[TERM_NUM_WAVES + 1] = 0;

    if(cdc_is_connected() && PIN(send_step) > 0) {
      cdc_tx(buf, TERM_NUM_WAVES + 1);
    }
  }

  if(cdc_is_connected()) {
    PIN(con) = 1.0;
  } else {
    PIN(con) = 0.0;
  }

  if(cdc_is_connected()) {
    char rx_buf[64];
    if(cdc_getline(rx_buf, sizeof(rx_buf))) {
      hal_parse(rx_buf);
    }
  }
}

hal_comp_t term_comp_struct = {
    .name      = "term",
    .nrt       = nrt_func,
    .rt        = rt_func,
    .frt       = 0,
    .nrt_init  = nrt_init,
    .rt_start  = 0,
    .frt_start = 0,
    .rt_stop   = 0,
    .frt_stop  = 0,
    .ctx_size  = sizeof(struct term_ctx_t),
    .pin_count = sizeof(struct term_pin_ctx_t) / sizeof(struct hal_pin_inst_t),
};
