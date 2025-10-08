#include <string.h>

#include "liste.h"
#include "../mem/mem.h"

/* TODO: this file has be coded a long time ago and needs some refactoring */

/* {{{ Adding methods */

void gl_add_elem_first(generic_liste_t *l, void *data)
{
    generic_elem_liste_t *new = p_calloc(sizeof(*new));

    new->data = data;
    new->suiv = l->first;
    new->prec = NULL;

    if (!gl_is_empty(l)) {
        l->first->prec = new;
    } else {
        l->end = new;
    }

    l->first = new;
    (l->nbre_elem)++;
}

void gl_add_elem_last(generic_liste_t *l, void *data)
{
    generic_elem_liste_t *new = p_calloc(sizeof(*new));

    new->data = data;
    new->suiv = NULL;
    new->prec = l->end;

    if (!gl_is_empty(l)) {
        l->end->suiv = new;
    } else {
        l->first = new;
    }

    l->end = new;
    (l->nbre_elem)++;
}

void gl_add_elem_trie(generic_liste_t *l, void *data,
                      int (*compar_elem)(void const *d1, void const *d2),
                      bool sens_croissant)
{
    if (!(compar_elem == NULL)) {
        if (gl_is_empty(l)) {
            gl_add_elem_first(l, data);
        } else {
            if (sens_croissant) {
                gl_add_elem_trie_c(l, data, compar_elem);
            } else {
                gl_add_elem_trie_d(l, data, compar_elem);
            }
        }
    } else {
        fprintf(stderr, "compar_elem NULL\n");
    }
}

void gl_add_elem_trie_c(generic_liste_t *l, void *data,
                        int (*compar_elem)(void const *d1, void const *d2))
{
    if ((*(compar_elem))(l->first->data, data) >= 0) {
        gl_add_elem_first(l, data);
    } else if ((*(compar_elem))(l->end->data, data) <= 0) {
        gl_add_elem_last(l, data);
    } else {
        generic_elem_liste_t *p = l->first;

        if ((*(compar_elem))(data, l->first->data) > 0) {
            while ((!gl_is_elem_empty(p->suiv)) &&
                  ((*(compar_elem))(p->suiv->data, data) < 0))
            {
                p = p->suiv;
            }
        }
        gl_add_elem_next(p, data);
        (l->nbre_elem)++;
    }
}

void gl_add_elem_next(generic_elem_liste_t *e, void *data)
{
    generic_elem_liste_t *new = p_calloc(sizeof(*new));

    new->data = data;
    new->suiv = e->suiv;
    new->prec = e;
    e->suiv->prec = new;
    e->suiv = new;
}

void gl_add_elem_trie_d(generic_liste_t *l, void *data,
                        int (*compar_elem)(void const *d1, void const *d2))
{
    if ((*(compar_elem))(l->first->data, data) <= 0) {
        gl_add_elem_first(l, data);
    } else if ((*(compar_elem))(l->end->data, data) >= 0) {
        gl_add_elem_last(l, data);
    } else {
        generic_elem_liste_t *p = l->first;

        if ((*(compar_elem))(data, l->first->data) < 0) {
            while ((!gl_is_elem_empty(p->suiv)) &&
                  ((*(compar_elem))(p->suiv->data, data) > 0))
            {
                p = p->suiv;
            }
        }
        gl_add_elem_next(p, data);
        (l->nbre_elem)++;
    }
}

/* }}} */
/* {{{ Removing methods */

void gl_remove_first_elem(generic_liste_t *l, void (*remove_data)(void *data))
{
    if (!gl_is_empty(l)) {
        generic_elem_liste_t *p = l->first;

        l->first = l->first->suiv;
        (l->nbre_elem)--;
        /* remove_data can be NULL as data can contain a data detained by
         * someone else */
        if (remove_data != NULL) {
            (*remove_data)(p->data);
        }
        p_free((void **)&p);

        if (!gl_is_empty(l)) {
            l->first->prec = NULL;
        }
    }
}

void gl_remove_last_elem(generic_liste_t *l, void (*remove_data)(void *data))
{
    if (!gl_is_empty(l)) {
        generic_elem_liste_t *p = l->end;

        l->end = l->end->prec;
        (l->nbre_elem)--;
        /* remove_data can be NULL as data can contain a data detained by
         * someone else */
        if (remove_data != NULL) {
            (*remove_data)(p->data);
        }
        p_free((void **)&p);

        if (!gl_is_empty(l)) {
            l->end->suiv = NULL;
        }
    }
}

void gl_remove_elem_n(generic_liste_t *l, unsigned int const n,
                      void (*remove_data)(void *data))
{
    printf("removeElementN NOT YET FONCTIONNAL\n");
    return;

    if (((int)n > 0) && (n <= l->nbre_elem)) {
        if (n == 1) {
            gl_remove_first_elem(l, remove_data);
        } else if (n == l->nbre_elem) {
            gl_remove_last_elem(l, remove_data);
        } else {
            generic_elem_liste_t *p = NULL;
            unsigned int compt = 1;

            printf("suppresion milieu : %d\n", n);

            p = l->first;
            while (compt < n - 1) {
                p = p->suiv;
            }
            gl_remove_next_elem(p, remove_data);
        }
    }
    printf("\n");
}

void
gl_remove_next_elem(generic_elem_liste_t *e, void (*remove_data)(void *data))
{
    printf("gl_remove_next_elem NOT YET FONCTIONNAL\n");
    return;

    generic_elem_liste_t *p = e->suiv;
    e->suiv = e->suiv->suiv;
    e->suiv->prec = e;
    /* remove_data can be NULL as data can contain a data detained by
     * someone else */
    if (remove_data != NULL) {
        (*remove_data)(p->data);
    }
    p_free((void **)&p);
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
    return (l->first==NULL);
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

void gl_free(generic_liste_t *l, void (*remove_data)(void *data))
{
    while (!gl_is_empty(l)) {
        gl_remove_first_elem(l, remove_data);
    }
    l->first = NULL;
    l->end = NULL;
}

