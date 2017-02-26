#include "hal2.h"
#include "defines.h"
#include <stdio.h>

COMP2(test);

HAL2_PIN(in);
HAL2_PIN(out);

struct test_ctx_t{
   float mem;
};

void list(char * ptr){
   int j = 0;
   int pin_offset = 0;
   for(int i = 0; i < comp_count; i++){
      printf("%s\n", comps[i]->name);
      for(; j < pin_offset + comps[i]->pin_count; j++){
         printf("- %s\n", pins[j]);
      }
      pin_offset += comps[i]->pin_count;
   }
}

void load(char * ptr){
   load_comp(comp_by_name("test"));
}

void show(char * ptr){
   uint32_t offset = 0;
   for(int i = 0; i < comp_inst_count; i++){
      printf("%s%i:\n", comp_insts[i].comp->name, comp_insts[i].instance);
      for(int j = 0; j < comp_insts[i].comp->pin_count; j++){
         hal_comp_inst_t * comp = comp_inst_by_pin_inst(comp_insts[i].pin_insts[j].source->source);
         printf("-  %s <= %s%i.%s = %f\n", comp_insts[i].pins[j], comp->comp->name, comp->instance, pin_by_pin_inst(comp_insts[i].pin_insts[j].source->source), comp_insts[i].pin_insts[j].source->source->value);
      }
   }
}

void link(char * ptr){
   hal_pin_inst_t * pin1 = pin_inst_by_name("test", 2, "in");
   hal_pin_inst_t * pin2 = pin_inst_by_name("test", 1, "out");
   if(pin1 && pin2){
      pin1->source = pin2;
   }
   else{
      //printf("link: %s%i%s <- %s%i%s failed\n", );
   }
}

COMMAND("load", load);
COMMAND("list", list);
COMMAND("show", show);
COMMAND("link", link);

static void nrt_func(void * ctx_ptr, hal_pin_inst_t * pin_ptr){
   struct test_ctx_t * ctx = (struct test_ctx_t *)ctx_ptr;
   struct test_pin_ctx_t * pins = (struct test_pin_ctx_t *)pin_ptr;
   
   PIN2(out) = PIN2(in);
}

hal2_comp_t test_comp_struct = {"test", nrt_func, 0, 0, sizeof(struct test_ctx_t), sizeof(struct test_pin_ctx_t) / sizeof(struct hal_pin_inst_t)};
