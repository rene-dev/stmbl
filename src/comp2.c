#include "hal.h"

//COMP
#define comp_name "comp2" 

HAL_PIN_NEW(pin1, 0.0);

typedef struct {
   int foo;
   char bla[100];
}self_t;

static void rt_init(self_t* self, struct hal_pin* pins){
   //PIN(pin1) = 0.0;
   pins[pin1].source->source->value = 0.0;
   
   self->foo = 0;
}

static const char name[] = "df";

const comp_t comp2 __attribute__((used, section (".comp_tbl."comp_name))) = {
   .name = comp_name,
   .rt_init = rt_init,
   .self_size = sizeof(self_t),
};

