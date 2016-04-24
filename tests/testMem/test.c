#include "test.h"

void testMyCalloc(){
    printf("testMyCalloc : \n");
    int *a = my_calloc(sizeof(*a));
    *a = 3;
    printf("a : %d\n", *a);
    free(a);
    a = NULL;

    a = my_calloc(sizeof(*a));
    *a = 10;
    printf("a : %d\n", *a);
    free(a);
    a = NULL;
}

void testMyRealloc(){
    printf("\ntestMyRealloc : \n");
    unsigned int i;
    unsigned int max = 100;
    unsigned int min = 10;
    int *a = my_calloc(sizeof(*a));
    *a = 3;
    printf("a : %d\n", *a);

    unsigned int taille = rand()%(max - min) + min;
    a = my_realloc(a, taille*sizeof(*a));
    for(i=0; i<taille; i++){
	a[i] = rand()%(taille*max - min) + min;
    }

    taille = rand()%(100 - 1) + 1;
    a = my_realloc(a, taille*sizeof(*a));
    for(i=0; i<taille; i++){
	a[i] = rand()%(taille*max - min) + min;
    }
    free(a);
}

void testAllocTab2D(){
    printf("\ntestAllocTab2D : \n");
    unsigned int max = 100;
    unsigned int min = 10;
    unsigned int nbreL = rand()%(max - min) + min;
    unsigned int nbreC = rand()%(max - min) + min;
    unsigned int i, j;

    int **tab = (int **)allocTab2D(nbreL, nbreC, sizeof(**tab));
    for(i=0; i<nbreL; i++){
	for(j=0; j<nbreC; j++){
	    tab[i][j] = rand()%((nbreL + nbreC)*max - min) + min;
	}
    }

    free(tab);
}


    
