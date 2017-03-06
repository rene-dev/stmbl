#pragma once 
#include <stdint.h>
#include <stdio.h>

#define HAL2_MAX_PINS 128
#define HAL2_MAX_COMPS 16
#define HAL2_MAX_CTX 4096

#define HAL_COMP2(name) 
#define HAL2_PIN(name) 
#define HAL2_PINA(name, index) 

typedef char NAME2[32];

typedef NAME2 const pin2_t;

typedef struct hal_pin_inst_t{
   float value;
   volatile struct hal_pin_inst_t * source;
} hal_pin_inst_t;

typedef const struct{
   NAME2 name;
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
} hal2_comp_t;

typedef struct hal_comp_inst_t{
   hal2_comp_t * comp;
   uint32_t instance;
   volatile void * ctx;
   volatile hal_pin_inst_t * pin_insts;
   pin2_t * pins;
} hal_comp_inst_t;

#define PIN2(p) (pins->p.source->source->value)
#define PINA2(p, i) (pins->p[i].source->source->value)

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
     HAL2_OK
   } hal_state;
   
   volatile struct hal_comp_inst_t * rt_comps[HAL2_MAX_COMPS];
   volatile struct hal_comp_inst_t * frt_comps[HAL2_MAX_COMPS];
   volatile struct hal_comp_inst_t * nrt_comps[HAL2_MAX_COMPS];
   
   volatile int32_t active_rt_func;
   volatile int32_t active_frt_func;
   volatile int32_t active_nrt_func;
   
   volatile struct hal_comp_inst_t comp_insts[HAL2_MAX_COMPS];
   volatile struct hal_pin_inst_t pin_insts[HAL2_MAX_PINS];
   volatile uint8_t ctxs[HAL2_MAX_CTX]; // create runtime ctx print in python based on COMP_ctx_t
   uint32_t comp_inst_count;
   uint32_t pin_inst_count;
   uint32_t ctx_count;
} hal2_t;

extern hal2_t hal2;

hal2_comp_t * comp_by_name(NAME2 name);
volatile hal_comp_inst_t * comp_inst_by_name(NAME2 name, uint32_t instance);
uint32_t pin_offset_by_comp_name(NAME2 name);
pin2_t * pin_by_name(NAME2 comp_name, NAME2 pin_name);
volatile hal_pin_inst_t * pin_inst_by_name(NAME2 comp_name, uint32_t instance, NAME2 pin_name);
uint32_t load_comp(hal2_comp_t * comp);
pin2_t * pin_by_pin_inst(volatile hal_pin_inst_t * p);
volatile hal_comp_inst_t * comp_inst_by_pin_inst(volatile hal_pin_inst_t * p);

void start();
void start_rt();
void start_frt();
void stop();
void stop_rt();
void stop_frt();
void run_rt(float period);
void run_frt(float period);
void run_nrt(float period);
uint32_t hal_parse(char * cmd);

#include "hal_tbl.h"
