#include "test.h"
#include "../../src/mem/mem.h"
#include "verif.h"

void visu_liste(generic_liste_t *l)
{
    generic_elem_liste_t *elem;

    gl_for_each(elem, l->first) {
        printf("%d, ", *(int *)(elem->data));
    }
}

void remove_element(void *data)
{
    p_free(&data);
}

int compar_element(void const *d1, void const *d2)
{
    int const *a = d1;
    int const *b = d2;

    return (*a - *b);
}

void test_add_and_remove_element(generic_liste_t *l)
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

    gl_add_elem_last(l, a);
    gl_add_elem_first(l, b);
    gl_add_elem_last(l, c);
    gl_remove_first_elem(l, remove_element);
    gl_add_elem_last(l, d);
    gl_add_elem_first(l, e);
    gl_remove_last_elem(l, remove_element);
    gl_remove_first_elem(l, remove_element);
    gl_add_elem_first(l, f);

    printf("test_add_and_remove_element\t : attendu => 6, 1, 3 || obtenu => ");
    visu_liste(l);
    printf("\n");

    gl_free(l, remove_element);
}

void test_add_element_trie_c(generic_liste_t *l)
{
    int tab[6] = {1, 2, 3, 4, 5, 6};

    gl_add_elem_trie(l, &tab[2], compar_element, true);
    gl_add_elem_trie(l, &tab[4], compar_element, true);
    gl_add_elem_trie(l, &tab[5], compar_element, true);
    gl_add_elem_trie(l, &tab[1], compar_element, true);
    gl_add_elem_trie(l, &tab[3], compar_element, true);
    gl_add_elem_trie(l, &tab[0], compar_element, true);

    printf("test_add_element_trie_c\t : attendu => 1, 2, 3, 4, 5, 6 || "
           "obtenu => ");
    visu_liste(l);

    if(verif_pointeur(*l)){
        printf("|| verif_pointeur ok\n");
    }
    else{
        printf("|| verif_pointeur not ok\n");
    }

    gl_free(l, NULL);
}

void test_add_element_trie_d(generic_liste_t *l)
{
    int tab[6] = {1, 2, 3, 4, 5, 6};

    gl_add_elem_trie(l, &tab[1], compar_element, false);
    gl_add_elem_trie(l, &tab[3], compar_element, false);
    gl_add_elem_trie(l, &tab[0], compar_element, false);
    gl_add_elem_trie(l, &tab[5], compar_element, false);
    gl_add_elem_trie(l, &tab[2], compar_element, false);
    gl_add_elem_trie(l, &tab[4], compar_element, false);

    printf("testAddElementTrieD\t : attendu => 6, 5, 4, 3, 2, 1 || obtenu => ");
    visu_liste(l);

    if(verif_pointeur(*l)){
        printf("|| verif_pointeur ok\n");
    }
    else{
        printf("|| verif_pointeur not ok\n");
    }

    gl_free(l, NULL);
}

void test_remove_element(generic_liste_t *l)
{
    /*int tab[6] = {1, 2, 3, 4, 5, 6};

      gl_add_elem_trie(l, &tab[0], true);
      gl_add_elem_trie(l, &tab[1], true);
      gl_add_elem_trie(l, &tab[2], true);
      gl_add_elem_trie(l, &tab[3], true);
      gl_add_elem_trie(l, &tab[4], true);
      gl_add_elem_trie(l, &tab[5], true);*/

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

    gl_add_elem_trie(l, a, compar_element, true);
    gl_add_elem_trie(l, b, compar_element, true);
    gl_add_elem_trie(l, c, compar_element, true);
    gl_add_elem_trie(l, d, compar_element, true);
    gl_add_elem_trie(l, e, compar_element, true);
    gl_add_elem_trie(l, f, compar_element, true);

    gl_remove_elem_n(l, 2, remove_element);
    //gl_remove_elem_n(l, 3);

    printf("test1 : attendu => 1, 3, 5, 6  || obtenu => ");
    visu_liste(l);

    gl_free(l, remove_element);
}

void test_triage_liste(generic_liste_t *l)
{
    int tab[6] = {1, 2, 3, 4, 5, 6};

    gl_add_elem_trie(l, &tab[4], compar_element, true);
    gl_add_elem_trie(l, &tab[0], compar_element, true);
    gl_add_elem_trie(l, &tab[1], compar_element, true);
    gl_add_elem_trie(l, &tab[5], compar_element, true);
    gl_add_elem_trie(l, &tab[3], compar_element, true);
    gl_add_elem_trie(l, &tab[2], compar_element, true);

    printf("testTriage : avant triage => ");
    visu_liste(l);

    //trieListeFusion(l, true);

    printf("|| apres triage => ");
    visu_liste(l);

    gl_free(l, NULL);
}
