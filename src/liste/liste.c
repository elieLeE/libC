#include <string.h>

#include "assert.h"

#include "liste.h"
#include "../mem/mem.h"
#include "../logger/logger.h"
#include "../macros.h"

/* TODO: this file has be coded a long time ago and needs some refactoring */

/* {{{ Inserting methods */

/* Methods here do not create elements. They just insert them */

void gl_insert_elem_first(generic_liste_t *l, gl_elem_t *elem)
{
    elem->suiv = l->first;
    elem->prec = NULL;

    if (gl_is_empty(l)) {
        l->end = elem;
    } else {
        l->first->prec = elem;
    }

    l->first = elem;
    (l->nbre_elem)++;
}

void gl_insert_elem_next(generic_liste_t *l, gl_elem_t *prev, gl_elem_t *elem)
{
    if (gl_is_empty(l)) {
        gl_insert_elem_first(l, elem);
        return;
    }

    elem->suiv = prev->suiv;
    elem->prec = prev;

    if (prev->suiv != NULL) {
        prev->suiv->prec = elem;
    }
    prev->suiv = elem;

    if (l->end == prev) {
        l->end = elem;
    }

    (l->nbre_elem)++;
}

void gl_insert_elem_sorted(generic_liste_t *l, gl_elem_t *elem,
                           int (*cmp_data_cb)(void const *d1, void const *d2))
{
    if (cmp_data_cb == NULL) {
        logger_fatal("cmp_data_cb is NULL");
    }

    if (gl_is_empty(l)) {
        gl_insert_elem_first(l, elem);
    } else if (cmp_data_cb(l->first->data, elem->data) >= 0) {
        gl_insert_elem_first(l, elem);
    } else if (cmp_data_cb(l->end->data, elem->data) <= 0) {
        gl_insert_elem_next(l, l->end, elem);
    } else {
        generic_elem_liste_t *next_elem;

        gl_for_each(p, l->first) {
            if (cmp_data_cb(p->data, elem->data) >= 0) {
                next_elem = p;
                break;
            }
        }
        gl_insert_elem_next(l, next_elem->prec, elem);
    }
}

/* }}} */
/* {{{ Adding methods */

/* Methods here create elements and insert them by calling insert methods */

static gl_elem_t *create_new_elem(void *data)
{
    gl_elem_t *elem = p_calloc(sizeof(*elem));

    elem->data = data;

    return elem;
}

void gl_add_elem_first(generic_liste_t *l, void *data)
{
    generic_elem_liste_t *new;

    new = create_new_elem(data);
    gl_insert_elem_first(l, new);
}

static void
gl_add_elem_next(generic_liste_t *l, generic_elem_liste_t *e, void *data)
{
    generic_elem_liste_t *new;

    new = create_new_elem(data);
    gl_insert_elem_next(l, e, new);
}

void gl_add_elem_last(generic_liste_t *l, void *data)
{
    if (gl_is_empty(l)) {
        gl_add_elem_first(l, data);
    } else {
        gl_add_elem_next(l, l->end, data);
    }
}

void
gl_add_elem_sorted(generic_liste_t *l, void *data,
                   int (*cmp_data_cb)(void const *d1, void const *d2))
{
    gl_elem_t *new;

    new = create_new_elem(data);
    gl_insert_elem_sorted(l, new, cmp_data_cb);
}

/* }}} */
/* {{{ Getting methods */

/* Methods here find an element from the list and return it but without
 * removing it from the list. So, the element will be still present in it.
 */

generic_elem_liste_t *gl_get_elem_n(generic_liste_t *l, int n)
{
    int count = 0;

    if (n < 0) {
        logger_error("index got in 'gl_get_elem_n' is below than 0: %d", n);
        return NULL;
    }

    if (l->nbre_elem < n) {
        logger_error("index got (%d) in 'gl_get_elem_n' is greater length "
                     "of the list (%d)", n, l->nbre_elem);
        return NULL;
    }

    gl_for_each(p, l->first) {
        if (count == n) {
            return p;
        }
        count++;
    }

    logger_error("element at the position %d has not been found", n);

    return NULL;
}

generic_elem_liste_t *
gl_get_elem_data(generic_liste_t *l, const void *searching_data,
                 int (*cmp_cb)(const void *, const void *))
{
    gl_for_each(elem, l->first) {
        if (cmp_cb(elem->data, searching_data) == 0) {
            return elem;
        }
    }

    return NULL;
}

/* }}} */
/* {{{ Taking methods */

/* Methods here remove element from the list but without delete them.
 * So, by calling them, user becomes responsible of the memory of the element
 */

gl_elem_t *gl_take_first_elem(generic_liste_t *l)
{
    gl_elem_t *elem;

    if (gl_is_empty(l)) {
        logger_warning("can not remove first element of the list as the list "
                       "is empty");
        return NULL;
    }

    elem = l->first;

    l->first = l->first->suiv;

    if (l->nbre_elem == 0) {
        logger_fatal("'nbre_elem' is equal to 0 and so cannot be decreased, "
                     "but an element has just been removed");
    }
    (l->nbre_elem)--;

    if (gl_is_empty(l)) {
        l->end = NULL;
    } else {
        l->first->prec = NULL;
    }

    return elem;
}

gl_elem_t *gl_take_last_elem(generic_liste_t *l)
{
    generic_elem_liste_t *elem;

    if (gl_is_empty(l)) {
        logger_warning("can not remove first element of the list as the list "
                       "is empty");
        return NULL;
    }

    elem = l->end;

    l->end = l->end->prec;

    if (l->nbre_elem == 0) {
        logger_fatal("'nbre_elem' is equal to 0 and so cannot be decreased, "
                     "but an element has just been removed");
    }
    (l->nbre_elem)--;

    if (gl_is_empty(l)) {
        l->end = NULL;
    } else {
        l->end->suiv = NULL;
    }

    return elem;
}

int gl_remove_elem(generic_liste_t *l, gl_elem_t *elem)
{
    if (elem == NULL) {
        logger_error("Can not remove a NULL elem");
        assert(false);
        return -1;
    }

    if (elem == l->first) {
        return gl_take_first_elem(l) == NULL ? -1 : 0;
    }

    if (elem == l->end) {
        return gl_take_last_elem(l) == NULL ? -1 : 0;
    }

    if (elem == NULL) {
        logger_error("previous element is NULL whereas it should not be");
        assert(false);
        return -1;
    }
    if (elem  == NULL) {
        logger_error("next element is NULL whereas it should not be");
        assert(false);
        return -1;
    }

    elem->prec->suiv = elem->suiv;
    elem->suiv->prec = elem->prec;

    if (l->nbre_elem == 0) {
        logger_fatal("'nbre_elem' is equal to 0 and so cannot be decreased, "
                     "but an element has just been removed");
    }
    (l->nbre_elem)--;

    return 0;
}

/* }}} */
/* {{{ Deleting methods */

/* Methods here deleting element from the list. */

int
gl_delete_first_elem(generic_liste_t *l, void (*remove_data_cb)(void *data))
{
    generic_elem_liste_t *p;

    if (gl_is_empty(l)) {
        logger_warning("can not remove first element of the list as the list "
                       "is empty");
        return -1;
    }

int gl_delete_first_elem(generic_liste_t *l, void (*remove_data_cb)(void *data))
{
    gl_elem_t *elem;

    elem = RETHROW_PN(gl_take_first_elem(l));

    /* remove_data_cb can be NULL as data can contain a data detained by
     * someone else */
    if (remove_data_cb != NULL) {
        remove_data_cb (elem->data);
    }
    p_free((void **)&elem);

    return 0;
}

int gl_delete_last_elem(generic_liste_t *l, void (*remove_data_cb)(void *data))
{
    generic_elem_liste_t *p;

    p = RETHROW_PN(gl_take_last_elem(l));

    /* remove_data_cb can be NULL as data can contain a data detained by
     * someone else */
    if (remove_data_cb != NULL) {
        (*remove_data_cb)(p->data);
    }
    p_free((void **)&p);

    return 0;
}

int gl_delete_elem(generic_liste_t *l, generic_elem_liste_t *elem_to_remove,
                   void (*remove_data_cb)(void *data))
{
    RETHROW(gl_remove_elem(l, elem_to_remove));

    /* remove_data_cb can be NULL as data can contain a data detained by
     * someone else */
    if (remove_data_cb != NULL) {
        (*remove_data_cb)(elem_to_remove->data);
    }

    p_free((void **)&elem_to_remove);

    return 0;
}

int gl_delete_elem_n(generic_liste_t *l, int const n,
                      void (*remove_data_cb)(void *data))
{
    int count = 0;

    if (l->nbre_elem < n) {
        logger_error("index %d given to 'gl_remove_elem_n' is too big, "
                     "the size of the list is %d", n, l->nbre_elem);
        return -1;
    }

    gl_for_each(p, l->first) {
        if (count == n) {
            return gl_delete_elem(l, p, remove_data_cb);
        }
        count++;
    }

    logger_error("element at the position %d has not been found", n);
    return -1;
}

/* }}} */
/* {{{ Trie methods */

/*void gl_trie_fusion(generic_liste_t *l, bool sensCroissant)
{}

void gl_division(generic_liste_t *l)
{}

void gl_fusion(generic_liste_t *l, bool sensCroissant)
{}
*/

/* }}} */

void gl_init(generic_liste_t *l)
{
    p_clear(l, 1);
}

bool gl_is_empty(const generic_liste_t *l)
{
    return (l->first == NULL);
}

bool gl_is_elem_empty(const generic_elem_liste_t *e)
{
    return (e == NULL);
}

bool gl_contains_data(generic_liste_t *l, const void *searching_data,
                 int (*cmp_cb)(const void *, const void *))
{
    return gl_get_elem_data(l, searching_data, cmp_cb) != NULL;
}

/* XXX: maybe this method is not necessary. TO REMOVE ? */
void gl_visu(const generic_liste_t *l, void (*visuElement)(void const *data),
             char const *sep)
{
    if (gl_is_empty(l)) {
        return;
    }
    if (visuElement != NULL) {
        gl_for_each(elem, l->first) {
            (*(visuElement))(elem->data);
            printf("%s", sep);
        }
    } else {
        fprintf(stderr,
                "impossible de visuliser la liste, visuElement NULL\n");
    }
}

void gl_free(generic_liste_t *l, void (*free_data_cb)(void *data))
{
    while (!gl_is_empty(l)) {
        assert(gl_delete_first_elem(l, free_data_cb) == 0);
    }
    l->first = NULL;
    l->end = NULL;
}

