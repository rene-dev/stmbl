#include "version.h"

volatile const struct version_info* bt_version_info = (void*)0x0800010c;
volatile const struct version_info version_info = {
    .product_name   = "STMBL-HV",
    .major          = 0,
    .minor          = 9,
    .patch          = 0
};
