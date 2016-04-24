#ifndef _SHARED_SEARCH_H_
#define _SHARED_SEARCH_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void visuTab(void const* tab, unsigned int const tailleTab, size_t const tailleType, void (*visuElem)(void const *e), char const *s);
/*
 * retourne l'indice de l'élément le plus proche de n
 * la comparaison est faîte par la fonction pointée par le pinteur de fonction en argument
 * */
int searchInd(void const *tab, int const tailleTab, size_t const, void const *n, int (*compar)(void const*, void const*));

#endif
