#include "hal.h"


static void rt_init(){

}

const comp_t comp1 __attribute__((used, section (".comp_tbl.foo"))) = {
   .name = "comp1",
   .rt_init = rt_init,
};
