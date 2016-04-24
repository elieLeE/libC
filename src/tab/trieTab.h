#ifndef _TRIE_TAB_H_
#define _TRIE_TAB_H_

/*
 * trie fusion generique
 * environ 2 * plus lent que le qsort
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../mem/alloc.h"

void trieFusion(void *tab, unsigned int const tailleTab, size_t const tailleType, int (*comparElement)(void const *d1, void const *d2));
void division(void *tab, unsigned int const deb, unsigned int const fin, size_t const tailleType, int (*comparElement)(void const *d1, void const *d2));
void fusion(void *tab, unsigned int const deb, unsigned int const mil, unsigned int const fin, size_t const tailleType, int (*comparElement)(void const *d1, void const *d2));

#endif
