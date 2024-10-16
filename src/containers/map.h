#ifndef __MAP_H__
#define __MAP_H__

#include <stdlib.h>
#include <stdint.h>

#include "../macros.h"

#include "utils.h"

#define DEFAULT_SIZE 1031

#define generic_map_data_t(_keys_type, _values_type)                          \
    struct {                                                                  \
        _keys_type keys;                                                      \
        _values_type values;                                                  \
        int32_t *tabs_len, *tabs_size;                                        \
        int32_t size;                                                         \
        int64_t count;                                                        \
        const size_t __size_elem_val;                                         \
        const size_t __size_elem_key;                                         \
    }

typedef generic_map_data_t(void **, void **) __map_void_t;

#define gm_t(_name_) generic_map_##_name_##_t

#define generic_map_t(_name, _keys_type, _values_type, hash_cb, cmp_cb)       \
    typedef union gm_t(_name) {                                               \
        generic_map_data_t(_keys_type **, _values_type **);                   \
        __map_void_t __map;                                                   \
    } gm_t(_name);

typedef int32_t (*gm_hash_cb_t)(const void *);

#define __gm_keys_size(_map) sizeof(*_map->keys[0])
#define __gm_values_size(_map) sizeof(*_map->values[0])

/* {{{ Inits */

void __gm_init(__map_void_t *map, int32_t size, size_t size_elem_key,
               size_t size_elem_val);
#define gm_init(_gmap)                                                        \
    ({  __auto_type __gmap = (_gmap);                                         \
        __gm_init(&__gmap->__map, DEFAULT_SIZE, __gm_keys_size(__gmap),       \
                  __gm_values_size(__gmap));                                  \
        __gmap;                                                               \
    })

void __gm_init_size(__map_void_t *map, int32_t size, size_t size_elem_key,
                    size_t size_elem_val);
#define gm_init_size(_gmap, _size)                                            \
    ({                                                                        \
        int64_t __size = (_size);                                             \
        if (__size < 0) {                                                     \
            logger_fatal("_size is below 0: %ld", __size);                    \
        }                                                                     \
        __auto_type __gmap = (_gmap);                                         \
        __gm_init_size(&__gmap->__map, __size, __gm_keys_size(__gmap),        \
                  __gm_values_size(__gmap));                                  \
        __gmap;                                                               \
    })

/* }}} */
/* {{{ Pos methods */

static inline int32_t
__get_pos1_from_key(const __map_void_t *map, const void *k,
                    gm_hash_cb_t hash_cb)
{
    return (int32_t)(hash_cb(k) % map->size);
}

int32_t __get_pos2_from_key(const __map_void_t *map, const void *k,
                            g_cmp_cb_t cmp_cb, int32_t first_pos);

/* }}} */

void __gm_wipe(__map_void_t *map,
               void (*free_keys_cb)(void **),
               void (*free_values_cb)(void **));

#define gm_wipe(_gmap, free_keys_cb, free_values_cb)                          \
    __gm_wipe(&(_gmap)->__map, free_keys_cb, free_values_cb)

#endif
