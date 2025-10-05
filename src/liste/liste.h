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

/* Here, head has to be a generic_elem_liste_t */
#define gl_for_each(_p, _head) \
    for (generic_elem_liste_t *_p = _head; _p != NULL; _p = _p->suiv)

void gl_init(generic_liste_t *l);

bool gl_is_empty(generic_liste_t *l);
bool gl_is_elem_empty(generic_elem_liste_t *e);

void gl_add_elem_first(generic_liste_t* l, void *e);
void gl_add_elem_last(generic_liste_t *l, void *e);
void gl_add_elem_trie(generic_liste_t *l, void *e,
                      int (*compar_elem)(void const *d1, void const *d2),
                      bool sens_croissant);
void
gl_add_elem_trie_c(generic_liste_t *l, void *e,
                      int (*compar_elem)(void const *d1, void const *d2));
void
gl_add_elem_trie_d(generic_liste_t *l, void *e,
                      int (*compar_elem)(void const *d1, void const *d2));
void gl_add_elem_next(generic_elem_liste_t *e, void *data);

void gl_remove_first_elem(generic_liste_t *l,
                             void (*remove_data)(void *data));
void gl_remove_last_elem(generic_liste_t *l,
                            void (*remove_data)(void *data));
void gl_remove_next_elem(generic_elem_liste_t *e,
                            void (*remove_data)(void *data));
void gl_remove_elem_n(generic_liste_t *l, unsigned int const n,
                         void (*remove_data)(void *data));
//void gl_remove_elem_adrr_data(generic_liste_t *l, void *d);
//void gl_remove_elem_doublons(generic_liste_t *l);
//void gl_remove_elem_val_data(generic_liste_t *l, void *data);

void gl_trie_fusion(generic_liste_t *l, bool sens_croissant);
void gl_division(generic_liste_t *l);
void gl_fusion(generic_liste_t *l, bool sens_croissant);

void gl_visu(generic_liste_t *l, void (*visuelem)(void const *data),
             char const *sep);

void gl_free(generic_liste_t *l, void (*remove_data)(void *data));

#endif
