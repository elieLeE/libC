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

liste creer_liste(void (*visu_element)(void const *data),
                  int (*compar_element)(void const*, void const*),
                  void (*remove_data)(void *data));

bool is_liste_empty(liste *l);
bool is_elem_empty(element *e);

void add_element_first(liste* l, void *e);
void add_element_last(liste *l, void *e);
void add_element_trie(liste *l, void *e, bool sens_croissant);
void add_element_trie_c(liste *l, void *e);
void add_element_trie_d(liste *l, void *e);
void add_element_next(element *e, void *data);

void remove_data(liste const *l, void *data);
void remove_first_element(liste *l);
void remove_last_element(liste *l);
void remove_next_element(liste *l, element *e);
void remove_element_n(liste *l, unsigned int const n);
//void remove_element_adrr_data(liste *l, void *d);
//void remove_element_doublons(liste *l);
//void remove_element_val_data(liste *l, void *data);

void trie_liste_fusion(liste *l, bool sens_croissant);
void division(liste *l);
void fusion(liste *l, bool sens_croissant);

void visu_liste(liste l, char const *s);

void free_liste(liste *l);

#endif
