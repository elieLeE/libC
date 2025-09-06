#ifndef _SHARED_SEARCH_H_
#define _SHARED_SEARCH_H_

#include <stdlib.h>

typedef struct structDichoSearch structDichoSearch;
struct structDichoSearch{
    void const *tab;
    unsigned int tailleTab;
    size_t tailleType;
    void const *valSearched;
    int (*compar)(void const*, void const*);
    unsigned int deb, pos, fin;
};

void visuTab(void const* tab, unsigned int const tailleTab, size_t const tailleType, void (*visuElem)(void const *e), char const *s);
/*
 * retourne l'indice de l'élément le plus proche de n
 * la comparaison est faîte par la fonction pointée par le pinteur de fonction en argument
 * */
int searchInd(void const *tab, unsigned int const tailleTab, size_t const, void const *n, int (*compar)(void const*, void const*));
int searchBasique(structDichoSearch *s);
int searchDicho(structDichoSearch *s);

#endif
