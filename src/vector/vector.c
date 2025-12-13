#include "vector.h"

#include "../mem/mem.h"
#include "../macros.h"
#include "../math/calculs.h"

void __gv_set(const __vector_void_t *src, __vector_void_t *dst)
{
    if (dst->size < src->len) {
        __gv_extend(dst, src->len - dst->len);
    }
    memcpy(dst->tab, src->tab, src->len * src->__size_elem);

    dst->len = src->len;
}

int __gv_copy(const __vector_void_t *src, long idx_start_src,
              long idx_start_dst, long len, __vector_void_t *dst)
{
    if (idx_start_src < 0) {
        logger_error("index of src vector is negative: %ld", idx_start_src);
        return -1;
    }
    if (idx_start_dst < 0) {
        logger_error("index of dst vector is negative: %ld", idx_start_src);
        return -1;
    }
    if (len < 0) {
        logger_error("the length is negative: %ld", len);
        return -1;
    }
    if (idx_start_src + len > src->len) {
        logger_error("the space memory indicated to copy is not available");
        return -1;
    }

    if (dst->size < idx_start_dst + len) {
        __gv_extend(dst, idx_start_dst + len - dst->len);
    }
    memcpy(dst->tab + (idx_start_dst * dst->__size_elem),
           src->tab + (idx_start_src * src->__size_elem),
           len * src->__size_elem);

    dst->len = idx_start_dst + len;

    return 0;
}

void *__gv_extend(__vector_void_t *vec, long extra)
{
    void *res;

    if (vec->len + extra > vec->size) {
       long new_size = get_next_closest_power_of_two(vec->len + extra);

        vec->tab = RETHROW_P(p_realloc(vec->tab, new_size * vec->__size_elem));
        vec->size = new_size;

        res = vec->tab + vec->len * vec->__size_elem;
        p_clear(res, extra * vec->__size_elem);

        return res;
    }

    res = vec->tab + vec->len * vec->__size_elem;

    return res;
}

void *__gv_grow(__vector_void_t *vec, long extra)
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
        logger_trace("position indicated in '_gv_create_empty_spot' (%ld) "
                     "is bigger than the length of the vector (%ld) - "
                     "new element is added at the end", pos, vec->len);
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

static long gv_find_sequantial(const __vector_void_t *vec, void *elem,
                               int (*cmp_data_cb)(const void *, const void *))
{
    gv_for_each_pos(pos, vec) {
        if (cmp_data_cb((vec->tab + pos * vec->__size_elem), elem) == 0) {
            return pos;
        }
    }
    return -1;
}

static long
gv_find_dichotomy_rec(const __vector_void_t *vec, void *elem, long min,
                      long max, int (*cmp_data_cb)(const void *, const void *))
{
    long current_pos = (max + min) / 2;
    int res;

    if (min > max) {
        return -1;
    }

    res = cmp_data_cb(vec->tab + current_pos * vec->__size_elem, elem);

    if (res == 0) {
        return current_pos;
    } else if (res < 0) {
        return gv_find_dichotomy_rec(vec, elem, current_pos + 1,
                                     max, cmp_data_cb);
    } else {
        return gv_find_dichotomy_rec(vec, elem, min, current_pos - 1,
                                     cmp_data_cb);
    }

    return -1;
}

static long
gv_find_dichotomy(const __vector_void_t *vec, void *elem,
                  int (*cmp_data_cb)(const void *, const void *))
{
    if (vec->len == 0) {
        return -1;
    }

    if (cmp_data_cb(vec->tab, elem) > 0) {
        return -1;
    }

    if (cmp_data_cb(vec->tab + (vec->len - 1) * vec->__size_elem, elem) < 0) {
        return -1;
    }

    return gv_find_dichotomy_rec(vec, elem, 0, vec->len, cmp_data_cb);
}

long __gv_find(const __vector_void_t *vec, void *elem, gv_algo_search_t algo,
               int (*cmp_data_cb)(const void *, const void *d))
{
    if (algo < 0 || algo > GV_ALGO_MAX_SEARCH) {
        logger_fatal("cannot search in the vector - algo given is wrong: %d",
                     algo);
    }

    switch (algo) {
    case GV_SEQUENTIAL_SEARCH:
        return gv_find_sequantial(vec, elem, cmp_data_cb);
        break;

    case GV_DICHOTOMY_SEARCH:
        return gv_find_dichotomy(vec, elem, cmp_data_cb);
        break;

    default:
        logger_fatal("no sort algorithm associated to %d", algo);
        break;
    }

    return -1;
}

void __gv_fast_clear(__vector_void_t *vec)
{
    vec->len = 0;
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
