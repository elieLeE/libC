#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "array.h"
#include "utils.h"
#include "../mem/mem.h"
#include "../attributes.h"
#include "../macros.h"
#include "../logger/logger.h"

typedef enum gv_algo_search_t {
    GV_SEQUENTIAL_SEARCH,
    GV_DICHOTOMY_SEARCH,

    GV_ALGO_MAX_SEARCH,
} gv_algo_search_t;

/* These 'len' and 'size' elements should never be below 0 but it is hard to
 * be absolutely sure about that so I have decided to keep them as signed */
#define generic_vector_data_t(_type)                                          \
    struct {                                                                  \
        _type *tab;                                                           \
        int64_t len, size;                                                    \
        const size_t __size_elem;                                             \
    }

typedef generic_vector_data_t(void) __vector_void_t;

#define gv_t(_name_) generic_vector_##_name_##_t

/* len is the number of elements in the vector.
 * size if the size of the vector. Indeed, when elements are removed,
 * the array is not automatically reallocated.
 * We use a union with almost 2 same type in order to call some generics
 * methods giving to them __vector_void_t' when it will be possible. */
#define generic_vector_t(_name, _type)                                        \
    typedef union gv_t(_name) {                                               \
        generic_vector_data_t(_type);                                         \
        __vector_void_t vec;                                                  \
    } gv_t(_name);

#define __gv_size(_vec) sizeof(_vec->tab[0])
#define __gv_type(_vec) typeof(_vec->tab[0])

/* {{{ Macros gv_for_each* */

#define gv_for_each_pos(_pos, _gvec)                                          \
    for (int64_t _pos = 0; _pos < (_gvec)->len; _pos++)

#define gv_for_each_pos_rev(_pos, _gvec)                                      \
    for (int64_t _pos = (_gvec)->len - 1; _pos >= 0; _pos--)

/* #define gv_for_each(_elem, _gvec)                                       \ */
/*     int64_t CONCAT(_p, __LINE__) = 0;                                   \ */
/*     for (__gv_type((_gvec)) _elem = (_gvec)->tab[CONCAT(_p, __LINE__)]; \ */
/*          CONCAT(_p, __LINE__) < (_gvec)->len;                           \ */
/*          _elem = (_gvec)->tab[++(CONCAT(_p, __LINE__))])                  */

/* #define gv_for_each_rev(_elem, _gvec)                                   \ */
/*     int64_t CONCAT(_p, __LINE__) = (_gvec)->len - 1;                    \ */
/*     for (__gv_type((_gvec)) _elem = (_gvec)->tab[CONCAT(_p, __LINE__)]; \ */
/*          CONCAT(_p, __LINE__) >= 0;                                     \ */
/*          _elem = (_gvec)->tab[--(CONCAT(_p, __LINE__))])                  */

#define gv_for_each_p(_elem, _gvec)                                           \
    for (__gv_type((_gvec)) *_elem = (_gvec)->tab;                            \
         _elem != (_gvec)->tab + (_gvec)->len; _elem++)

#define gv_for_each_const_p(_elem, _v)                                        \
    for (const __gv_type((_v)) *_elem = (_v)->tab;                            \
         _elem != (_v)->tab + (_v)->len; _elem++)

#define gv_for_each_p_rev(_elem, _gvec)                                       \
    for (__gv_type((_gvec)) *_elem = (_gvec)->tab + (_gvec)->len - 1;         \
         _elem != (_gvec)->tab; _elem--)

#define gv_for_each_const_p_rev(_elem, _gvec)                                 \
    for (const __gv_type((_gvec)) *_elem = (_gvec)->tab + (_gvec)->len - 1;   \
         _elem != (_gvec)->tab; _elem--)

/* }}} */

void *__gv_extend(__vector_void_t *vec, int64_t extra);
#define gv_extend(_gvec, extra)                                               \
    __gv_extend(&(_gvec)->vec, extra)

void *__gv_grow(__vector_void_t *vec, int64_t extra);
#define gv_grow(_gvec, extra)                                                 \
    (__gv_type((_gvec)) *)__gv_grow(&(_gvec)->vec, extra);

#define gv_grow1(_vec) gv_grow((_vec), 1)

static inline void __gv_set_size_elem(__vector_void_t *vec, size_t size_elem)
{
    size_t *size_elem_p;

    size_elem_p = unconst_cast(size_t, &vec->__size_elem);
    if (size_elem_p == NULL) {
        logger_fatal("error when trying to set '__size_elem'");
    }
    *size_elem_p = size_elem;
}

#define gv_init(_gvec)                                                        \
    ({  __auto_type __gvec = (_gvec);                                         \
        p_clear(__gvec, 1);                                                   \
        __gv_set_size_elem(&__gvec->vec, __gv_size(__gvec));                  \
        __gvec;                                                               \
    })

#define gv_new(_name)                                                         \
    ({  gv_t(_name) *__gvec = p_calloc(sizeof(gv_t(_name)));                  \
        if (__gvec != NULL) {                                                 \
            gv_init(__gvec);                                                  \
        }                                                                     \
        __gvec;                                                               \
    })

#define gv_init_size(_gvec, _size)                                            \
    do {                                                                      \
        int64_t __size = (_size);                                             \
        if (__size < 0) {                                                     \
            logger_fatal("_size is below 0: %ld", __size);                    \
        }                                                                     \
        __auto_type __gvec = (_gvec);                                         \
        p_clear(__gvec, 1);                                                   \
        __gv_set_size_elem(&__gvec->vec, __gv_size(__gvec));                  \
        __gv_extend(&__gvec->vec, _size);                                     \
    } while (0)

void __gv_set(const __vector_void_t *src, __vector_void_t *dst);
#define gv_set(_src, _dst)                                                    \
    __gv_set(&(_src)->vec, &(_dst)->vec);

/* This method can be used to co a partial copy of the source vector in the
 * destination vector */
int __gv_copy(const __vector_void_t *src, int64_t idx_start_src,
              int64_t idx_start_dst, int64_t len, __vector_void_t *dst);
#define gv_copy(_src, _idx_src, _idx_dst, len, _dst)                          \
    __gv_copy(&(_src)->vec, (_idx_src), (_idx_dst), (len), &(_dst)->vec)

#define gv_append(_gvec, _val)                                                \
    ({                                                                        \
        int64_t __res = -1;                                                   \
        __auto_type __gvec = (_gvec);                                         \
        __gv_type(__gvec) *__elem = __gv_grow(&__gvec->vec, 1);               \
        if (__elem != NULL) {                                                 \
            *__elem = (_val);                                                 \
            __res = __gvec->len -1;                                           \
        }                                                                     \
        __res;                                                                \
    })

void *__gv_create_empty_spot_at_pos(__vector_void_t *vec, int64_t pos);
#define gv_insert_elem_at_pos(_gvec, _val, _pos)                              \
    ({                                                                        \
        int __res = -1, __pos = (_pos);                                       \
        __auto_type __gvec = (_gvec);                                         \
        __gv_type(__gvec) *__elem;                                            \
        __elem =__gv_create_empty_spot_at_pos(&__gvec->vec, __pos);           \
        if (__elem != NULL) {                                                 \
            *__elem = (_val);                                                 \
            __res = __pos;                                                    \
        }                                                                     \
        __res;                                                                \
    })

void *__gv_create_empty_spot_in_sorted_vec_from_val(__vector_void_t *vec,
                                                    void *elem, int64_t *pos,
                                                    g_cmp_cb_t cmp_cb);
#define gv_insert_elem_sorted(_gvec, _val, cmp_cb)                            \
    ({                                                                        \
        __auto_type __val = (_val);                                           \
        __auto_type __gvec = (_gvec);                                         \
        int64_t __pos = -1;                                                   \
        __gv_type(__gvec) *__elem;                                            \
        __elem = __gv_create_empty_spot_in_sorted_vec_from_val(&__gvec->vec,  \
                                                               &__val, &__pos,\
                                                               cmp_cb);       \
        if (__elem != NULL) {                                                 \
            *__elem = __val;                                                  \
        }                                                                     \
        __pos;                                                                \
    })

/* Warning, this method decreases the length of the vector, eventually move the
 * data in order that the elements from the first one to the 'vec->length are
 * the elements of the vector, but the new "empty" spot are not reset ! */
int __gv_remove_elem_n(__vector_void_t *vec, int64_t pos);
#define gv_remove(_gvec, _pos) __gv_remove_elem_n(&(_gvec)->vec, _pos)

#define gv_take(_gvec, _pos)                                                  \
    ({                                                                        \
        __auto_type __gvec = (_gvec);                                         \
        __auto_type __elem = NULL;                                            \
        int64_t __pos = (_pos);                                               \
        if (__pos >= 0 && __pos < __gvec->len) {                              \
            __elem = __gvec->tab[_pos];                                       \
            __gv_remove_elem_n(&__gvec->vec, __pos);                          \
        }                                                                     \
        __elem;                                                               \
    })

static inline void
__gv_sort(__vector_void_t *vec, g_cmp_cb_t cmp_cb)
{
    qsort(vec->tab, vec->len, vec->__size_elem, cmp_cb);
}

#define gv_sort(_gvec, cmp_cb)                                                \
    do {                                                                      \
        __gv_sort(&((_gvec)->vec), cmp_cb);                                   \
    } while (0)

#define gv_shuffle(_gvec)                                                     \
    do {                                                                      \
        __auto_type __gvec = (_gvec);                                         \
        shuffle_tab(__gvec->vec.tab, __gvec->len, __gvec->__size_elem);       \
    } while (0)

int64_t __gv_find(const __vector_void_t *vec, const void *elem,
                  gv_algo_search_t algo, g_cmp_cb_t cmp_cb);

#define gv_find(_gvec, _elem, algo, cmp_cb)                                   \
    ({                                                                        \
        __auto_type __elem = (_elem);                                         \
        __gv_find(&(_gvec)->vec, &__elem, algo, cmp_cb);                      \
    })

#define gv_contains(_gvec, _elem, algo, cmp_cb)                               \
    gv_find(_gvec, _elem, algo, cmp_cb) >= 0

/* This method only reset the len of the vector to 0. So, new elements will
 * replace the old ones gradually but that needs test */
void __gv_fast_clear(__vector_void_t *vec);
#define gv_fast_clear(_gvec)                                                  \
    __gv_fast_clear(&(_gvec)->vec)

void __gv_clear(__vector_void_t *vec, void (*free_data_cb)(void **));
void __gv_wipe(__vector_void_t *vec, void (*free_data_cb)(void **));

#define gv_clear(_gvec, free_data_cb)                                         \
    __gv_clear(&(_gvec)->vec, free_data_cb)

#define gv_wipe(_gvec, free_data_cb)                                          \
    __gv_wipe(&(_gvec)->vec, free_data_cb)

#define gv_delete(_gvec, free_data_cb)                                        \
    do {                                                                      \
        __gv_wipe(&(_gvec)->vec, free_data_cb);                               \
        p_free((void **)&(_gvec));                                            \
    } while (0)


/* I am not sure about that. To see. To be used only with vector type on which
 * mathematical operations can do. */
#define get_all_elem_sum(_gvec)                                               \
    ({                                                                        \
        __auto_type __gvec = (_gvec);                                         \
        __gv_type(__gvec) __sum = 0;                                          \
        gv_for_each_pos(pos, __gvec) {                                        \
            __sum += __gvec->tab[pos];                                        \
        }                                                                     \
       __sum;                                                                 \
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
