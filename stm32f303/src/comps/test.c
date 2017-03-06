#include "hal2.h"
#include "defines.h"
#include "commands.h"
#include <stdio.h>

HAL_COMP2(test);

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
   for(int i = 0; i < hal2.comp_inst_count; i++){
      printf("%s%i:\n", hal2.comp_insts[i].comp->name, hal2.comp_insts[i].instance);
      for(int j = 0; j < hal2.comp_insts[i].comp->pin_count; j++){
         volatile hal_comp_inst_t * comp = comp_inst_by_pin_inst(hal2.comp_insts[i].pin_insts[j].source->source);
         printf("-  %s <= %s%i.%s = %f\n", hal2.comp_insts[i].pins[j], comp->comp->name, comp->instance, pin_by_pin_inst(hal2.comp_insts[i].pin_insts[j].source->source), hal2.comp_insts[i].pin_insts[j].source->source->value);
      }
   }
}

void link(char * ptr){
   volatile hal_pin_inst_t * pin1 = pin_inst_by_name("test", 2, "in");
   volatile hal_pin_inst_t * pin2 = pin_inst_by_name("test", 1, "out");
   if(pin1 && pin2){
      pin1->source = pin2;
   }
   else{
      //printf("link: %s%i%s <- %s%i%s failed\n", );
   }
}
//weak static void (*nrt_func)(void * ctx_ptr, hal_pin_inst_t * pin_ptr) = 0;
static void nrt_func(float period, volatile void * ctx_ptr, volatile hal_pin_inst_t * pin_ptr){
   struct test_ctx_t * ctx = (struct test_ctx_t *)ctx_ptr;
   struct test_pin_ctx_t * pins = (struct test_pin_ctx_t *)pin_ptr;
   
   ctx->mem += 1.0;
   //PIN2(out) = PIN2(in);
   PIN2(out) = ctx->mem;
}

void run(char * ptr){
   run_nrt(0.1);
   //nrt_func(0.1, hal2.comp_insts[0].ctx, hal2.comp_insts[0].pin_insts);
}

COMMAND("load", load);
COMMAND("list", list);
COMMAND("show", show);
COMMAND("link", link);
COMMAND("run", run);



hal2_comp_t test_comp_struct = {"test", nrt_func, 0, 0, 0, 0, 0, 0, 0, sizeof(struct test_ctx_t), sizeof(struct test_pin_ctx_t) / sizeof(struct hal_pin_inst_t)};
