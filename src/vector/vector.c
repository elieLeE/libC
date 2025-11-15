#include "vector.h"

#include "../mem/mem.h"
#include "../macros.h"

void *__gv_extend(__vector_void_t *vec, int extra)
{
    void *res;

    if (vec->len + extra >= vec->size) {
        int new_size = vec->size + extra;

        vec->tab = RETHROW_P(p_realloc(vec->tab, new_size * vec->__size_elem));
        vec->size = new_size;
    }

    res = vec->tab + vec->len * vec->__size_elem;

    p_clear(res, extra * vec->__size_elem);

    return res;
}

void *__gv_grow(__vector_void_t *vec, int extra)
{
    void *res;

    res = RETHROW_P(__gv_extend(vec, extra));

    vec->len++;

    return res;
}

void *__gv_create_empty_spot(__vector_void_t *vec, long pos)
{
    if (pos < 0) {
        logger_error("position indicated in '_gv_create_empty_spot' "
                     "is wrong: %ld", pos);
        return NULL;
    }
    if (pos >= vec->size) {
        logger_error("position indicated in '_gv_create_empty_spot' (%ld) "
                     "is bigger than the size of the vector (%ld)",
                     pos, vec->size);
        return NULL;
    }
    if (pos >= vec->len) {
        logger_warning("position indicated in '_gv_create_empty_spot' (%ld) "
                     "is bigger than the length of the vector (%ld) - "
                     "new element is added at the end",
                     pos, vec->len);
        /* we have checked that pos < vec->size above */
        return vec->tab + vec->__size_elem * vec->len;
    }

    memmove(vec->tab + vec->__size_elem * (pos + 1),
            vec->tab + vec->__size_elem * pos,
            (vec->len - pos) * vec->__size_elem);

    return vec->tab + vec->__size_elem * pos;
}

/* This methods suppose that that array is sorted. It will stop at the first
 * elem below than the one given as argument.
 * Moreover, by default elements are sorted by increasing order. If you wants
 * to sort them by decreasing order, implement a method that will return > 0
 * when the first element is below than the second one.
 */
int __gv_search_spot(__vector_void_t *vec, void *elem,
                     int (*cmp_data_cb)(const void *d1, const void *d2))
{
    if (vec->len == 0) {
        return 0;
    }
    if (cmp_data_cb(vec->tab + (vec->len - 1) * vec->__size_elem, elem) < 0) {
        return vec->len;
    }
    gv_for_each_pos(pos, vec) {
        if (cmp_data_cb(vec->tab + (pos * vec->__size_elem), elem) > 0) {
            return pos;
        }
    }

    logger_error("No spot have been found in '_gv_search_spot'");
    return -1;
}

int __gv_remove_elem_n(__vector_void_t *vec, long pos)
{
    if (pos < 0) {
        logger_error("pos is wrong: %ld", pos);
        return -1;
    }
    if (pos > vec->len -1) {
        logger_error("pos (%ld) is bigger then the length of the vector (%ld)",
                     pos, vec->len);
        return -1;
    }
    if (pos < vec->len - 1) {
        memmove(vec->tab + vec->__size_elem * pos,
                vec->tab + vec->__size_elem * (pos + 1),
                vec->__size_elem * (vec->len - pos - 1));
    }
    vec->len--;

    return 0;
}

int __gv_find(__vector_void_t *vec, void *elem,
              int (*cmp_data_cb)(const void *, const void *))
{
    gv_for_each_pos(pos, vec) {
        if (cmp_data_cb((vec->tab + pos * vec->__size_elem), elem) == 0) {
            return pos;
        }
    }
    return -1;
}

static void __gv_free_tab(__vector_void_t *vec, void (*free_data_cb)(void **))
{
    gv_for_each_pos(pos, vec) {
        free_data_cb(vec->tab + pos * vec->__size_elem);
    }
}

void __gv_clear(__vector_void_t *vec, void (*free_data_cb)(void **))
{
    if (free_data_cb != NULL) {
        __gv_free_tab(vec, free_data_cb);
    }
    p_clear(vec->tab, vec->__size_elem * vec->size);

    vec->len = 0;
}

void __gv_wipe(__vector_void_t *vec, void (*free_data_cb)(void **))
{
    if (free_data_cb != NULL) {
        __gv_free_tab(vec, free_data_cb);
    }

    vec->len = 0;
    vec->size = 0;

    p_free((void **)&(vec->tab));
}
