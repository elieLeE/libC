#include "liste.h"
#include "../mem/alloc.h"

/* {{{ Adding methods */

void add_element_first(liste* l, void *data)
{
    element* new = p_calloc(sizeof(*new));

    new->data = data;
    new->suiv = l->first;
    new->prec = NULL;

    if(!is_liste_empty(l)) {
        l->first->prec = new;
    } else {
        l->end = new;
    }

    l->first = new;
    (l->nbreElement)++;
}

void add_element_last(liste *l, void *data)
{
    element* new = p_calloc(sizeof(*new));

    new->data = data;
    new->suiv = NULL;
    new->prec = l->end;

    if(!is_liste_empty(l)) {
        l->end->suiv = new;
    } else {
        l->first = new;
    }

    l->end = new;
    (l->nbreElement)++;
}

void add_element_trie(liste *l, void *data,
                      int (*compar_element)(void const *d1, void const *d2),
                      bool sens_croissant)
{
    if(!(compar_element == NULL)) {
        if(is_liste_empty(l)) {
            add_element_first(l, data);
        } else {
            if(sens_croissant) {
                add_element_trie_c(l, data, compar_element);
            } else {
                add_element_trie_d(l, data, compar_element);
            }
        }
    } else {
        fprintf(stderr, "compar_element NULL\n");
    }
}

void add_element_trie_c(liste *l, void *data,
                        int (*compar_element)(void const *d1, void const *d2))
{
    if((*(compar_element))(l->first->data, data) >= 0) {
        add_element_first(l, data);
    } else if((*(compar_element))(l->end->data, data) <= 0) {
        add_element_last(l, data);
    } else {
        element *p = l->first;

        if((*(compar_element))(data, l->first->data) > 0) {
            while((!is_elem_empty(p->suiv)) &&
                  ((*(compar_element))(p->suiv->data, data) < 0))
            {
                p = p->suiv;
            }
        }
        add_element_next(p, data);
        (l->nbreElement)++;
    }
}

void add_element_next(element *e, void *data)
{
    element *new = p_calloc(sizeof(*new));

    new->data = data;
    new->suiv = e->suiv;
    new->prec = e;
    e->suiv->prec = new;
    e->suiv = new;
}

void add_element_trie_d(liste *l, void *data,
                        int (*compar_element)(void const *d1, void const *d2))
{
    if((*(compar_element))(l->first->data, data) <= 0) {
        add_element_first(l, data);
    } else if((*(compar_element))(l->end->data, data) >= 0) {
        add_element_last(l, data);
    } else {
        element *p = l->first;

        if((*(compar_element))(data, l->first->data) < 0) {
            while((!is_elem_empty(p->suiv)) &&
                  ((*(compar_element))(p->suiv->data, data) > 0))
            {
                p = p->suiv;
            }
        }
        add_element_next(p, data);
        (l->nbreElement)++;
    }
}

/* }}} */
/* {{{ Removing methods */

void remove_first_element(liste *l, void (*remove_data)(void *data))
{
    if(!is_liste_empty(l)) {
        element *p = l->first;

        l->first = l->first->suiv;
        (l->nbreElement)--;
        /* remove_data can be NULL as data can contain a data detained by
         * someone else */
        if (remove_data != NULL) {
            (*remove_data)(p->data);
        }
        p_free((void **)&p);

        if(!is_liste_empty(l)) {
            l->first->prec = NULL;
        }
    }
}

void remove_last_element(liste* l, void (*remove_data)(void *data))
{
    if(!is_liste_empty(l)) {
        element *p = l->end;

        l->end = l->end->prec;
        (l->nbreElement)--;
        /* remove_data can be NULL as data can contain a data detained by
         * someone else */
        if (remove_data != NULL) {
            (*remove_data)(p->data);
        }
        p_free((void **)&p);

        if(!is_liste_empty(l)) {
            l->end->suiv = NULL;
        }
    }
}

void remove_element_n(liste *l, unsigned int const n,
                      void (*remove_data)(void *data))
{
    printf("removeElementN NOT YET FONCTIONNAL\n");
    return;

    if(((int)n > 0) && (n <= l->nbreElement)) {
        if(n == 1) {
            remove_first_element(l, remove_data);
        } else if(n == l->nbreElement) {
            remove_last_element(l, remove_data);
        } else {
            element *p = NULL;
            unsigned int compt = 1;

            printf("suppresion milieu : %d\n", n);

            p = l->first;
            while(compt<n-1) {
                p = p->suiv;
            }
            remove_next_element(p, remove_data);
        }
    }
    printf("\n");
}

void
remove_next_element(element *e, void (*remove_data)(void *data))
{
    printf("remove_next_element NOT YET FONCTIONNAL\n");
    return;

    element *p = e->suiv;
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

/*void trie_liste_fusion(liste *l, bool sensCroissant)
{}

void division(liste *l)
{}

void fusion(liste *l, bool sensCroissant)
{}
*/

/* }}} */

liste creer_liste(void)
{
    liste l;

    l.first = NULL;
    l.end = NULL;
    l.nbreElement = 0;

    return l;
}

bool is_liste_empty(liste *l)
{
    return (l->first==NULL);
}

bool is_elem_empty(element *e)
{
    return (e == NULL);
}

void
visu_liste(liste l, void (*visuElement)(void const *data), char const *s)
{
    if(visuElement != NULL)
    {
        element *p = l.first;

        while(!is_elem_empty(p)) {
            (*(visuElement))(p->data);
            p = p->suiv;
        }
        printf("%s", s);
    } else {
        fprintf(stderr,
                "impossible de visuliser la liste, visuElement NULL\n");
    }
}

void free_liste(liste* l, void (*remove_data)(void *data))
{
    while(!is_liste_empty(l)) {
        remove_first_element(l, remove_data);
    }
    l->first = NULL;
    l->end = NULL;
}

