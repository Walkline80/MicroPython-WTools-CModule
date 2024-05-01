/*
 * Copyright © 2024 Walkline Wang (https://walkline.wang)
 * Gitee: https://gitee.com/walkline/micropython-wtools-cmodule
 */
#include <string.h>
#include "arraytool.h"

/*******************
 * Class ArrayTool *
 *******************/

static mp_obj_t arraytool_init(mp_obj_t self_in, mp_obj_t value_obj) {
    arraytool_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t value = mp_obj_get_int(value_obj);

    for (int row = 0; row < self->rows; row++) {
        for (int col = 0; col < self->cols; col++) {
            self->array[row][col] = BITREAD(value, (self->rows * self->cols - 1) - (col * self->rows + row));
        }
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(arraytool_init_obj, arraytool_init);

static mp_obj_t arraytool_print(mp_obj_t self_in) {
    arraytool_obj_t *self = MP_OBJ_TO_PTR(self_in);

    for (int row = 0; row < self->rows; row++) {
        for (int col = 0; col < self->cols; col++) {
            if (col == 0) {
                mp_printf(&mp_plat_print, "[%d, ", self->array[row][col]);
            } else if (col == self->cols - 1) {
                mp_printf(&mp_plat_print, "%d]", self->array[row][col]);
            } else {
                mp_printf(&mp_plat_print, "%d, ", self->array[row][col]);
            }
        }
        mp_printf(&mp_plat_print, "\n");
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(arraytool_print_obj, arraytool_print);

static mp_obj_t arraytool_shift(mp_obj_t self_in, mp_obj_t direction_obj) {
    arraytool_obj_t *self = MP_OBJ_TO_PTR(self_in);
    direction_t direction = mp_obj_get_int(direction_obj);

    if (direction < UP || direction > RIGHT) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid direction"));
    }

    switch (direction) {
        case UP:
            for (int row = 0; row < self->rows - 1; row++) {
                memcpy(self->array[row], self->array[row + 1],
                    sizeof(uint8_t) * self->cols);
            }
            memset(self->array[self->rows - 1], 0, sizeof(uint8_t) * self->cols);
            break;
        case DOWN:
            for (int row = self->rows - 1; row > 0; row--) {
                memcpy(self->array[row], self->array[row - 1],
                    sizeof(uint8_t) * self->cols);
            }
            memset(self->array[0], 0, sizeof(uint8_t) * self->cols);
            break;
        case LEFT:
            for (int row = 0; row < self->rows; row++) {
                for (int col = 0; col < self->cols - 1; col++) {
                    self->array[row][col] = self->array[row][col + 1];
                }
                self->array[row][self->cols - 1] = 0;
            }
            break;
        case RIGHT:
            for (int row = 0; row < self->rows; row++) {
                for (int col = self->cols - 1; col > 0; col--) {
                    self->array[row][col] = self->array[row][col - 1];
                }
                self->array[row][0] = 0;
            }
            break;
        default:
            break;
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(arraytool_shift_obj, arraytool_shift);

static mp_obj_t arraytool_replace_col(mp_obj_t self_in, mp_obj_t index_obj, mp_obj_t value_obj) {
    arraytool_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t index = mp_obj_get_int(index_obj);

    if (index < 0 || index >= self->cols) {
        mp_raise_ValueError(MP_ERROR_TEXT("index out of range"));
    }

    if (!mp_obj_is_type(value_obj, &mp_type_list)) {
        mp_raise_ValueError(MP_ERROR_TEXT("value is not a list"));
    }

    mp_obj_list_t *value_list = MP_OBJ_TO_PTR(value_obj);

    if (value_list->len != self->rows) {
        mp_raise_ValueError(MP_ERROR_TEXT("list length not match"));
    }

    for (int row = 0; row < self->rows; row++) {
        self->array[row][index] = mp_obj_get_int(value_list->items[row]);
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(arraytool_replace_col_obj, arraytool_replace_col);

static mp_obj_t arraytool_replace_row(mp_obj_t self_in, mp_obj_t index_obj, mp_obj_t value_obj) {
    arraytool_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_list_t *value_list;
    mp_int_t index = mp_obj_get_int(index_obj);

    if (index < 0 || index >= self->rows) {
        mp_raise_ValueError(MP_ERROR_TEXT("index out of range"));
    }

    if (!mp_obj_is_type(value_obj, &mp_type_list)) {
        mp_raise_ValueError(MP_ERROR_TEXT("value is not a list"));
    }

    value_list = MP_OBJ_TO_PTR(value_obj);

    if (value_list->len != self->cols) {
        mp_raise_ValueError(MP_ERROR_TEXT("list length not match"));
    }

    for (int col = 0; col < self->cols; col++) {
        self->array[index][col] = mp_obj_get_int(value_list->items[col]);
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(arraytool_replace_row_obj, arraytool_replace_row);

static mp_obj_t arraytool_get_col(mp_obj_t self_in, mp_obj_t index_obj) {
    arraytool_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t index = mp_obj_get_int(index_obj);
    mp_obj_list_t *value_list;

    if (self->cols + index < 0 || self->cols + index > self->cols * 2) {
        mp_raise_ValueError(MP_ERROR_TEXT("index out of range"));
    }

    index = index < 0 ? self->cols + index : index;
    value_list = MP_OBJ_TO_PTR(mp_obj_new_list(self->rows, NULL));

    for (int row = 0; row < self->rows; row++) {
        value_list->items[row] = mp_obj_new_int(self->array[row][index]);
    }

    return MP_OBJ_FROM_PTR(value_list);
}
static MP_DEFINE_CONST_FUN_OBJ_2(arraytool_get_col_obj, arraytool_get_col);

static mp_obj_t arraytool_get_row(mp_obj_t self_in, mp_obj_t index_obj) {
    arraytool_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t index = mp_obj_get_int(index_obj);
    mp_obj_list_t *value_list;

    if (self->rows + index < 0 || self->rows + index > self->rows * 2) {
        mp_raise_ValueError(MP_ERROR_TEXT("index out of range"));
    }

    index = index < 0 ? self->rows + index : index;
    value_list = MP_OBJ_TO_PTR(mp_obj_new_list(self->cols, NULL));

    for (int col = 0; col < self->cols; col++) {
        value_list->items[col] = mp_obj_new_int(self->array[index][col]);
    }

    return MP_OBJ_FROM_PTR(value_list);
}
static MP_DEFINE_CONST_FUN_OBJ_2(arraytool_get_row_obj, arraytool_get_row);

static mp_obj_t arraytool_to_list(mp_obj_t self_in) {
    arraytool_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_list_t *value_list = MP_OBJ_TO_PTR(mp_obj_new_list(self->rows * self->cols, NULL));

    for (int col = 0; col < self->cols; col++) {
        for (int row = 0; row < self->rows; row++) {
            value_list->items[col * self->rows + row] = mp_obj_new_int(self->array[row][col]);
        }
    }

    return MP_OBJ_FROM_PTR(value_list);
}
static MP_DEFINE_CONST_FUN_OBJ_1(arraytool_to_list_obj, arraytool_to_list);




static void arraytool_type_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    arraytool_obj_t *self = MP_OBJ_TO_PTR(self_in);

    mp_printf(print, "<ArrayTool cols=%d, rows=%d>", self->cols, self->rows);
}

static mp_obj_t arraytoolbase_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    enum {
        ARG_cols,
        ARG_rows,
    };

    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_cols, MP_ARG_INT, {.u_int = 0}},
        {MP_QSTR_rows, MP_ARG_INT, {.u_int = 0}},
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    arraytool_obj_t *self = m_new_obj(arraytool_obj_t);

    self->base.type = &arraytool_type;
    self->cols = args[ARG_cols].u_int;
    self->rows = args[ARG_rows].u_int;
    self->array = (uint8_t **)malloc(sizeof(uint8_t *) * self->rows);

    for (int row = 0; row < self->rows; row++) {
        self->array[row] = (uint8_t *)malloc(sizeof(uint8_t) * self->cols);
        memset(self->array[row], 0, sizeof(uint8_t) * self->cols);
    }

    return MP_OBJ_FROM_PTR(self);
}

static const mp_rom_map_elem_t arraytool_locals_dict_table[] = {
    {MP_ROM_QSTR(MP_QSTR_init),         MP_ROM_PTR(&arraytool_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_print),        MP_ROM_PTR(&arraytool_print_obj)},
    {MP_ROM_QSTR(MP_QSTR_shift),        MP_ROM_PTR(&arraytool_shift_obj)},
    {MP_ROM_QSTR(MP_QSTR_replace_col),  MP_ROM_PTR(&arraytool_replace_col_obj)},
    {MP_ROM_QSTR(MP_QSTR_replace_row),  MP_ROM_PTR(&arraytool_replace_row_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_col),      MP_ROM_PTR(&arraytool_get_col_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_row),      MP_ROM_PTR(&arraytool_get_row_obj)},
    {MP_ROM_QSTR(MP_QSTR_to_list),      MP_ROM_PTR(&arraytool_to_list_obj)},


    // Constants
    {MP_ROM_QSTR(MP_QSTR_SHIFT_UP),         MP_ROM_INT(UP)},
    {MP_ROM_QSTR(MP_QSTR_SHIFT_DOWN),       MP_ROM_INT(DOWN)},
    {MP_ROM_QSTR(MP_QSTR_SHIFT_LEFT),       MP_ROM_INT(LEFT)},
    {MP_ROM_QSTR(MP_QSTR_SHIFT_RIGHT),      MP_ROM_INT(RIGHT)},
};
static MP_DEFINE_CONST_DICT(arraytool_locals_dict, arraytool_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    arraytool_type,
    MP_QSTR_ArrayTool,
    MP_TYPE_FLAG_NONE,
    print, arraytool_type_print,
    make_new, arraytoolbase_make_new,
    locals_dict, &arraytool_locals_dict
    );
