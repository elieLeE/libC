#ifndef _SHARED_LISTE_H_
#define _SHARED_LISTE_H_

/*
 * liste generique
 * liste doublement chainée, avec toutes les fonctions de base sur les listes.
 * la structure de base contient des pointeurs sur le début et la fin de la liste, 
 * le nombre d'element dans le liste ainsi que des pointeurs sur les fonctions de visualisation des données,
 * de comparaison entre deux données et de liberation d'une donnée.
 * */

#include <stdbool.h>
#include "type.h"

liste creerListe(void (*visuElement)(void const *data),
                 int (*comparElement)(void const*, void const*),
                 void (*removeData)(void *data));

bool listeEmpty(liste *l);
bool elemEmpty(element *e);

void addElementFirst(liste* l, void *e);
void addElementLast(liste *l, void *e);
void addElementTrie(liste *l, void *e, bool sensCroissant);
void addElementTrieC(liste *l, void *e);
void addElementTrieD(liste *l, void *e);
void addElementNext(element *e, void *data);

void removeData(liste const *l, void *data);
void removeFirstElement(liste *l);
void removeLastElement(liste *l);
void removeNextElement(liste *l, element *e);
void removeElementN(liste *l, unsigned int const n);
//void removeElementAdrrData(liste *l, void *d);
//void removeElementDoublons(liste *l);
//void removeElementValData(liste *l, void *data);

void trieListeFusion(liste *l, bool sensCroissant);
void division(liste *l);
void fusion(liste *l, bool sensCroissant);

void visuListe(liste l, char const *s);

void freeListe(liste *l);

#endif
