#include <string.h>

#include "mem.h"
#include "../logger/logger.h"

void* p_malloc(size_t const taille)
{
    void *p;

    if (taille <= 0) {
        logger_error("cannot allocate a negative size: %ld", taille);
        return NULL;
    }

    p = malloc(taille);
    if (p == NULL) {
        logger_error("allocation has failed");
        return NULL;
    }
    return p;
}

void* p_calloc(size_t const taille)
{
    void *p;

    if (taille <= 0) {
        logger_error("cannot allocate a negative size: %ld", taille);
        return NULL;
    }

    p = calloc(1, taille);
    if (p == NULL) {
        logger_error("allocation has failed");
        return NULL;
    }
    return p;
}

void* p_realloc(void *p, size_t const taille)
{
    if (taille <= 0) {
        logger_error("cannot allocate a negative size: %ld", taille);
        return NULL;
    }

    p = realloc(p, taille);
    if (p == NULL) {
        logger_error("allocation has failed");
        return NULL;
    }
    return p;
}

void _p_clear(void *p, size_t size)
{
    memset(p, 0, size);
}

void** alloc_tab_2d(int const lig, int const col, size_t const type_size)
{
    int i, t;
    void **tab;

    if (type_size <= 0) {
        logger_error("cannot allocate a negative size: %ld", type_size);
        return NULL;
    }
    if (lig <= 0) {
        logger_error("cannot allocate a negative size: %d", lig);
        return NULL;
    }
    if (col <= 0) {
        logger_error("cannot allocate a negative size: %d", col);
        return NULL;
    }

    tab = p_calloc(lig * sizeof(void *));
    t = col * type_size;
    for (i = 0; i < lig; i++) {
        tab[i] = p_calloc(t);
    }
    return tab;
}

void free_tab_2d(void **matrice, int const lig)
{
    int i;

    if (lig <= 0) {
        logger_error("cannot free an array with a negative line number: %d",
                     lig);
        return;
    }

    for (i = 0; i < lig; i++) {
        p_free(&matrice[i]);
    }

    /* XXX: As matrice is a double pointer, we have to give triple pointer to
     * '_p_free'. But _p_free expects double pointer. We hide that in this way
     * but I am not happy of that. A better solution would be use directly the
     * types and so use MACROS but it is much more complicated.
     * To investigate. */
    p_free((void **)&matrice);
}

void _p_free(void **p)
{
    free(*p);
    *p = NULL;
}
