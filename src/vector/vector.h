#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>
#include <stdint.h>

#include "../mem/mem.h"
#include "../../libc.h"

/* TODO
 * gv_add_sorted
 * gv_remove_*
 * gv_sort => use qsort
 * gv_find / gv_contains
 * gv_shuffle
 * gv_for_each
 */

#define generic_vector_data_t(_type)                                          \
    struct {                                                                  \
        _type *tab;                                                           \
        int len, size;                                                        \
    }

typedef generic_vector_data_t(void) __vector_void_t;

/* len is the number of elements in the vector.
 * size if the size of the vector. Indeed, when elements are removed,
 * the array is not automatically reallocated.
 * We use a union with almost 2 same type in order to call some generics
 * methods giving to them __vector_void_t' when it will be possible. */
#define generic_vector_t(_name, _type)                                        \
    typedef union generic_vector_##_name##_t {                                \
        generic_vector_data_t(_type);                                         \
        __vector_void_t vec;                                                  \
    } generic_vector_##_name##_t;

#define gv_t(_name) generic_vector_##_name##_t

#define __gv_size(_vec) sizeof(_vec->tab[0])
#define __gv_type(_vec) typeof(_vec->tab[0])

#define gv_for_each_pos(_pos, _gvec)                                          \
    for (int _pos = 0; _pos < (_gvec)->len; _pos++)

void *__gv_grow(__vector_void_t *vec, int extra, size_t size_elem);
void *__gv_extend(__vector_void_t *vec, int extra, size_t size_elem);

#define gv_grow(_gvec, extra)                                                 \
    ({                                                                        \
        __auto_type __gvec = _gvec;                                           \
        __gv_type(__gvec) *__elem = __gv_grow(&(__gvec)->vec, extra,          \
                                              __gv_size((__gvec)));           \
        __elem;                                                               \
    })

#define gv_grow1(_vec) gv_grow((_vec), 1)

#define gv_init(_gvec)                                                        \
    do {                                                                      \
        __auto_type __gvec = (_gvec);                                         \
        p_clear(__gvec, 1);                                                   \
    } while (0)

#define gv_new(_name)                                                         \
    (gv_t(_name) *)p_calloc(sizeof(gv_t(_name)))                              \

#define gv_init_size(_gvec, _size)                                            \
    do {                                                                      \
        __auto_type __gvec = (_gvec);                                         \
        p_clear(__gvec, 1);                                                   \
        gv_grow(&__gvec->vec, _size, gv_size(__gvec));                        \
    } while (0)

#define gv_add(_gvec, _val)                                                   \
    ({  __auto_type __gvec = (_gvec);                                         \
        __gv_type(__gvec) *__elem = __gv_grow(&__gvec->vec, 1,                \
                                              __gv_size(__gvec));             \
        *__elem = _val;                                                       \
    })

void *__gv_create_empty_spot(__vector_void_t *vec, size_t size_elem, int pos);

#define __set_elem_at_new_spot(__gvec, _val, _pos)                            \
    do {                                                                      \
        __elem =__gv_create_empty_spot(&__gvec->vec, __gv_size(__gvec),       \
                                       _pos);                                 \
        if (__elem != NULL) {                                                 \
            *__elem = _val;                                                   \
            __gvec->len++;                                                    \
        }                                                                     \
    } while(0)

#define gv_insert_elem_at_pos(_gvec, _val, _pos)                              \
    do {                                                                      \
        __auto_type __gvec = (_gvec);                                         \
        __gv_type(__gvec) *__elem;                                            \
        __gv_extend(&__gvec->vec, 1, __gv_size(__gvec));                      \
        __set_elem_at_new_spot(__gvec, _val, _pos);                           \
    } while(0)

void _gv_clear(__vector_void_t *vec, int size_elem,
              void (*free_data_cb)(void **));
void _gv_wipe(__vector_void_t *vec, int size_elem,
              void (*free_data_cb)(void **));

#define gv_clear(_gvec, free_data_cb)                                         \
    _gv_clear(&(_gvec)->vec, __gv_size((_gvec)), free_data_cb)

#define gv_wipe(_gvec, free_data_cb)                                          \
    _gv_wipe(&(_gvec)->vec, __gv_size((_gvec)), free_data_cb)

#define gv_delete(_gvec, free_data_cb)                                        \
    do {                                                                      \
        _gv_wipe(&(_gvec)->vec, __gv_size((_gvec)), free_data_cb);            \
        p_free((void **)&(_gvec));                                            \
    } while (0)


generic_vector_t(int8, int8_t);
generic_vector_t(uint8, uint8_t);
generic_vector_t(int16, int16_t);
generic_vector_t(uint16, uint16_t);
generic_vector_t(int32, int32_t);
generic_vector_t(uint32, uint32_t);
generic_vector_t(int64, int64_t);
generic_vector_t(uint64, uint64_t);
generic_vector_t(double, double);
generic_vector_t(string, char *);

#endif
