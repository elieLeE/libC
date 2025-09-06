#include "test.h"

void testAddAndRemoveElement(liste *l){
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

    addElementLast(l, a);
    addElementFirst(l, b);
    addElementLast(l, c);
    removeFirstElement(l);
    addElementLast(l, d);
    addElementFirst(l, e);
    removeLastElement(l);
    removeFirstElement(l);
    addElementFirst(l, f);

    printf("testAddAndRemoveElement\t : attendu => 6, 1, 3 || obtenu => ");
    visuListe(*l, "\b\b \n");

    freeListe(l);
}

void testAddElementTrieC(liste *l){
    int tab[6] = {1, 2, 3, 4, 5, 6};

    addElementTrie(l, &tab[2], true);
    addElementTrie(l, &tab[4], true);
    addElementTrie(l, &tab[5], true);
    addElementTrie(l, &tab[1], true);
    addElementTrie(l, &tab[3], true);
    addElementTrie(l, &tab[0], true);

    printf("testAddElementTrieC\t : attendu => 1, 2, 3, 4, 5, 6 || obtenu => ");
    visuListe(*l, "\b\b ");

    if(verifPointeur(*l)){
        printf("|| verifPointeur ok\n");
    }
    else{
        printf("|| verifPointeur not ok\n");
    }

    freeListe(l);
}

void testAddElementTrieD(liste *l){
    int tab[6] = {1, 2, 3, 4, 5, 6};

    addElementTrie(l, &tab[1], false);
    addElementTrie(l, &tab[3], false);
    addElementTrie(l, &tab[0], false);
    addElementTrie(l, &tab[5], false);
    addElementTrie(l, &tab[2], false);
    addElementTrie(l, &tab[4], false);

    printf("testAddElementTrieD\t : attendu => 6, 5, 4, 3, 2, 1 || obtenu => ");
    visuListe(*l, "\b\b ");

    if(verifPointeur(*l)){
        printf("|| verifPointeur ok\n");
    }
    else{
        printf("|| verifPointeur not ok\n");
    }

    freeListe(l);
}

void testRemoveElement(liste *l){
    /*int tab[6] = {1, 2, 3, 4, 5, 6};

      addElementTrie(l, &tab[0], true);
      addElementTrie(l, &tab[1], true);
      addElementTrie(l, &tab[2], true);
      addElementTrie(l, &tab[3], true);
      addElementTrie(l, &tab[4], true);
      addElementTrie(l, &tab[5], true);*/

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

    addElementTrie(l, a, true);
    addElementTrie(l, b, true);
    addElementTrie(l, c, true);
    addElementTrie(l, d, true);
    addElementTrie(l, e, true);
    addElementTrie(l, f, true);

    removeElementN(l, 2);
    //removeElementN(l, 3);

    printf("test1 : attendu => 1, 3, 5, 6  || obtenu => ");
    visuListe(*l, "\b\b \n");

    freeListe(l);
}
void testTriageListe(liste *l){
    int tab[6] = {1, 2, 3, 4, 5, 6};

    addElementTrie(l, &tab[4], true);
    addElementTrie(l, &tab[0], true);
    addElementTrie(l, &tab[1], true);
    addElementTrie(l, &tab[5], true);
    addElementTrie(l, &tab[3], true);
    addElementTrie(l, &tab[2], true);

    printf("testTriage : avant triage => ");
    visuListe(*l, "\b\b ");

    //trieListeFusion(l, true);

    printf("|| apres triage => ");
    visuListe(*l, "\b\b \n");

    freeListe(l);
}
