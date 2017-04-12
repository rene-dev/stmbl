#include "hal.h"
#include "defines.h"
#include "commands.h"
#include <stdio.h>

HAL_COMP(test);

HAL_PIN(in);
HAL_PIN(out);

struct test_ctx_t{
   float mem;
};

//weak static void (*nrt_func)(void * ctx_ptr, hal_pin_inst_t * pin_ptr) = 0;
static void nrt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct test_ctx_t * ctx = (struct test_ctx_t *)ctx_ptr;
   struct test_pin_ctx_t * pins = (struct test_pin_ctx_t *)pin_ptr;
   
   ctx->mem += 1.0;
   //PIN2(out) = PIN2(in);
   PIN(out) = ctx->mem;
}

hal_comp_t test_comp_struct = {"test", nrt_func, 0, 0, 0, 0, 0, 0, 0, sizeof(struct test_ctx_t), sizeof(struct test_pin_ctx_t) / sizeof(struct hal_pin_inst_t)};
