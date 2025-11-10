#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>

#include "../mem/mem.h"

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

#endif
