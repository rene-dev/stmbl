#include "version.h"
#include "version_number.h"

volatile const version_info_t version_info __attribute__((section(".version_info"))) = {
    .product_name = "STMBL",
    .major        = VERSION_MAJOR,
    .minor        = VERSION_MINOR,
    .patch        = VERSION_PATCH};
