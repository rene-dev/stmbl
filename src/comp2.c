#include "hal.h"

static void rt_init(){

}

const comp_t comp2 __attribute__((used, section (".comp_tbl.foo"))) = {
   .name = "comp2",
   .rt_init = rt_init,
};
