#include <string.h>

#include "assert.h"

#include "liste.h"
#include "../mem/mem.h"
#include "../logger/logger.h"

/* TODO: this file has be coded a long time ago and needs some refactoring */

/* {{{ Adding methods */

void gl_add_elem_first(generic_liste_t *l, void *data)
{
    generic_elem_liste_t *new = p_calloc(sizeof(*new));

    new->data = data;
    new->suiv = l->first;
    new->prec = NULL;

    if (gl_is_empty(l)) {
        l->end = new;
    } else {
        l->first->prec = new;
    }

    l->first = new;
    (l->nbre_elem)++;
}

static void
gl_add_elem_next(generic_liste_t *l, generic_elem_liste_t *e, void *data)
{
    generic_elem_liste_t *new = p_calloc(sizeof(*new));

    new->data = data;
    new->suiv = e->suiv;
    new->prec = e;
    if (e->suiv != NULL) {
        e->suiv->prec = new;
    }
    e->suiv = new;

    (l->nbre_elem)++;
}

void gl_add_elem_last(generic_liste_t *l, void *data)
{
    if (gl_is_empty(l)) {
        gl_add_elem_first(l, data);
    } else {
        gl_add_elem_next(l, l->end, data);

        l->end = l->end->suiv;
    }
}

void gl_add_elem_sorted(generic_liste_t *l, void *data,
                        int (*cmp_elem)(void const *d1, void const *d2))
{
    if (cmp_elem == NULL) {
        logger_fatal("cmp_elem is NULL");
    }

    if (gl_is_empty(l)) {
        gl_add_elem_first(l, data);
    } else if (cmp_elem(l->first->data, data) >= 0) {
        gl_add_elem_first(l, data);
    } else if (cmp_elem(l->end->data, data) <= 0) {
        gl_add_elem_last(l, data);
    } else {
        generic_elem_liste_t *n;

        gl_for_each(p, l->first) {
            if (cmp_elem(p->suiv->data, data) >= 0) {
                n = p;
                break;
            }
        }
        gl_add_elem_next(l, n, data);
    }
}

/* }}} */
/* {{{ Removing methods */

int
gl_remove_first_elem(generic_liste_t *l, void (*remove_data_cb)(void *data))
{
    generic_elem_liste_t *p;

    if (gl_is_empty(l)) {
        logger_warning("can not remove first element of the list as the list "
                       "is empty");
        return -1;
    }

    p = l->first;

    l->first = l->first->suiv;

    if (l->nbre_elem == 0) {
        logger_fatal("'nbre_elem' is equal to 0 and so cannot be decreased, "
                     "but an element has just been removed");
    }
    (l->nbre_elem)--;

    /* remove_data_cb can be NULL as data can contain a data detained by
     * someone else */
    if (remove_data_cb != NULL) {
        remove_data_cb(p->data);
    }
    p_free((void **)&p);

    if (gl_is_empty(l)) {
        l->end = NULL;
    } else {
        l->first->prec = NULL;
    }

    return 0;
}

int gl_remove_last_elem(generic_liste_t *l, void (*remove_data_cb)(void *data))
{
    generic_elem_liste_t *p;

    if (gl_is_empty(l)) {
        logger_warning("can not remove first element of the list as the list "
                       "is empty");
        return -1;
    }

    p = l->end;

    l->end = l->end->prec;

    if (l->nbre_elem == 0) {
        logger_fatal("'nbre_elem' is equal to 0 and so cannot be decreased, "
                     "but an element has just been removed");
    }
    (l->nbre_elem)--;

    /* remove_data_cb can be NULL as data can contain a data detained by
     * someone else */
    if (remove_data_cb != NULL) {
        (*remove_data_cb)(p->data);
    }
    p_free((void **)&p);

    if (gl_is_empty(l)) {
        l->end = NULL;
    } else {
        l->end->suiv = NULL;
    }

    return 0;
}

static int
gl_remove_elem(generic_liste_t *l,  generic_elem_liste_t *elem_to_remove,
               void (*remove_data_cb)(void *data))
{
    if (elem_to_remove == NULL) {
        logger_error("Can not remove a NULL elem");
        assert(false);
        return -1;
    }
    if (elem_to_remove->prec == NULL) {
        logger_error("previous element is NULL whereas it should not be");
        assert(false);
        return -1;
    }
    if (elem_to_remove->suiv  == NULL) {
        logger_error("next element is NULL whereas it should not be");
        assert(false);
        return -1;
    }

    elem_to_remove->prec->suiv = elem_to_remove->suiv;
    elem_to_remove->suiv->prec = elem_to_remove->prec;

    if (l->nbre_elem == 0) {
        logger_fatal("'nbre_elem' is equal to 0 and so cannot be decreased, "
                     "but an element has just been removed");
    }
    (l->nbre_elem)--;

    /* remove_data_cb can be NULL as data can contain a data detained by
     * someone else */
    if (remove_data_cb != NULL) {
        (*remove_data_cb)(elem_to_remove->data);
    }

    p_free((void **)&elem_to_remove);

    return -1;
}

int gl_remove_elem_n(generic_liste_t *l, unsigned int const n,
                      void (*remove_data_cb)(void *data))
{
    unsigned int count = 0;

    if (n == 0) {
        return gl_remove_first_elem(l, remove_data_cb);
    }

    if (l->nbre_elem == n) {
        return gl_remove_last_elem(l, remove_data_cb);
    }

    if (l->nbre_elem < n) {
        logger_error("index %d given to 'gl_remove_elem_n' is too big, "
                     "the size of the list is %d", n, l->nbre_elem);
        return -1;
    }

    gl_for_each(p, l->first) {
        if (count == n) {
            return gl_remove_elem(l, p, remove_data_cb);
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
    memset(l, 0, sizeof(*l));
}

bool gl_is_empty(generic_liste_t *l)
{
    return (l->first == NULL);
}

bool gl_is_elem_empty(generic_elem_liste_t *e)
{
    return (e == NULL);
}

generic_elem_liste_t *
gl_get_elem_data(generic_liste_t *l, void *searching_data,
                 int (*cmp_cb)(const void *, const void *))
{
    gl_for_each(elem, l->first) {
        if (cmp_cb(elem->data, searching_data) == 0) {
            return elem;
        }
    }

    return NULL;
}

bool gl_contains_data(generic_liste_t *l, void *searching_data,
                 int (*cmp_cb)(const void *, const void *))
{
    return gl_get_elem_data(l, searching_data, cmp_cb) != NULL;
}

/* XXX: maybe this method is not necessary. TO REMOVE ? */
void gl_visu(generic_liste_t *l, void (*visuElement)(void const *data),
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

void gl_free(generic_liste_t *l, void (*remove_data_cb)(void *data))
{
    while (!gl_is_empty(l)) {
        assert(gl_remove_first_elem(l, remove_data_cb) == 0);
    }
    l->first = NULL;
    l->end = NULL;
}

