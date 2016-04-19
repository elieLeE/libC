#include "alloc.h"

void* my_calloc(size_t taille){
    void *p = calloc(1, taille);
    if(p == NULL){
	fprintf(stderr, "allocation incorrecte, fichier %s, ligne %d\n", __FILE__, __LINE__);
	exit(0);
    }
    return p;
}

void* my_realloc(void *p, size_t taille){
    p = realloc(p, taille);
    if(p == NULL){
	fprintf(stderr, "allocation incorrecte, fichier %s, ligne %d\n", __FILE__, __LINE__);
	exit(0);
    }
    return p;
}

void** allocTab2D(int lig, int col, int tailleType){
    int i, t;
    void **tab = my_calloc(lig*sizeof(void*));
    t = col*tailleType;
    for(i=0; i<lig; i++){
	tab[i] = my_calloc(t);
    }
    return tab;
}

void liberationTab2D(void** matrice, int lig){
    int i;
    for(i=0; i<lig; i++){
	free(matrice[i]);
    }
    free(matrice);
}
