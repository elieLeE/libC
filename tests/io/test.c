#include <stdio.h>
#include <string.h>

#include "test.h"
#include "../../src/io/io.h"
#include "../../src/mem/mem.h"

bool test_lecture_simple()
{
    FILE* f = ouv_fichier("fichierTest", "r");
    char *s = p_calloc(50*sizeof(*s));
    char temoin[] = "test de lecture";
    bool res;

    fgets(s, 50, f);
    fermer_fichier(&f);

    res = !strncmp(s, temoin, strlen(temoin));

    p_free((void **)&s);

    return res;
}

static bool compar_tab(unsigned int const *tab1,
                       unsigned int const *tab2,
                       unsigned int const taille)
{
    unsigned int i;

    for(i=0; i<taille; i++) {
        if(tab1[i] != tab2[i]) {
            return false;
        }
    }
    return true;
}

void visu(unsigned int *tab, unsigned int taille)
{
    unsigned int i;

    for(i=0; i<taille; i++) {
        printf("%d, ", tab[i]);
    }
    printf("\n");
}

bool test_lecture_matrice()
{
    unsigned int i;
    unsigned int lig = 20;
    unsigned int col = 7;
    unsigned int mat[20][7] = {{35, 34, 58, 4, 59, 3, 47},
        {68, 45, 55, 69, 20, 23, 21},
        {9, 8, 32, 42, 48, 11, 26},
        {68, 52, 63, 34, 19, 49, 10},
        {47, 5, 24, 12, 39, 12, 29},
        {40, 27, 6, 51, 2, 3, 50},
        {34, 39, 13, 43, 59, 46, 28},
        {37, 69, 54, 35, 63, 59, 11},
        {12, 50, 34, 59, 55, 58, 14},
        {37, 13, 43, 19, 40, 61, 0},
        {55, 6, 62, 19, 45, 6, 5},
        {34, 64, 33, 2, 63, 29, 49},
        {56, 19, 3, 11, 69, 37, 0},
        {67, 37, 26, 46, 50, 11, 65},
        {33, 2, 66, 18, 9, 0, 37},
        {66, 6, 54, 43, 0, 17, 57},
        {5, 47, 36, 4, 8, 51, 15},
        {19, 18, 27, 28, 68, 54, 4},
        {48, 65, 0, 11, 10, 8, 41},
        {31, 8, 21, 39, 27, 5, 12}};
    unsigned int **tab =
        (unsigned int **)alloc_tab_2d(lig, col, sizeof(unsigned int));
    FILE* f = ouv_fichier("fichierTestLectureMatrice", "r");

    lire_matrice(f, tab, lig, col);

    for(i=0; i<lig; i++) {
        if(!compar_tab(mat[i], tab[i], 7)) {
            printf("i : %d\n", i);
            visu(tab[i], col);
            return false;
        }
    }

    free_tab_2d((void **)tab, lig);
    fermer_fichier(&f);

    return true;
}

