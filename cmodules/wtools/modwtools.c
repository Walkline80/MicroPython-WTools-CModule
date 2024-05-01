/*
 * Copyright © 2024 Walkline Wang (https://walkline.wang)
 * Gitee: https://gitee.com/walkline/micropython-wtools-cmodule
 */
#include "modwtools.h"

/*****************
 * Module wtools *
 *****************/

static const mp_rom_map_elem_t mp_module_wtools_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__),  MP_ROM_QSTR(MP_QSTR_wtools)},
    {MP_ROM_QSTR(MP_QSTR_ColorTool), MP_ROM_PTR(&colortool_type)},
    {MP_ROM_QSTR(MP_QSTR_ArrayTool), MP_ROM_PTR(&arraytool_type)},
};
static MP_DEFINE_CONST_DICT(mp_module_wtools_globals, mp_module_wtools_globals_table);

const mp_obj_module_t mp_module_wtools = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&mp_module_wtools_globals,
};

MP_REGISTER_MODULE(MP_QSTR_wtools, mp_module_wtools);
