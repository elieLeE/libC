#ifndef _SHARED_FICHIER_H_
#define _SHARED_FICHIER_H_

#include <stdio.h>
#include <stdlib.h>

FILE* ouvFichier(char* name, char* mode);
void fermerFichier(FILE **f);
void lireMatrice(FILE *fichier, unsigned int* matrice[], int, int);

#endif
