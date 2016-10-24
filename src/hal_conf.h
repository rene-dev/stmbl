#pragma once

#include <string.h>
#include "hal.h"
#include "hal_conf.h"

void hal_conf_save();
int hal_conf_load();
uint16_t hal_conf_init();
