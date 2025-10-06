#ifndef _FICHIER_H_
#define _FICHIER_H_

#include <stdio.h>
#include <stdlib.h>

void flush_stdin(void);

FILE *ouv_fichier(char const *name, char const *mode);
void fermer_fichier(FILE **f);
void lire_matrice(FILE* fichier, unsigned int* matrice[],
                  int const tailleLig, int const tailleCol);

#endif
