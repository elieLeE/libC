#include "test.h"

void test1(liste *l){
    int *a = my_calloc(sizeof(int));
    int *b = my_calloc(sizeof(int));
    int *c = my_calloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;

    addElementFirst(l, a);
    addElementFirst(l, b);
    addElementFirst(l, c);

    printf("test1 : attendu => 3, 2, 1 || obtenu => ");

    visuListe(*l, "\b\b \n");

    freeListe(l);
}

void test2(liste *l){
    int *a = my_calloc(sizeof(int));
    int *b = my_calloc(sizeof(int));
    int *c = my_calloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;

    addElementLast(l, a);
    addElementLast(l, b);
    addElementLast(l, c);

    printf("test1 : attendu => 1, 2, 3 || obtenu => ");
    visuListe(*l, "\b\b \n");

    freeListe(l);
}

void test3(liste *l){
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


    printf("test1 : attendu => 6, 1, 3 || obtenu => ");
    visuListe(*l, "\b\b \n");

    freeListe(l);
}

void test4(liste *l){
    int *a = my_calloc(sizeof(int));
    int *b = my_calloc(sizeof(int));

    *a = 1;
    *b = 2;

    addElementLast(l, a);
    addElementFirst(l, b);
    removeLastElement(l);

    printf("test1 : attendu => 2 || obtenu => ");
    visuListe(*l, "\b\b \n");

    freeListe(l);
}


