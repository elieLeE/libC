#include <stdio.h>

#include "test_io.h"
#include "../src/io/io.h"
#include "../src/mem/mem.h"
#include "../src/macros.h"

void test_lecture_simple()
{
    FILE* f = ouv_fichier("fichierTest", "r");
    char *s = p_calloc(50*sizeof(*s));
    char temoin[] = "test de lecture\n";

    fgets(s, 50, f);
    fermer_fichier(&f);

    ASSERT_STR_EQUAL(s, temoin);

    p_free((void **)&s);
}

void test_lecture_matrice()
{
    int lig = 20;
    int col = 7;
    int mat[20][7] = {{35, 34, 58, 4, 59, 3, 47},
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
    int **tab = (int **)alloc_tab_2d(lig, col, sizeof(int));
    FILE* f = ouv_fichier("fichierTestLectureMatrice", "r");

    lire_matrice(f, tab, lig, col);

    for (int i = 0; i < lig; i++) {
        for (int j = 0; j < col; j++) {
            ASSERT_EQUAL(mat[i][j], tab[i][j]);
        }
    }

    free_tab_2d((void **)tab, lig);
    fermer_fichier(&f);
}

module_tests_t *get_all_tests_io(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "IO");
    ADD_TEST_TO_MODULE(module_tests, test_lecture_simple);
    ADD_TEST_TO_MODULE(module_tests, test_lecture_matrice);

    return module_tests;
}
