/*
 * Copyright © 2024 Walkline Wang (https://walkline.wang)
 * Gitee: https://gitee.com/walkline/micropython-wtools-cmodule
 */
#include "modwtools.h"


/*******************
 * Class ColorTool *
 *******************/

// An 8-bit gamma-correction function
// ColorTool.gamma8(value)
static mp_obj_t colortool_gamma8(mp_obj_t value_obj) {
    mp_int_t value = mp_obj_get_int(value_obj);

    if (value < 0 || value > 255) {
        mp_raise_ValueError(MP_ERROR_TEXT("input value out of range. [0~255]"));
    }

    return mp_obj_new_int(GammaTable[value]);
}
static MP_DEFINE_CONST_FUN_OBJ_1(colortool_gamma8_fun_obj, colortool_gamma8);
static MP_DEFINE_CONST_STATICMETHOD_OBJ(colortool_gamma8_obj, MP_ROM_PTR(&colortool_gamma8_fun_obj));


static mp_obj_t colortoolbase_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    (void)type;
    (void)n_args;
    (void)n_kw;
    (void)args;

    return MP_OBJ_FROM_PTR(&colortool_type);
}

static const mp_rom_map_elem_t colortool_locals_dict_table[] = {
    // Static methods
    {MP_ROM_QSTR(MP_QSTR_gamma8),   MP_ROM_PTR(&colortool_gamma8_obj)},
};
static MP_DEFINE_CONST_DICT(colortool_locals_dict, colortool_locals_dict_table);

static MP_DEFINE_CONST_OBJ_TYPE(
    colortool_type,
    MP_QSTR_ColorTool,
    MP_TYPE_FLAG_NONE,
    make_new, colortoolbase_make_new,
    locals_dict, &colortool_locals_dict
    );


/*****************
 * Module wtools *
 *****************/

static const mp_rom_map_elem_t mp_module_wtools_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),  MP_ROM_QSTR(MP_QSTR_wtools) },
    { MP_ROM_QSTR(MP_QSTR_ColorTool), MP_ROM_PTR(&colortool_type) },
};
static MP_DEFINE_CONST_DICT(mp_module_wtools_globals, mp_module_wtools_globals_table);

const mp_obj_module_t mp_module_wtools = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_wtools_globals,
};


MP_REGISTER_MODULE(MP_QSTR_wtools, mp_module_wtools);
