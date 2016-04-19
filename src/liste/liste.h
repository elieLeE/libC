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
struct liste{
    struct liste *suiv;
    void *data;
};

liste* creerListe();
bool listeEmpty(liste *l);
void addElementFirst(liste** l, void *e);
void addElementLast(liste *l, void* e);
void visuListe(liste *l, void (*visuElement)(void *data), char* s);
void removeFirstElement(liste **l);
void removeLastElement(liste *l);
void removeElementN(liste **l, int n);
void freeListe(liste **l);

#endif
