#pragma once 
#include <stdint.h>
#include <stdio.h>

#define HAL_CALC_TIME
#define HAL_COMP_CALC_TIME


#define HAL_MAX_PINS 512
#define HAL_MAX_COMPS 16
#define HAL_MAX_CTX 4096

#define HAL_COMP(name) 
#define HAL_PIN(name) 
#define HAL_PINA(name, index) 

extern uint32_t hal_get_systick_value();
extern uint32_t hal_get_systick_reload();
extern uint32_t hal_get_systick_freq();

typedef char NAME[32];

typedef NAME const pin_t;

typedef struct hal_pin_inst_t{
   float value;
   volatile struct hal_pin_inst_t * source;
} hal_pin_inst_t;

typedef const struct{
   NAME name;
   void (*nrt)(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr);
   void (*rt)(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr);
   void (*frt)(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr);
   
   void (*nrt_init)(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr);
   void (*rt_start)(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr);
   void (*frt_start)(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr);
   void (*rt_stop)(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr);
   void (*frt_stop)(volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr);

   uint32_t ctx_size;
   uint32_t pin_count;
} hal_comp_t;

typedef struct hal_comp_inst_t{
   hal_comp_t * comp;
   uint32_t instance;
   volatile void * ctx;
   volatile hal_pin_inst_t * pin_insts;
   pin_t * pins;
} hal_comp_inst_t;

#define PIN(p) (pins->p.source->source->value)
#define PINA(p, i) (pins->p[i].source->source->value)

typedef volatile struct{
   volatile enum{
     RT_CALC,
     RT_SLEEP,
     RT_STOP
   } rt_state;

   volatile enum{
     FRT_CALC,
     FRT_SLEEP,
     FRT_STOP
   } frt_state;

   volatile enum {
     FRT_TOO_LONG,
     RT_TOO_LONG,
     MISC_ERROR,
     MEM_ERROR,
     CONFIG_LOAD_ERROR,
     CONFIG_ERROR,
     NAN_ERROR,
     HAL_OK2
   } hal_state;
   
   volatile struct hal_comp_inst_t * rt_comps[HAL_MAX_COMPS];
   volatile struct hal_comp_inst_t * frt_comps[HAL_MAX_COMPS];
   
   volatile int32_t active_rt_func;
   volatile int32_t active_frt_func;
   volatile int32_t active_nrt_func;
   
   volatile struct hal_comp_inst_t comp_insts[HAL_MAX_COMPS];
   volatile struct hal_pin_inst_t pin_insts[HAL_MAX_PINS];
   volatile uint8_t ctxs[HAL_MAX_CTX]; // create runtime ctx print in python based on COMP_ctx_t
   uint32_t comp_inst_count;
   uint32_t rt_comp_count;
   uint32_t frt_comp_count;
   uint32_t pin_inst_count;
   uint32_t ctx_count;
   
   volatile float rt_calc_time;
   volatile float max_rt_calc_time;
   volatile float frt_calc_time;
   volatile float max_frt_calc_time;
   volatile float nrt_calc_time;
   volatile float max_nrt_calc_time;
   volatile float rt_period;
   volatile float frt_period;
   volatile float nrt_period;
} hal_t;

extern hal_t hal;

hal_comp_t * comp_by_name(NAME name);
volatile hal_comp_inst_t * comp_inst_by_name(NAME name, uint32_t instance);
uint32_t pin_offset_by_comp_name(NAME name);
pin_t * pin_by_name(NAME comp_name, NAME pin_name);
volatile hal_pin_inst_t * pin_inst_by_name(NAME comp_name, uint32_t instance, NAME pin_name);
uint32_t load_comp(hal_comp_t * comp);
pin_t * pin_by_pin_inst(volatile hal_pin_inst_t * p);
volatile hal_comp_inst_t * comp_inst_by_pin_inst(volatile hal_pin_inst_t * p);
void hal_print_pin(volatile hal_pin_inst_t * p);

void hal_init();
void hal_init_nrt();
void hal_start();
void hal_stop();
void hal_run_rt();
void hal_run_frt();
void hal_run_nrt();
uint32_t hal_parse(char * cmd);

#include "hal_tbl.h"
