#include "test.h"
#include "../../src/mem/alloc.h"
#include "verif.h"

void test_add_and_remove_element(liste *l)
{
    int *a = my_calloc(sizeof(int));
    int *b = my_calloc(sizeof(int));
    int *c = my_calloc(sizeof(int));
    int *d = my_calloc(sizeof(int));
    int *e = my_calloc(sizeof(int));
    int *f = my_calloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;
    *e = 5;
    *f = 6;

    add_element_last(l, a);
    add_element_first(l, b);
    add_element_last(l, c);
    remove_first_element(l);
    add_element_last(l, d);
    add_element_first(l, e);
    remove_last_element(l);
    remove_first_element(l);
    add_element_first(l, f);

    printf("test_add_and_remove_element\t : attendu => 6, 1, 3 || obtenu => ");
    visu_liste(*l, "\b\b \n");

    free_liste(l);
}

void test_add_element_trie_c(liste *l)
{
    int tab[6] = {1, 2, 3, 4, 5, 6};

    add_element_trie(l, &tab[2], true);
    add_element_trie(l, &tab[4], true);
    add_element_trie(l, &tab[5], true);
    add_element_trie(l, &tab[1], true);
    add_element_trie(l, &tab[3], true);
    add_element_trie(l, &tab[0], true);

    printf("test_add_element_trie_c\t : attendu => 1, 2, 3, 4, 5, 6 || "
           "obtenu => ");
    visu_liste(*l, "\b\b ");

    if(verif_pointeur(*l)){
        printf("|| verif_pointeur ok\n");
    }
    else{
        printf("|| verif_pointeur not ok\n");
    }

    free_liste(l);
}

void test_add_element_trie_d(liste *l){
    int tab[6] = {1, 2, 3, 4, 5, 6};

    add_element_trie(l, &tab[1], false);
    add_element_trie(l, &tab[3], false);
    add_element_trie(l, &tab[0], false);
    add_element_trie(l, &tab[5], false);
    add_element_trie(l, &tab[2], false);
    add_element_trie(l, &tab[4], false);

    printf("testAddElementTrieD\t : attendu => 6, 5, 4, 3, 2, 1 || obtenu => ");
    visu_liste(*l, "\b\b ");

    if(verif_pointeur(*l)){
        printf("|| verif_pointeur ok\n");
    }
    else{
        printf("|| verif_pointeur not ok\n");
    }

    free_liste(l);
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

    int *a = my_calloc(sizeof(int));
    int *b = my_calloc(sizeof(int));
    int *c = my_calloc(sizeof(int));
    int *d = my_calloc(sizeof(int));
    int *e = my_calloc(sizeof(int));
    int *f = my_calloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;
    *e = 5;
    *f = 6;

    add_element_trie(l, a, true);
    add_element_trie(l, b, true);
    add_element_trie(l, c, true);
    add_element_trie(l, d, true);
    add_element_trie(l, e, true);
    add_element_trie(l, f, true);

    remove_element_n(l, 2);
    //remove_element_n(l, 3);

    printf("test1 : attendu => 1, 3, 5, 6  || obtenu => ");
    visu_liste(*l, "\b\b \n");

    free_liste(l);
}
void test_triage_liste(liste *l)
{
    int tab[6] = {1, 2, 3, 4, 5, 6};

    add_element_trie(l, &tab[4], true);
    add_element_trie(l, &tab[0], true);
    add_element_trie(l, &tab[1], true);
    add_element_trie(l, &tab[5], true);
    add_element_trie(l, &tab[3], true);
    add_element_trie(l, &tab[2], true);

    printf("testTriage : avant triage => ");
    visu_liste(*l, "\b\b ");

    //trieListeFusion(l, true);

    printf("|| apres triage => ");
    visu_liste(*l, "\b\b \n");

    free_liste(l);
}
