#pragma once

#include <string.h>
#include "hal.h"

void hal_term_print_pin(hal_pin_t* pin);
void hal_term_print_info();
void hal_term_list();
void hal_term_getconf();
void hal_term_print_state();
extern void Wait(uint32_t ms);//TODO: find a way of removing wait form hal_term
