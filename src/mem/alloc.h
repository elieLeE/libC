#ifndef _SHARED_ALLOC_H_
#define _SHARED_ALLOC_H_

#include <stdio.h>
#include <stdlib.h>

void* p_calloc(size_t const taille);
void* p_realloc(void *p, size_t const taille);

void** alloc_tab_2d(int const lig, int const col, size_t const tailleType);

void free_tab_2d(void** matrice, int const lig);

void _p_free(void **p);
#define p_free(_p)                                                            \
    do {                                                                      \
        _p_free(_p);                                                          \
    } while (0)

#endif
