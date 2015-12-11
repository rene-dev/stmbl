#include "version.h"

volatile const struct version_info* bt_version_info = (void*)0x08000188;
const struct version_info version_info = {
    .product_name   = "STMBL",
    .major          = 0,
    .minor          = 9,
    .patch          = 8
};
