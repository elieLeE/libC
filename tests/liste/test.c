#include "test.h"
#include "../../src/mem/alloc.h"
#include "verif.h"

void visu_element(void const *data){
    printf("%d, ", *((int *)data));
}

void remove_element(void *data){
    free(data);
}

int compar_element(void const *d1, void const *d2){
    int const *a = d1;
    int const *b = d2;

    return (*a - *b);
}

void test_add_and_remove_element(liste *l)
{
    int *a = p_calloc(sizeof(int));
    int *b = p_calloc(sizeof(int));
    int *c = p_calloc(sizeof(int));
    int *d = p_calloc(sizeof(int));
    int *e = p_calloc(sizeof(int));
    int *f = p_calloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;
    *e = 5;
    *f = 6;

    add_element_last(l, a);
    add_element_first(l, b);
    add_element_last(l, c);
    remove_first_element(l, remove_element);
    add_element_last(l, d);
    add_element_first(l, e);
    remove_last_element(l, remove_element);
    remove_first_element(l, remove_element);
    add_element_first(l, f);

    printf("test_add_and_remove_element\t : attendu => 6, 1, 3 || obtenu => ");
    visu_liste(*l, visu_element, "\b\b \n");

    free_liste(l, remove_element);
}

void test_add_element_trie_c(liste *l)
{
    int tab[6] = {1, 2, 3, 4, 5, 6};

    add_element_trie(l, &tab[2], compar_element, true);
    add_element_trie(l, &tab[4], compar_element, true);
    add_element_trie(l, &tab[5], compar_element, true);
    add_element_trie(l, &tab[1], compar_element, true);
    add_element_trie(l, &tab[3], compar_element, true);
    add_element_trie(l, &tab[0], compar_element, true);

    printf("test_add_element_trie_c\t : attendu => 1, 2, 3, 4, 5, 6 || "
           "obtenu => ");
    visu_liste(*l, visu_element, "\b\b ");

    if(verif_pointeur(*l)){
        printf("|| verif_pointeur ok\n");
    }
    else{
        printf("|| verif_pointeur not ok\n");
    }

    free_liste(l, NULL);
}

void test_add_element_trie_d(liste *l){
    int tab[6] = {1, 2, 3, 4, 5, 6};

    add_element_trie(l, &tab[1], compar_element, false);
    add_element_trie(l, &tab[3], compar_element, false);
    add_element_trie(l, &tab[0], compar_element, false);
    add_element_trie(l, &tab[5], compar_element, false);
    add_element_trie(l, &tab[2], compar_element, false);
    add_element_trie(l, &tab[4], compar_element, false);

    printf("testAddElementTrieD\t : attendu => 6, 5, 4, 3, 2, 1 || obtenu => ");
    visu_liste(*l, visu_element, "\b\b ");

    if(verif_pointeur(*l)){
        printf("|| verif_pointeur ok\n");
    }
    else{
        printf("|| verif_pointeur not ok\n");
    }

    free_liste(l, NULL);
}

void test_remove_element(liste *l)
{
    /*int tab[6] = {1, 2, 3, 4, 5, 6};

      add_element_trie(l, &tab[0], true);
      add_element_trie(l, &tab[1], true);
      add_element_trie(l, &tab[2], true);
      add_element_trie(l, &tab[3], true);
      add_element_trie(l, &tab[4], true);
      add_element_trie(l, &tab[5], true);*/

    int *a = p_calloc(sizeof(int));
    int *b = p_calloc(sizeof(int));
    int *c = p_calloc(sizeof(int));
    int *d = p_calloc(sizeof(int));
    int *e = p_calloc(sizeof(int));
    int *f = p_calloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;
    *e = 5;
    *f = 6;

    add_element_trie(l, a, compar_element, true);
    add_element_trie(l, b, compar_element, true);
    add_element_trie(l, c, compar_element, true);
    add_element_trie(l, d, compar_element, true);
    add_element_trie(l, e, compar_element, true);
    add_element_trie(l, f, compar_element, true);

    remove_element_n(l, 2, remove_element);
    //remove_element_n(l, 3);

    printf("test1 : attendu => 1, 3, 5, 6  || obtenu => ");
    visu_liste(*l, visu_element, "\b\b \n");

    free_liste(l, remove_element);
}

void test_triage_liste(liste *l)
{
    int tab[6] = {1, 2, 3, 4, 5, 6};

    add_element_trie(l, &tab[4], compar_element, true);
    add_element_trie(l, &tab[0], compar_element, true);
    add_element_trie(l, &tab[1], compar_element, true);
    add_element_trie(l, &tab[5], compar_element, true);
    add_element_trie(l, &tab[3], compar_element, true);
    add_element_trie(l, &tab[2], compar_element, true);

    printf("testTriage : avant triage => ");
    visu_liste(*l, visu_element, "\b\b ");

    //trieListeFusion(l, true);

    printf("|| apres triage => ");
    visu_liste(*l, visu_element, "\b\b \n");

    free_liste(l, NULL);
}
