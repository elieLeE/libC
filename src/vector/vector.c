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

static void
_gv_free_tab(__vector_void_t *vec, int size_elem, void (*free_data_cb)(void **))
{
    gv_for_each_pos(pos, vec) {
        free_data_cb(vec->tab + pos * size_elem);
    }
}

void
_gv_clear(__vector_void_t *vec, int size_elem, void (*free_data_cb)(void **))
{
    if (free_data_cb != NULL) {
        _gv_free_tab(vec, size_elem, free_data_cb);
    }
    p_clear(vec->tab, size_elem * vec->size);

    vec->len = 0;
}

void
_gv_wipe(__vector_void_t *vec, int size_elem, void (*free_data_cb)(void **))
{
    if (free_data_cb != NULL) {
        _gv_free_tab(vec, size_elem, free_data_cb);
    }

    vec->len = 0;
    vec->size = 0;

    p_free((void **)&(vec->tab));
}
