#ifndef _SHARED_ALLOC_H_
#define _SHARED_ALLOC_H_

#include <stdio.h>
#include <stdlib.h>

void* my_calloc(size_t taille);
void* my_realloc(void *p, size_t taille);

void** allocTab2D(int lig, int col, size_t tailleType);

void liberationTab2D(void** matrice, int lig);

#endif
