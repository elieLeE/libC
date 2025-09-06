#include "swap.h"
#include "mem/alloc.h"

void echangeGenerique(void* a, void* b, size_t taille){
    void *c = my_calloc(taille);
    memcpy(c, a, taille);
    memcpy(a, b, taille);
    memcpy(b, c, taille);
}

