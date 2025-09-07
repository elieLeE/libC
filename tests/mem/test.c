#include <stdio.h>

#include "test.h"
#include "../../src/mem/alloc.h"


void testMyCalloc(){
    printf("test p_calloc : \n");
    int *a = p_calloc(sizeof(*a));
    *a = 3;
    printf("a : %d\n", *a);
    p_free((void **)&a);
    a = NULL;

    a = p_calloc(sizeof(*a));
    *a = 10;
    printf("a : %d\n", *a);
    p_free((void **)&a);
    a = NULL;
}

void testMyRealloc(){
    printf("\ntest p_realloc : \n");
    unsigned int i;
    unsigned int max = 100;
    unsigned int min = 10;
    int *a = p_calloc(sizeof(*a));
    *a = 3;
    printf("a : %d\n", *a);

    unsigned int taille = rand()%(max - min) + min;
    a = p_realloc(a, taille*sizeof(*a));
    for(i=0; i<taille; i++){
        a[i] = rand()%(taille*max - min) + min;
    }

    taille = rand()%(100 - 1) + 1;
    a = p_realloc(a, taille*sizeof(*a));
    for(i=0; i<taille; i++){
        a[i] = rand()%(taille*max - min) + min;
    }
    p_free((void **)&a);
}

void testAllocTab2D(){
    printf("\ntestAllocTab2D : ");
    unsigned int max = 100;
    unsigned int min = 10;
    unsigned int nbreL = rand()%(max - min) + min;
    unsigned int nbreC = rand()%(max - min) + min;
    unsigned int i, j;

    int **tab = (int **)alloc_tab_2d(nbreL, nbreC, sizeof(**tab));
    for(i=0; i<nbreL; i++){
        for(j=0; j<nbreC; j++){
            tab[i][j] = rand()%((nbreL + nbreC)*max - min) + min;
        }
    }

    p_free((void **)&tab);

    printf("OK\n");
}



