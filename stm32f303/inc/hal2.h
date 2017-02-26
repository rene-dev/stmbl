#pragma once 
#include <stdint.h>
#include <stdio.h>

#define HAL2_MAX_PINS 16
#define HAL2_MAX_COMPS 8
#define HAL2_MAX_CTX 64

#define COMP2(name) 
#define HAL2_PIN(name) 
#define COMMAND(a, b) 

typedef char NAME2[32];

typedef NAME2 const pin2_t;

typedef struct hal_pin_inst_t{
   float value;
   struct hal_pin_inst_t * source;
} hal_pin_inst_t;

typedef const struct{
   NAME2 name;
   void (*nrt)(void * ctx_ptr, hal_pin_inst_t * pin_ptr);
   void (*rt)(void * ctx_ptr, hal_pin_inst_t * pin_ptr);
   void (*frt)(void * ctx_ptr, hal_pin_inst_t * pin_ptr);
   uint32_t ctx_size;
   uint32_t pin_count;
} hal2_comp_t;

typedef struct hal_comp_inst_t{
   hal2_comp_t * comp;
   uint32_t instance;
   void * ctx;
   hal_pin_inst_t * pin_insts;
   pin2_t * pins;
} hal_comp_inst_t;

#define PIN2(p) (pins->p.source->source->value)

extern struct hal_comp_inst_t comp_insts[];
extern struct hal_pin_inst_t pin_insts[];
extern uint8_t ctxs[];
extern uint32_t comp_inst_count;
extern uint32_t pin_inst_count;
extern uint32_t ctx_count;

hal2_comp_t * comp_by_name(NAME2 name);
hal_comp_inst_t * comp_inst_by_name(NAME2 name, uint32_t instance);
uint32_t pin_offset_by_comp_name(NAME2 name);
pin2_t * pin_by_name(NAME2 comp_name, NAME2 pin_name);
hal_pin_inst_t * pin_inst_by_name(NAME2 comp_name, uint32_t instance, NAME2 pin_name);
uint32_t load_comp(hal2_comp_t * comp);
pin2_t * pin_by_pin_inst(hal_pin_inst_t * p);
hal_comp_inst_t * comp_inst_by_pin_inst(hal_pin_inst_t * p);

#include "hal_tbl.h"
