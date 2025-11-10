#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>
#include <stdint.h>

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

#define gv_init(_gvec)                                                        \
    ({  __auto_type __gvec = (_gvec);                                         \
        p_clear(__gvec, 1);                                                   \
   })

#define gv_free(_gvec, free_data_cb) \
    ({  __auto_type __gvec = (_gvec);                                         \
        p_free((void **)&(__gvec->tab));                                      \
    })

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
