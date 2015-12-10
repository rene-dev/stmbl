#include "hal.h"

#define COMP_NAME comp1

HAL_PIN_NEW(pin1, 0.0);
HAL_PIN_NEW(pin2, 0.0);
HAL_PIN_NEW(pin3, 0.0);
HAL_PIN_NEW(pin4, 0.0);
HAL_PIN_NEW(pin5, 0.0);

typedef struct {
   int foo;
   char bla[100];
}self_t;

static void rt_init(self_t* self, struct hal_pin* pins){
   //PIN(pin1) = 0.0;
   pins[pin1].source->source->value = 0.0;
   
   self->foo = 0;
}

COMP_NEW {
   .name = TOSTRING(COMP_NAME),
   .rt_init = (void (*)(void*,struct hal_pin*))rt_init,
   .self_size = sizeof(self_t),
};

