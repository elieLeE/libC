#include "io.h"

void flush_stdin(void)
{
    char c;

    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

FILE *ouv_fichier(char const *name, char const *mode)
{
    FILE* f = fopen(name, mode);

    if (f == NULL) {
        fprintf(stderr, "impossible d'ouvrir le fichier %s\n", name);
        exit(0);
    }
    return f;
}

void fermer_fichier(FILE **f)
{
    fclose(*f);
}

void lire_matrice(FILE* fichier, unsigned int* matrice[],
                  int const tailleLig, int const tailleCol)
{
    int i, j;

    for (i = 0; i < tailleLig; i++) {
        for (j = 0; j < tailleCol; j++) {
            fscanf(fichier, "%d", &matrice[i][j]);
        }
    }
}
