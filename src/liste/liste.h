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

liste creerListe(void (*visuElement)(void const *data), int (*comparElement)(void const*, void const*), void (*removeData)(void *data));
bool listeEmpty(liste *l);
bool elemEmpty(element *e);

void visuListe(liste l, char const *s);

void freeListe(liste *l);

#endif
