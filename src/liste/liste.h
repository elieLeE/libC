#ifndef _SHARED_LISTE_H_
#define _SHARED_LISTE_H_

/*
 * liste generique
 * */

#define DEBUG 0

#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include "../mem/alloc.h"

typedef struct liste liste;
typedef struct element element;

struct element{
    struct element *prec, *suiv;
    void *data;
};

struct liste{
    struct element *first, *end;
    int nbreElement;

    void (*visuElement)(void *data);
    void (*removeData)(void *data);
};

liste creerListe(void (*visuElement)(void *data), void (*removeData)(void *data));
bool listeEmpty(element *e);
void addElementFirst(liste* l, void *e);
void addElementLast(liste *l, void* e);
void visuListe(liste l, char *s);
void removeFirstElement(liste *l);
void removeLastElement(liste *l);
//void removeElementN(liste *l, int n);
void freeListe(liste *l);

#endif
