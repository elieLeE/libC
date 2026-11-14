#include "map.h"

#include "../mem/mem.h"

/* {{{ Helpers methods */

static inline int32_t get_next_prime_number(int32_t n)
{
    /* greatest prime number below than 2147483647 (greater nb on 31 bits) */
    const int32_t max_arrays_nb = 2147483629;

    if (n > max_arrays_nb) {
        logger_warning("sized asked (%d) overtakes max authorized size (%d) - "
                       "force to use max size", n, max_arrays_nb);
        return max_arrays_nb;
    }

    if (n % 2 == 0) {
        n++;
    }

    for (int32_t i = n; i <= 2 * n; i += 2) {
        bool is_prime = true;

        for (int32_t j = 3; j * j <= i; j += 2) {
            if (i % j == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (i > max_arrays_nb) {
                i = max_arrays_nb;

                logger_warning("size calculated (%d) overtakes max authorized "
                               "size (%d) - force to use max size",
                               i, max_arrays_nb);
            }
            return i;
        }
    }

    return -1;
}

/* }}} */
/* {{{ Inits methods */

static inline void
__gm_set_sizes_elem(__map_void_t *map, size_t size_keys, size_t size_values)
{
    size_t *size_elem_p;

    size_elem_p = unconst_cast(size_t, &map->__size_elem_key);
    if (size_elem_p == NULL) {
        logger_fatal("error when trying to set '__size_elem'");
    }
    *size_elem_p = size_keys;

    size_elem_p = unconst_cast(size_t, &map->__size_elem_val);
    if (size_elem_p == NULL) {
        logger_fatal("error when trying to set '__size_elem'");
    }
    *size_elem_p = size_values;
}

void __gm_init(__map_void_t *map, int32_t size, size_t size_elem_key,
               size_t size_elem_val)
{
    p_clear(map, 1);

    __gm_set_sizes_elem(map, size_elem_key, size_elem_val);

    map->size = size;

    map->keys = p_calloc(sizeof(*(map->keys)) * size);
    map->values = p_calloc(sizeof(*(map->values)) * size);
    map->tabs_len = p_calloc(sizeof(int) * size);
    map->tabs_size = p_calloc(sizeof(int) * size);
}

void __gm_init_size(__map_void_t *map, int32_t size, size_t size_elem_key,
               size_t size_elem_val)
{
    __gm_init(map, get_next_prime_number(size), size_elem_key, size_elem_val);
}

/* }}} */
/* {{{ Pos methods */

int32_t __get_pos2_from_key(const __map_void_t *map, const void *k,
                            g_cmp_cb_t cmp_cb, int32_t pos1)
{
    const void *possible_equal_keys = map->keys[pos1];

    for (int32_t i = 0; i < map->tabs_len[pos1]; i++) {
        if (cmp_cb(k, possible_equal_keys + (i * map->__size_elem_key)) == 0) {
            return i;
        }
    }

    return -1;
}

/* }}} */
/* {{{ Extending methods */

static inline int32_t get_new_array_size(int32_t current_size)
{
    /* greater nb on '31 bits signed' (so 30 bits).
     * One bit is lost for the sign, the other one for collision. */
    const int max_array_size = 1073741823;

    int64_t new_size = get_next_closest_power_of_two(current_size);

    if (new_size > max_array_size) {
        logger_warning("new size (%ld) overtakes max authorized size (%d) - "
                       "force to use max size", new_size, max_array_size);
        return max_array_size;
    }

    return new_size;
}

static inline int __gm_extend(__map_void_t *map, int32_t pos1, long extra,
                              void **key_spot, void **val_spot)
{
    int32_t len = map->tabs_len[pos1];
    int32_t size = map->tabs_size[pos1];
    void *new_p;
    int32_t new_size;

    if (len + extra <= size) {
        *key_spot = map->keys[pos1] + len * map->__size_elem_key;
        *val_spot = map->values[pos1] + len * map->__size_elem_val;

        return 0;
    }

    new_size = get_new_array_size(len + extra);

    /* realloc keys tab */
    new_p = p_realloc(map->keys[pos1], new_size * map->__size_elem_key);
    if (new_p == NULL) {
        logger_error("failed allocating memory\n");
        return -1;
    }
    map->keys[pos1] = new_p;

    *key_spot = map->keys[pos1] + len * map->__size_elem_key;
    p_clear(*key_spot, extra * map->__size_elem_key);

    /* realloc values tab */
    new_p = p_realloc(map->values[pos1], new_size * map->__size_elem_val);
    if (new_p == NULL) {
        logger_error("failed allocating memory\n");
        return -1;
    }
    map->values[pos1] = new_p;

    *val_spot = map->values[pos1] + len * map->__size_elem_val;
    p_clear(*val_spot, extra * map->__size_elem_val);

    map->tabs_size[pos1] = new_size;

    return 0;
}

int __gm_grow(__map_void_t *map, int32_t pos1, long extra,
              void **key_spot, void **val_spot)
{
    RETHROW(__gm_extend(map, pos1, extra, key_spot, val_spot));

    map->tabs_len[pos1]++;
    map->count++;

    return 0;
}

/* }}} */
/* {{{ Getting methods */

static void
__gm_get_p_from_2pos(const __map_void_t *map, int32_t pos1, int32_t pos2,
                     void **out)
{
    *out = map->values[pos1] + (map->__size_elem_val * pos2);
}

static int
__gm_get_p_from_2pos_safe(const __map_void_t *map, int32_t pos1, int32_t pos2,
                          void **out)
{
    if (pos1 < 0 || pos1 >= map->size ||
        pos2 < 0 || pos2 >= map->tabs_len[pos1])
    {
        return -1;
    }

    __gm_get_p_from_2pos(map, pos1, pos2, out);

    return 0;
}

void __gm_get_p_from_fpos(const __map_void_t *map, int64_t fpos, void **out)
{
    int32_t pos1, pos2;

    __gm_get_2pos_from_fpos(fpos, &pos1, &pos2);
    __gm_get_p_from_2pos(map, pos1, pos2, out);
}

int
__gm_get_p_from_fpos_safe(const __map_void_t *map, int64_t fpos, void **out)
{
    int32_t pos1, pos2;

    __gm_get_2pos_from_fpos(fpos, &pos1, &pos2);
    return __gm_get_p_from_2pos_safe(map, pos1, pos2, out);
}

int
__gm_get_p_from_key(const __map_void_t *map, const void *k,
                    gm_hash_cb_t hash_cb, g_cmp_cb_t cmp_cb, void **out)
{
    int32_t pos1, pos2;

    __get_2pos_from_key(map, k, hash_cb, cmp_cb, &pos1, &pos2);

    if (pos2 >= 0) {
        __gm_get_p_from_2pos(map, pos1, pos2, out);
        return 0;
    }
    return -1;
}

/* }}} */

int __gm_find(const __map_void_t *map, const void *k,
              gm_hash_cb_t hash_cb, g_cmp_cb_t cmp_cb)
{
    int32_t pos1, pos2;

    __get_2pos_from_key(map, k, hash_cb, cmp_cb, &pos1, &pos2);

    return pos2 >= 0 ? __gm_get_fpos_from_2pos(pos1, pos2, 0) : -1;
}

void __gm_wipe(__map_void_t *map,
               void (*free_keys_cb)(void **),
               void (*free_values_cb)(void **))
{
    for (int32_t i = 0; i < map->size; i++) {
        int32_t len = map->tabs_len[i];

        if (len > 0) {
            if (free_keys_cb != NULL) {
                for (int32_t j = 0; j < len; j++) {
                    free_keys_cb(map->keys[i]);
                }
            }

            if (free_values_cb != NULL) {
                for (int32_t j = 0; j < len; j++) {
                    free_values_cb(map->values[i]);
                }
            }

            p_free((void **)&(map->keys[i]));
            p_free((void **)&(map->values[i]));
        }
    }

    map->size = 0;
    map->count = 0;

    p_free((void **)&(map->keys));
    p_free((void **)&(map->values));
    p_free((void **)&(map->tabs_len));
    p_free((void **)&(map->tabs_size));
}
