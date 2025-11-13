#include "vector.h"

#include "../mem/mem.h"
#include "../macros.h"

void *__gv_extend(__vector_void_t *vec, int extra, size_t size_elem)
{
    void *res;

    if (vec->len + extra >= vec->size) {
        int new_size = vec->size + extra;

        vec->tab = RETHROW_P(p_realloc(vec->tab, new_size * size_elem));
        vec->size = new_size;
    }

    res = vec->tab + vec->len * size_elem;

    p_clear(res, extra);

    return res;
}

void *__gv_grow(__vector_void_t *vec, int extra, size_t size_elem)
{
    void *res;

    res = __gv_extend(vec, extra, size_elem);

    vec->len++;

    return res;
}

void *__gv_create_empty_spot(__vector_void_t *vec, size_t size_elem, int pos)
{
    if (pos < 0) {
        logger_error("position indicated in '_gv_create_empty_spot' "
                     "is wrong: %d", pos);
        return NULL;
    }
    if (pos >= vec->size) {
        logger_error("position indicated in '_gv_create_empty_spot' (%d) "
                     "is bigger than the size of the vector (%d)",
                     pos, vec->size);
        return NULL;
    }
    if (pos >= vec->len) {
        logger_warning("position indicated in '_gv_create_empty_spot' (%d) "
                     "is bigger than the length of the vector (%d) - "
                     "new element is added at the end",
                     pos, vec->len);
        /* we have checked that pos < vec->size above */
        return vec->tab + size_elem * vec->len;
    }

    memmove(vec->tab + size_elem * (pos + 1), vec->tab + size_elem * pos,
            (vec->len - pos) * size_elem);

    return vec->tab + size_elem * pos;
}

/* This methods suppose that that array is sorted. It will stop at the first
 * elem below than the one given as argument.
 * Moreover, by default elements are sorted by increasing order. If you wants
 * to sort them by decreasing order, implement a method that will return > 0
 * when the first element is below than the second one.
 */
int __gv_search_spot(__vector_void_t *vec, size_t size_elem, void *elem,
                     int (*cmp_data_cb)(const void *d1, const void *d2))
{
    if (vec->len == 0) {
        return 0;
    }
    if (cmp_data_cb(vec->tab + (vec->len - 1) * size_elem, elem) < 0) {
        return vec->len;
    }
    gv_for_each_pos(pos, vec) {
        if (cmp_data_cb(vec->tab + (pos * size_elem), elem) > 0) {
            return pos;
        }
    }

    logger_error("No sport have been found in '_gv_search_spot'");
    return -1;
}

static void __gv_free_tab(__vector_void_t *vec, int size_elem,
                          void (*free_data_cb)(void **))
{
    gv_for_each_pos(pos, vec) {
        free_data_cb(vec->tab + pos * size_elem);
    }
}

void
__gv_clear(__vector_void_t *vec, int size_elem, void (*free_data_cb)(void **))
{
    if (free_data_cb != NULL) {
        __gv_free_tab(vec, size_elem, free_data_cb);
    }
    p_clear(vec->tab, size_elem * vec->size);

    vec->len = 0;
}

void
__gv_wipe(__vector_void_t *vec, int size_elem, void (*free_data_cb)(void **))
{
    if (free_data_cb != NULL) {
        __gv_free_tab(vec, size_elem, free_data_cb);
    }

    vec->len = 0;
    vec->size = 0;

    p_free((void **)&(vec->tab));
}
