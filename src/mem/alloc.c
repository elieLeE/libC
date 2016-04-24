#include "alloc.h"

void* my_calloc(size_t const taille){
    if(taille<=0){
	fprintf(stderr, "taille <= 0, fichier %s, ligne %d\n", __FILE__, __LINE__);
	exit(0);
    }
    void *p = calloc(1, taille);
    if(p == NULL){
	fprintf(stderr, "allocation incorrecte, fichier %s, ligne %d\n", __FILE__, __LINE__);
	exit(0);
    }
    return p;
}

void* my_realloc(void *p, size_t const taille){
    if(taille<=0){
	fprintf(stderr, "taille <= 0, fichier %s, ligne %d\n", __FILE__, __LINE__);
	exit(0);
    }
    p = realloc(p, taille);
    if(p == NULL){
	fprintf(stderr, "allocation incorrecte, fichier %s, ligne %d\n", __FILE__, __LINE__);
	exit(0);
    }
    return p;
}

void** allocTab2D(int const lig, int const col, size_t const tailleType){
    if((tailleType<=0) || (lig<=0) || (col<=0)){
	fprintf(stderr, "taille, lig ou col <= 0, fichier %s, ligne %d\n", __FILE__, __LINE__);
	exit(0);
    }
    int i, t;
    void **tab = my_calloc(lig*sizeof(void*));
    t = col*tailleType;
    for(i=0; i<lig; i++){
	tab[i] = my_calloc(t);
    }
    return tab;
}

void liberationTab2D(void** matrice, int const lig){
    if(lig<=0){
	fprintf(stderr, "lig <= 0, fichier %s, ligne %d\n", __FILE__, __LINE__);
	exit(0);
    }
    int i;
    for(i=0; i<lig; i++){
	free(matrice[i]);
	matrice[i] = NULL;
    }
    free(matrice);
    matrice = NULL;
}
