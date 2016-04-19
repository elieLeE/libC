#ifndef _SHARED_SEARCH_H_
#define _SHARED_SEARCH_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void visuTab(unsigned int* tab, unsigned int tailleTab, void (*visuElem)(void *e), char* s);
/*
 * retourne l'indice de l'élément le plus proche de n
 * la comparaison est faîte par la fonction pointée par le pinteur de fonction en argument
 * */
int searchInd(void *tab, int tailleTab, size_t, void * n, int (*compar)(void*, void*));

#endif
