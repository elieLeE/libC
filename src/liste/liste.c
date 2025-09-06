#include "liste.h"
#include "../mem/alloc.h"

/* {{{ Adding methods */

void add_element_first(liste* l, void *data)
{
    element* new = my_calloc(sizeof(*new));

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
    element* new = my_calloc(sizeof(*new));

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

void add_element_trie(liste *l, void *data, bool sens_croissant)
{
    if(!(l->comparElement == NULL)) {
        if(is_liste_empty(l)) {
            add_element_first(l, data);
        } else {
            if(sens_croissant) {
                add_element_trie_c(l, data);
            } else {
                add_element_trie_d(l, data);
            }
        }
    } else {
        fprintf(stderr, "comparElement NULL\n");
    }
}

void add_element_trie_c(liste *l, void *data)
{
    if((*(l->comparElement))(l->first->data, data) >= 0) {
        add_element_first(l, data);
    } else if((*(l->comparElement))(l->end->data, data) <= 0) {
        add_element_last(l, data);
    } else {
        element *p = l->first;

        if((*(l->comparElement))(data, l->first->data) > 0) {
            while((!is_elem_empty(p->suiv)) &&
                  ((*(l->comparElement))(p->suiv->data, data) < 0))
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
    element *new = my_calloc(sizeof(*new));

    new->data = data;
    new->suiv = e->suiv;
    new->prec = e;
    e->suiv->prec = new;
    e->suiv = new;
}

void add_element_trie_d(liste *l, void *data)
{
    if((*(l->comparElement))(l->first->data, data) <= 0) {
        add_element_first(l, data);
    } else if((*(l->comparElement))(l->end->data, data) >= 0) {
        add_element_last(l, data);
    } else {
        element *p = l->first;

        if((*(l->comparElement))(data, l->first->data) < 0) {
            while((!is_elem_empty(p->suiv)) &&
                  ((*(l->comparElement))(p->suiv->data, data) > 0))
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

void remove_data(liste const *l, void *data)
{
    if(l->removeData != NULL) {
        (*(l->removeData))(data);
    }
}

void remove_first_element(liste *l)
{
    if(!is_liste_empty(l)) {
        element *p = l->first;

        l->first = l->first->suiv;
        (l->nbreElement)--;
        remove_data(l, p->data);
        free(p);

        if(!is_liste_empty(l)) {
            l->first->prec = NULL;
        }
    }
}

void remove_last_element(liste* l)
{
    if(!is_liste_empty(l)) {
        element *p = l->end;

        l->end = l->end->prec;
        (l->nbreElement)--;
        remove_data(l, p->data);
        free(p);

        if(!is_liste_empty(l)) {
            l->end->suiv = NULL;
        }
    }
}

void remove_element_n(liste *l, unsigned int const n)
{
    printf("removeElementN NOT YET FONCTIONNAL\n");
    return;

    if(((int)n > 0) && (n <= l->nbreElement)) {
        if(n == 1) {
            remove_first_element(l);
        } else if(n == l->nbreElement) {
            remove_last_element(l);
        } else {
            element *p = NULL;
            unsigned int compt = 1;

            printf("suppresion milieu : %d\n", n);

            p = l->first;
            while(compt<n-1) {
                p = p->suiv;
            }
            remove_next_element(l, p);
        }
    }
    printf("\n");
}

void remove_next_element(liste *l, element *e)
{
    printf("remove_next_element NOT YET FONCTIONNAL\n");
    return;

    element *p = e->suiv;
    e->suiv = e->suiv->suiv;
    e->suiv->prec = e;
    remove_data(l, e->data);
    free(p);
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

liste creer_liste(void (*visu_element)(void const *data),
                  int (*compar_element)(void const *, void const *),
                  void (*remove_data)(void *data))
{
    liste l;

    l.first = NULL;
    l.end = NULL;
    l.nbreElement = 0;
    l.visuElement = visu_element;
    l.comparElement = compar_element;
    l.removeData = remove_data;

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

void visu_liste(liste l, char const *s)
{
    if(l.visuElement != NULL)
    {
        element *p = l.first;

        while(!is_elem_empty(p)) {
            (*(l.visuElement))(p->data);
            p = p->suiv;
        }
        printf("%s", s);
    } else {
        fprintf(stderr,
                "impossible de visuliser la liste, visuElement NULL\n");
    }
}

void free_liste(liste* l)
{
    while(!is_liste_empty(l)) {
        remove_first_element(l);
    }
    l->first = NULL;
    l->end = NULL;
}

