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
