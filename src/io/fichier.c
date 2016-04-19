#include "fichier.h"

FILE* ouvFichier(char* name, char* mode){
    FILE* f = fopen(name, mode);
    if(f == NULL){
	fprintf(stderr, "impossible d'ouvrir le fichier %s\n", name);
	exit(0);
    }
    return f;
}

void fermerFichier(FILE **f){
    fclose(*f);
}

void lireMatrice(FILE* fichier, unsigned int* matrice[], int tailleLig, int tailleCol){
    int i, j;
    for(i=0; i<tailleLig; i++){
	for(j=0; j<tailleCol; j++){
	    fscanf(fichier, "%d", &matrice[i][j]);
	}
    }
}
