#ifndef _SHARED_LISTE_H_
#define _SHARED_LISTE_H_

/*
 * liste generique
 * liste doublement chainée, avec toutes les fonctions de base sur les listes.
 * la structure de base contient des pointeurs sur le début et la fin de la
 * liste, le nombre d'element dans le liste.
 */

#include <stdbool.h>

typedef enum gl_sort_algo_t {
    INSERTION_SORT,

    ALGO_MAX,  /* do not use this value */
} gl_sort_algo_t;

typedef struct generic_elem_liste_t {
    struct generic_elem_liste_t *prec, *suiv;
    void *data;
} generic_elem_liste_t;

typedef generic_elem_liste_t gl_elem_t;

typedef struct generic_liste_t {
    generic_elem_liste_t *first, *end;
    int nbre_elem;
} generic_liste_t;

/* Here, head has to be a generic_elem_liste_t */
#define gl_for_each(_p, _head)                                                \
    for (generic_elem_liste_t *_p = _head; _p != NULL; _p = _p->suiv)

#define gl_for_each_rev(_p, _head)                                            \
    for (generic_elem_liste_t *_p = _head; _p != NULL; _p = _p->prev)

#define gl_for_each_until(_p, _head, _stop)                                   \
    for (generic_elem_liste_t *_p = _head; _p != NULL && _p != _stop;\
         _p = _p->suiv)


void gl_init(generic_liste_t *l);

bool gl_is_empty(const generic_liste_t *l);
bool gl_is_elem_empty(const generic_elem_liste_t *e);

bool gl_contains_data(const generic_liste_t *l, const void *searching_data,
                      int (*cmp_cb)(const void *, const void *));
generic_elem_liste_t *
gl_get_elem_data(generic_liste_t *l, const void *searching_data,
                 int (*cmp_cb)(const void *, const void *));
generic_elem_liste_t *gl_get_elem_n(generic_liste_t *l, int n);

void gl_insert_elem_first(generic_liste_t *l, gl_elem_t *elem);
void
gl_insert_elem_next(generic_liste_t *l, gl_elem_t *prev, gl_elem_t *elem);
void gl_insert_elem_sorted(generic_liste_t *l, gl_elem_t *elem,
                           int (*cmp_data_cb)(void const *d1, void const *d2));

gl_elem_t *gl_add_elem_first(generic_liste_t* l, void *e);
gl_elem_t *gl_add_elem_last(generic_liste_t *l, void *e);
gl_elem_t *
gl_add_elem_next(generic_liste_t *l, generic_elem_liste_t *e, void *data);
/* Add a new element and sorted it.
 * Warning; list has to be already sorted. If it is not, the position of the
 * new element will be undetermined.
 * You chose the way of the sorting. If 'cmp_cb' works like usual
 * (returns > 0 if d1 > d2, < 0 if d2 < d1 and 0 else), then the list will be
 * sorted increasing.
 * If you want to sort it decreasing, just return opposite value in 'cmp_cb'
 */
gl_elem_t *
gl_add_elem_sorted(generic_liste_t *l, void *e,
                   int (*cmp_data_cb)(void const *d1, void const *d2));

/* Warning: these methods remove elements from the list and returns it (except
 * 'gl_remove_elem' that just removes the elements). So, these elements are
 * not freed ! The calling  method becomes responsible of theirs memory. */
gl_elem_t *gl_take_first_elem(generic_liste_t *l);
gl_elem_t *gl_take_last_elem(generic_liste_t *l);
int gl_remove_elem(generic_liste_t *l, gl_elem_t *elem);
gl_elem_t *gl_take_elem_n(generic_liste_t *l, int const n);

/* The following methods removes and free the elements. */
int gl_delete_first_elem(generic_liste_t *l,
                         void (*remove_data)(void *data));
int gl_delete_last_elem(generic_liste_t *l,
                        void (*remove_data)(void *data));
int gl_delete_elem(generic_liste_t *l,  generic_elem_liste_t *elem_to_remove,
                   void (*remove_data_cb)(void *data));
int gl_delete_elem_n(generic_liste_t *l, int const n,
                     void (*remove_data)(void *data));

//void gl_remove_elem_doublons(generic_liste_t *l);

int gl_sort(generic_liste_t *l, gl_sort_algo_t algo,
            int (*cmp_data_cb)(void const *d1, void const *d2));

void gl_visu(const generic_liste_t *l, void (*visuelem)(void const *data),
             char const *sep);

void gl_free(generic_liste_t *l, void (*free_data_cb)(void *data));

#endif
