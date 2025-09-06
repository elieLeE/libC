#ifndef _TYPE_H_
#define _TYPE_H_

typedef struct element{
    struct element *prec, *suiv;
    void *data;
} element;

typedef struct liste{
    struct element *first, *end;
    unsigned int nbreElement;

    /*
     * renvoie :
     * - < 0 si d1 < d2
     * - 0 si d1 == d2
     * - > 0 si d1 > d2
     *   */
    int (*comparElement)(void const *d1, void const *d2);
    void (*removeData)(void *data);
} liste;

#endif
