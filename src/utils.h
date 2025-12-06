#ifndef __UTILS_H__
#define __UTILS_H__

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#define SWAP(_type, _a, _b)                                                   \
    do {                                                                      \
        _type _c = *_b;                                                       \
        *_b = *_a;                                                            \
        *_a = _c;                                                             \
    } while(0)

/* Warning, this method allocate a memory space of the size of 'size_elem'
 * in order to exchange the containing in d1 and d2. If you can, use the macro
 * SWAP or the method 'swap_cpy_fast'. */
void swap_cpy(void *a, void *b, size_t size_elem);

/* In this method, the tmp field is already allocated. So, it is up to the user
 * to allocate it correctly and especially only when it is necessary and as
 * little as possible. Moreover, this methods uses memcpy in order to copy the
 * data. */
void swap_cpy_fast(void *d1, void *d2, void *tmp, size_t size_elem);

/* {{{ cmp methods */

static inline int g_cmp_int8(const void *_d1, const void *_d2)
{
    const int8_t *d1 = (const int8_t *)_d1;
    const int8_t *d2 = (const int8_t *)_d2;

    return *d1 - *d2;
}

static inline int g_cmp_int8_rev(const void *_d1, const void *_d2)
{
    return - g_cmp_int8(_d1, _d2);
}

static inline int g_cmp_uint8(const void *_d1, const void *_d2)
{
    const uint8_t *d1 = (const uint8_t *)_d1;
    const uint8_t *d2 = (const uint8_t *)_d2;

    return *d1 - *d2;
}

static inline int g_cmp_uint8_rev(const void *_d1, const void *_d2)
{
    return - g_cmp_uint8(_d1, _d2);
}

static inline int g_cmp_int16(const void *_d1, const void *_d2)
{
    const int16_t *d1 = (const int16_t *)_d1;
    const int16_t *d2 = (const int16_t *)_d2;

    return *d1 - *d2;
}

static inline int g_cmp_int16_rev(const void *_d1, const void *_d2)
{
    return - g_cmp_int16(_d1, _d2);
}

static inline int g_cmp_uint16(const void *_d1, const void *_d2)
{
    const uint16_t *d1 = (const uint16_t *)_d1;
    const uint16_t *d2 = (const uint16_t *)_d2;

    return *d1 - *d2;
}

static inline int g_cmp_uint16_rev(const void *_d1, const void *_d2)
{
    return - g_cmp_uint16(_d1, _d2);
}

static inline int g_cmp_int32(const void *_d1, const void *_d2)
{
    const int32_t *d1 = (const int32_t *)_d1;
    const int32_t *d2 = (const int32_t *)_d2;

    return *d1 - *d2;
}

static inline int g_cmp_int32_rev(const void *_d1, const void *_d2)
{
    return - g_cmp_int32(_d1, _d2);
}

static inline int g_cmp_uint32(const void *_d1, const void *_d2)
{
    const int32_t *d1 = (const int32_t *)_d1;
    const int32_t *d2 = (const int32_t *)_d2;

    return *d1 - *d2;
}

static inline int g_cmp_uint32_rev(const void *_d1, const void *_d2)
{
    return - g_cmp_uint32(_d1, _d2);
}

static inline int g_cmp_int64(const void *_d1, const void *_d2)
{
    const int64_t *d1 = (const int64_t *)_d1;
    const int64_t *d2 = (const int64_t *)_d2;

    return *d1 - *d2;
}

static inline int g_cmp_int64_rev(const void *_d1, const void *_d2)
{
    return - g_cmp_int64(_d1, _d2);
}

static inline int g_cmp_uint64(const void *_d1, const void *_d2)
{
    const uint64_t *d1 = (const uint64_t *)_d1;
    const uint64_t *d2 = (const uint64_t *)_d2;

    return *d1 - *d2;
}

static inline int g_cmp_uint64_rev(const void *_d1, const void *_d2)
{
    return - g_cmp_uint64(_d1, _d2);
}

static inline int g_cmp_double(const void *_d1, const void *_d2)
{
    const double *d1 = (const double *)_d1;
    const double *d2 = (const double *)_d2;

    return *d1 - *d2;
}

static inline int g_cmp_double_rev(const void *_d1, const void *_d2)
{
    return - g_cmp_double(_d1, _d2);
}

static inline int g_cmp_str(const void *_str1, const void *_str2)
{
    const char **str1 = (const char **)_str1;
    const char **str2 = (const char **)_str2;

    return strcmp(*str1, *str2);
}

static inline int g_cmp_str_rev(const void *_str1, const void *_str2)
{
    return - g_cmp_str(_str1, _str2);
}

/* }}} */

#endif


