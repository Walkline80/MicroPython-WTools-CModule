/*
 * Copyright © 2024 Walkline Wang (https://walkline.wang)
 * Gitee: https://gitee.com/walkline/micropython-wtools-cmodule
 */
#include "colortool.h"

/*******************
 * Class ColorTool *
 *******************/

void rgb2hsv(color_rgb_t *rgb, color_hsv_t *hsv) {
    float max, min, delta = 0.0;
    float r = (float)((float)((int)rgb->r) / 255);
    float g = (float)((float)((int)rgb->g) / 255);
    float b = (float)((float)((int)rgb->b) / 255);

    max = MAX3(r, g, b);
    min = MIN3(r, g, b);
    delta = max - min;

    if (delta == 0) {
        hsv->h = 0;
    } else {
        if (r == max) {
            hsv->h = ((g - b) / delta) * 60;
        } else if (g == max) {
            hsv->h = 120 + (((b - r) / delta) * 60);
        } else if (b == max) {
            hsv->h = 240 + (((r - g) / delta) * 60);
        }

        if (hsv->h < 0) {
            hsv->h += 360;
        }
    }

    hsv->s = max == 0 ? 0 : (float)(delta / max);
    hsv->v = max;
}

void hsv2rgb(color_hsv_t *hsv, color_rgb_t *rgb) {
    int i;
    float f, a, b, c;

    float h = hsv->h;
    float s = hsv->s;
    float v = hsv->v;

    if (h >= 360) {
        h = 0;
    }

    if (s == 0) {
        rgb->r = (uint8_t)((int)(v * 255));
        rgb->g = (uint8_t)((int)(v * 255));
        rgb->b = (uint8_t)((int)(v * 255));
    } else {
        h /= 60.0;
        i = (int)floor(h);
        f = h - i;
        a = v * (1 - s);
        b = v * (1 - s * f);
        c = v * (1 - s * (1 - f));

        switch (i) {
            case 0:
                rgb->r = (uint8_t)((int)(v * 255));
                rgb->g = (uint8_t)((int)(c * 255));
                rgb->b = (uint8_t)((int)(a * 255));
                break;
            case 1:
                rgb->r = (uint8_t)((int)(b * 255));
                rgb->g = (uint8_t)((int)(v * 255));
                rgb->b = (uint8_t)((int)(a * 255));
                break;
            case 2:
                rgb->r = (uint8_t)((int)(a * 255));
                rgb->g = (uint8_t)((int)(v * 255));
                rgb->b = (uint8_t)((int)(c * 255));
                break;
            case 3:
                rgb->r = (uint8_t)((int)(a * 255));
                rgb->g = (uint8_t)((int)(b * 255));
                rgb->b = (uint8_t)((int)(v * 255));
                break;
            case 4:
                rgb->r = (uint8_t)((int)(c * 255));
                rgb->g = (uint8_t)((int)(a * 255));
                rgb->b = (uint8_t)((int)(v * 255));
                break;
            default:
                rgb->r = (uint8_t)((int)(v * 255));
                rgb->g = (uint8_t)((int)(a * 255));
                rgb->b = (uint8_t)((int)(b * 255));
                break;
        }
    }
}




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

static mp_obj_t colortool_rgb2hsv(size_t n_args, const mp_obj_t *args) {
    color_rgb_t rgb;
    color_hsv_t hsv;
    mp_obj_tuple_t *value_tuple;

    if (n_args == 1) {
        if (!mp_obj_is_type(args[0], &mp_type_tuple)) {
            mp_raise_ValueError(MP_ERROR_TEXT("value is not a tuple"));
        }

        value_tuple = MP_OBJ_TO_PTR(args[0]);

        if (value_tuple->len != 3) {
            mp_raise_ValueError(MP_ERROR_TEXT("invalid length of tuple"));
        }

        rgb.r = mp_obj_get_int(value_tuple->items[0]);
        rgb.g = mp_obj_get_int(value_tuple->items[1]);
        rgb.b = mp_obj_get_int(value_tuple->items[2]);
    } else if (n_args == 3) {
        rgb.r = mp_obj_get_int(args[0]);
        rgb.g = mp_obj_get_int(args[1]);
        rgb.b = mp_obj_get_int(args[2]);
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid params"));
    }

    rgb2hsv(&rgb, &hsv);

    value_tuple = MP_OBJ_TO_PTR(mp_obj_new_tuple(3, NULL));

    value_tuple->items[0] = mp_obj_new_float(hsv.h);
    value_tuple->items[1] = mp_obj_new_float(hsv.s);
    value_tuple->items[2] = mp_obj_new_float(hsv.v);

    return MP_OBJ_FROM_PTR(value_tuple);
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(colortool_rgb2hsv_fun_obj, 1, 3, colortool_rgb2hsv);
static MP_DEFINE_CONST_STATICMETHOD_OBJ(colortool_rgb2hsv_obj, MP_ROM_PTR(&colortool_rgb2hsv_fun_obj));

static mp_obj_t colortool_hsv2rgb(size_t n_args, const mp_obj_t *args) {
    color_rgb_t rgb;
    color_hsv_t hsv;
    mp_obj_tuple_t *value_tuple;

    if (n_args == 1) {
        if (!mp_obj_is_type(args[0], &mp_type_tuple)) {
            mp_raise_ValueError(MP_ERROR_TEXT("value is not a tuple"));
        }

        value_tuple = MP_OBJ_TO_PTR(args[0]);

        if (value_tuple->len != 3) {
            mp_raise_ValueError(MP_ERROR_TEXT("invalid length of tuple"));
        }

        hsv.h = mp_obj_get_float(value_tuple->items[0]);
        hsv.s = mp_obj_get_float(value_tuple->items[1]);
        hsv.v = mp_obj_get_float(value_tuple->items[2]);
    } else if (n_args == 3) {
        hsv.h = mp_obj_get_float(args[0]);
        hsv.s = mp_obj_get_float(args[1]);
        hsv.v = mp_obj_get_float(args[2]);
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid params"));
    }

    hsv2rgb(&hsv, &rgb);

    value_tuple = MP_OBJ_TO_PTR(mp_obj_new_tuple(3, NULL));

    value_tuple->items[0] = mp_obj_new_int(rgb.r);
    value_tuple->items[1] = mp_obj_new_int(rgb.g);
    value_tuple->items[2] = mp_obj_new_int(rgb.b);

    return MP_OBJ_FROM_PTR(value_tuple);
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(colortool_hsv2rgb_fun_obj, 1, 3, colortool_hsv2rgb);
static MP_DEFINE_CONST_STATICMETHOD_OBJ(colortool_hsv2rgb_obj, MP_ROM_PTR(&colortool_hsv2rgb_fun_obj));




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
    {MP_ROM_QSTR(MP_QSTR_rgb2hsv),  MP_ROM_PTR(&colortool_rgb2hsv_obj)},
    {MP_ROM_QSTR(MP_QSTR_hsv2rgb),  MP_ROM_PTR(&colortool_hsv2rgb_obj)},
};
static MP_DEFINE_CONST_DICT(colortool_locals_dict, colortool_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    colortool_type,
    MP_QSTR_ColorTool,
    MP_TYPE_FLAG_NONE,
    make_new, colortoolbase_make_new,
    locals_dict, &colortool_locals_dict
    );
