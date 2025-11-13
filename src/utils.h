#ifndef __UTILS_H__
#define __UTILS_H__

#include <string.h>

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

#endif


