#ifndef _SHARED_LISTE_H_
#define _SHARED_LISTE_H_

/*
 * liste generique
 * liste doublement chainée, avec toutes les fonctions de base sur les listes.
 * la structure de base contient des pointeurs sur le début et la fin de la
 * liste, le nombre d'element dans le liste.
 */

#include <stdbool.h>
#include "type.h"

generic_liste_t creer_liste(void);

bool is_liste_empty(generic_liste_t *l);
bool is_elem_empty(generic_elem_liste_t *e);

void add_element_first(generic_liste_t* l, void *e);
void add_element_last(generic_liste_t *l, void *e);
void add_element_trie(generic_liste_t *l, void *e,
                      int (*compar_element)(void const *d1, void const *d2),
                      bool sens_croissant);
void add_element_trie_c(generic_liste_t *l, void *e,
                        int (*compar_element)(void const *d1, void const *d2));
void add_element_trie_d(generic_liste_t *l, void *e,
                        int (*compar_element)(void const *d1, void const *d2));
void add_element_next(generic_elem_liste_t *e, void *data);

void remove_first_element(generic_liste_t *l, void (*remove_data)(void *data));
void remove_last_element(generic_liste_t *l, void (*remove_data)(void *data));
void remove_next_element(generic_elem_liste_t *e,
                         void (*remove_data)(void *data));
void remove_element_n(generic_liste_t *l, unsigned int const n,
                      void (*remove_data)(void *data));
//void remove_element_adrr_data(generic_liste_t *l, void *d);
//void remove_element_doublons(generic_liste_t *l);
//void remove_element_val_data(generic_liste_t *l, void *data);

void trie_liste_fusion(generic_liste_t *l, bool sens_croissant);
void division(generic_liste_t *l);
void fusion(generic_liste_t *l, bool sens_croissant);

void visu_liste(generic_liste_t l, void (*visuElement)(void const *data),
                char const *sep);

void free_liste(generic_liste_t *l, void (*remove_data)(void *data));

#endif
