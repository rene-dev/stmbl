#pragma once

#include <stdint.h>

struct version_info {
    // set at link-time by add-version-info.py
    //
    uint32_t    image_crc;
    uint32_t    image_size;

    char        git_version[32];
    char        git_branch[32];
    char        build_user[16];
    char        build_host[32];
    char        build_date[16];
    char        build_time[16];

    // set at compile-time
    //
    char        product_name[32];
    int         major;
    int         minor;
    int         patch;
};

extern volatile const struct version_info version_info __attribute__((section(".version_info")));
