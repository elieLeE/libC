#include "test.h"

bool testTrieFusion(int (*comparElement)(void const*, void const*)){
    //unsigned int tab[TAILLE_TEST];
    unsigned int *tab = p_calloc(TAILLE_TEST*sizeof(*tab));

    randomTabRemplissage(tab, TAILLE_TEST);
    trieFusion(tab, TAILLE_TEST, sizeof(*tab), comparElement);

    return verifTriageTab(tab, TAILLE_TEST);
}

void testQSort(int (*comparElement)(void const*, void const*)){
    //unsigned int tab[TAILLE_TEST];
    unsigned int *tab = p_calloc(TAILLE_TEST*sizeof(*tab));

    randomTabRemplissage(tab, TAILLE_TEST);
    qsort(tab, TAILLE_TEST, sizeof(unsigned int), comparElement);
}

bool testSearch(int (*comparElement)(void const*, void const*), void(*visuElem)(void const*)){
    unsigned int tab[TAILLE_TEST];

    constantTabRemplissage(tab, TAILLE_TEST);

    visuTab(tab, 10, sizeof(*tab), visuElem, "\b\b \n");

    unsigned int ind = 3;
    printf("val searched : %d => %d\n", tab[ind], searchInd(tab, TAILLE_TEST, sizeof(unsigned int), &tab[ind], (int(*)(void const*, void const*))comparElement));

    return false;
}

