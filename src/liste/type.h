#ifndef _TYPE_H_
#define _TYPE_H_

typedef struct generic_elem_liste_t {
    struct generic_elem_liste_t *prec, *suiv;
    void *data;
} generic_elem_liste_t;

typedef struct generic_liste_t {
    generic_elem_liste_t *first, *end;
    unsigned int nbre_elem;
} generic_liste_t;

#endif
