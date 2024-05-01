/*
 * Copyright © 2024 Walkline Wang (https://walkline.wang)
 * Gitee: https://gitee.com/walkline/micropython-wtools-cmodule
 */
#ifndef ARRAYTOOL_H
#define ARRAYTOOL_H

#include "modwtools.h"

#define BITREAD(value, bit) (((value) >> (bit)) & 0x01)

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction_t;

typedef struct _arraytool_obj_t {
    mp_obj_base_t base;
    uint8_t **array;
    uint8_t cols;
    uint8_t rows;
} arraytool_obj_t;

#endif
