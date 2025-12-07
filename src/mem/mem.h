#ifndef _SHARED_ALLOC_H_
#define _SHARED_ALLOC_H_

#include <stdio.h>
#include <stdlib.h>

__attribute__((malloc)) void* p_malloc(size_t const taille);
__attribute__((malloc)) void* p_calloc(size_t const taille);
__attribute__((malloc))
void* p_realloc(void *p, size_t const taille);

void _p_clear(void *p, size_t size);
#define p_clear(_p, _n)                                                       \
    do {                                                                      \
        _p_clear(_p, sizeof(*_p) * _n);                                       \
    } while (0)

__attribute__((malloc))
void** alloc_tab_2d(int const lig, int const col, size_t const tailleType);

void free_tab_2d(void** matrice, int const lig);

void _p_free(void **p);
#define p_free(_p)                                                            \
    do {                                                                      \
        _p_free(_p);                                                          \
    } while (0)

#define CREATE_GENERIC_INIT(_type_, _prefix_)                                 \
    static inline _type_ *_prefix_##_init(_type_ *p) {                        \
        p_clear(p, 1);                                                        \
        return p;                                                             \
    }

#define CREATE_GENERIC_NEW(_type_, _prefix_)                                  \
    __attribute__((malloc))                                                   \
    static inline _type_ *_prefix_##_new(void) {                              \
        return _prefix_##_init((_type_ *)p_malloc(sizeof(_type_))); }

#define CREATE_GENERIC_NEW_INIT(_type_, _prefix_)                             \
    CREATE_GENERIC_INIT(_type_, _prefix_);                                    \
    CREATE_GENERIC_NEW(_type_, _prefix_)

#endif
