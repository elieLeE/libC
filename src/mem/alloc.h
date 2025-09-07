#ifndef _SHARED_ALLOC_H_
#define _SHARED_ALLOC_H_

#include <stdio.h>
#include <stdlib.h>

void* my_calloc(size_t const taille);
void* my_realloc(void *p, size_t const taille);

void** allocTab2D(int const lig, int const col, size_t const tailleType);

void liberationTab2D(void** matrice, int const lig);

void p_free(void **p);

#endif
