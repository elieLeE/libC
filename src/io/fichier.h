#ifndef _FICHIER_H_
#define _FICHIER_H_

#include <stdio.h>
#include <stdlib.h>

FILE* ouvFichier(char const *name, char const *mode);
void fermerFichier(FILE **f);
void lireMatrice(FILE* fichier, unsigned int* matrice[], int const tailleLig, int const tailleCol);

#endif
