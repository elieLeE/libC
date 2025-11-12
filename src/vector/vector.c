#include "vector.h"

#include "../mem/mem.h"

static void *gv_extend(__vector_void_t *vec, int extra, int size_elem)
{
    void *res;

    if (vec->len + extra >= vec->size) {
        int new_size = vec->size + extra;

        vec->tab = p_realloc(vec->tab, new_size * size_elem);
        vec->size = new_size;
    }

    res = vec->tab + vec->len * size_elem;

    p_clear(res, extra);

    return res;
}

void *_gv_grow(__vector_void_t *vec, int extra, size_t size_elem)
{
    void *res;

    res = gv_extend(vec, extra, size_elem);

    vec->len++;

    return res;
}

