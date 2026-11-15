#ifndef __MAP_H__
#define __MAP_H__

#include <stdlib.h>
#include <stdint.h>

#include "../mem/mem.h"
#include "../macros.h"

#include "utils.h"

#define DEFAULT_SIZE 1031

/* 0100..00 on 64 bits. First bit is dedicated to sign. */
#define QHASH_COLLISION 0x4000000000000000
#define QHASH_OVERWRITE 0x1

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
    static inline uint32_t __hash_cb_map_##_name(const void *_v)              \
    {                                                                         \
        const _values_type *v = (const _values_type *)_v;                     \
        return hash_cb(v);                                                    \
    }                                                                         \
    static inline int __cmp_cb_map_##_name(const void *_v1, const void *_v2)  \
    {                                                                         \
        const _values_type *v1 = (const _values_type *)_v1;                   \
        const _values_type *v2 = (const _values_type *)_v2;                   \
        return cmp_cb(v1, v2);                                                \
    }                                                                         \
    typedef union gm_t(_name) {                                               \
        generic_map_data_t(_keys_type **, _values_type **);                   \
        __map_void_t __map;                                                   \
    } gm_t(_name);

typedef uint32_t (*gm_hash_cb_t)(const void *);

#define gm_hash_cb_name(_name) __hash_cb_map_##_name
#define gm_cmp_cb_name(_name) __cmp_cb_map_##_name

#define __gm_key_type(_map) typeof(*_map->keys[0])
#define __gm_value_type(_map) typeof(*_map->values[0])

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
/* {{{ Positions */


static inline int32_t
__get_pos1_from_key(const __map_void_t *map, const void *k,
                    gm_hash_cb_t hash_cb)
{
    return (int32_t)(hash_cb(k) % map->size);
}

int32_t __get_pos2_from_key(const __map_void_t *map, const void *k,
                            g_cmp_cb_t cmp_cb, int32_t first_pos);

static inline void
__gm_get_2pos_from_key(const __map_void_t *map, const void *k,
                       gm_hash_cb_t hash_cb, g_cmp_cb_t cmp_cb,
                       int32_t *pos1, int32_t *pos2)
{
    *pos1 = __get_pos1_from_key(map, k, hash_cb);
    *pos2 = __get_pos2_from_key(map, k, cmp_cb, *pos1);
}

static inline int64_t
__gm_get_fpos_from_2pos(int32_t pos1, int32_t pos2, int64_t collision)
{
    int64_t full_pos;

    full_pos = ((int64_t)pos1) << 32;
    full_pos += pos2;
    full_pos |= collision;

    return full_pos;
}

static inline void
__gm_get_2pos_from_fpos(int64_t full_pos, int32_t *pos1, int32_t *pos2)
{
    if (full_pos & QHASH_COLLISION) {
        full_pos &= ~QHASH_COLLISION;
    }
    *pos1 = (int32_t)(full_pos >> 32);
    *pos2 = (int32_t)full_pos;
}

/* }}} */
/* {{{ Adding elements */

int __gm_grow(__map_void_t *map, int32_t pos1, long extra,
              void **key_spot, void **val_spot);

#define gm_put(_name, _gmap, _key, _val, _force)                              \
    ({                                                                        \
        __auto_type __gmap = (_gmap);                                         \
        __gm_key_type(__gmap) __key = _key;                                   \
        int64_t __res = -1;                                                   \
        int32_t __pos1, __pos2;                                               \
        __gm_get_2pos_from_key(&__gmap->__map, &__key, gm_hash_cb_name(_name),\
                            gm_cmp_cb_name(_name), &__pos1, &__pos2);         \
        if (__pos2 == -1) {                                                   \
            __gm_key_type(__gmap) *__key_spot;                                \
            __gm_value_type(__gmap) *__val_spot;                              \
            __res = __gm_grow(&__gmap->__map, __pos1, 1,                      \
                              (void **)&__key_spot, (void **)&__val_spot);    \
            if (__res == 0) {                                                 \
                *__key_spot = __key;                                          \
                *__val_spot = (_val);                                         \
                __pos2 = __gmap->__map.tabs_len[__pos1] - 1;                  \
                __res = __gm_get_fpos_from_2pos(__pos1, __pos2, 0);           \
            }                                                                 \
        } else if ((_force) == QHASH_OVERWRITE) {                             \
            __gmap->values[__pos1][__pos2] = (_val);                          \
            __res = __gm_get_fpos_from_2pos(__pos1, __pos2, 0);               \
        } else {                                                              \
            __res = __gm_get_fpos_from_2pos(__pos1, __pos2, QHASH_COLLISION); \
        }                                                                     \
        __res;                                                                \
    })

#define gm_add(_name, _gmap, _key, _val)                                      \
    gm_put(_name, _gmap, _key, _val, 0)

#define gm_set(_name, _gmap, _key, _val)                                      \
    gm_put(_name, _gmap, _key, _val, QHASH_OVERWRITE)

static inline void
__gm_clear_val_spot(const __map_void_t *map, int32_t pos1, int32_t pos2)
{
    void *spot = map->values[pos1] + (pos2 * map->__size_elem_val);
    p_clear(spot, map->__size_elem_val);
}

#define gm_reserve(_name, _gmap, _key, _force)                                \
    ({                                                                        \
        __auto_type __gmap = (_gmap);                                         \
        __gm_key_type(__gmap) __key = _key;                                   \
        int64_t __res = -1;                                                   \
        int32_t __pos1, __pos2;                                               \
        __gm_get_2pos_from_key(&__gmap->__map, &__key, gm_hash_cb_name(_name),\
                            gm_cmp_cb_name(_name), &__pos1, &__pos2);         \
        if (__pos2 == -1) {                                                   \
            __gm_key_type(__gmap) *__key_spot;                                \
            __gm_value_type(__gmap) *__val_spot;                              \
            __res = __gm_grow(&__gmap->__map, __pos1, 1,                      \
                              (void **)&__key_spot, (void **)&__val_spot);    \
            if (__res == 0) {                                                 \
                *__key_spot = __key;                                          \
                __pos2 = __gmap->__map.tabs_len[__pos1] - 1;                  \
                __res = __gm_get_fpos_from_2pos(__pos1, __pos2, 0);           \
            }                                                                 \
        } else if ((_force) == QHASH_OVERWRITE) {                             \
            __gm_clear_val_spot(&__gmap->__map, __pos1, __pos2);              \
            __res = __gm_get_fpos_from_2pos(__pos1, __pos2, 0);               \
        } else {                                                              \
            __res = __gm_get_fpos_from_2pos(__pos1, __pos2, QHASH_COLLISION); \
        }                                                                     \
        __res;                                                                \
    })

/* }}} */
/* {{{ Getting elements */

int
__gm_get_p_from_key(const __map_void_t *map, const void *k,
                    gm_hash_cb_t hash_cb, g_cmp_cb_t cmp_cb, void **out);
#define gm_get_p_from_key(_name, _gmap, _key, _out)                           \
    ({                                                                        \
        __auto_type __gmap = (_gmap);                                         \
        __gm_key_type(__gmap) __key = _key;                                   \
        __gm_value_type(__gmap) *__val_got, **__out = (_out);                 \
        int __res = __gm_get_p_from_key(&(_gmap)->__map, &__key,              \
                                        gm_hash_cb_name(_name),               \
                                        gm_cmp_cb_name(_name),                \
                                        (void **)&__val_got);                 \
        if (__res == 0) {                                                     \
            *__out = __val_got;                                               \
        }                                                                     \
        __res;                                                                \
    })

#define gm_get_from_key(_name, _gmap, _key, _out)                             \
    ({                                                                        \
        __auto_type __gmap = (_gmap);                                         \
        __gm_key_type(__gmap) __key = _key;                                   \
        __gm_value_type(__gmap) *__val_got, *__out = (_out);                  \
        int __res = __gm_get_p_from_key(&(_gmap)->__map, &__key,              \
                                        gm_hash_cb_name(_name),               \
                                        gm_cmp_cb_name(_name),                \
                                        (void **)&__val_got);                 \
        if (__res == 0) {                                                     \
            *__out = *__val_got;                                              \
        }                                                                     \
        __res;                                                                \
    })

void __gm_get_p_from_fpos(const __map_void_t *map, int64_t fpos, void **out);
#define gm_get_p_from_fpos(_name, _gmap, _fpos, _out)                         \
    do {                                                                      \
        __auto_type __gmap = (_gmap);                                         \
        __gm_value_type(__gmap) *__val_got, **__out = (_out);                 \
        __gm_get_p_from_fpos(&__gmap->__map, (_fpos), (void **)&__val_got);   \
        *__out = __val_got;                                                   \
    } while (0)

#define gm_get_from_fpos(_name, _gmap, _fpos, _out)                           \
    do {                                                                      \
        __auto_type __gmap = (_gmap);                                         \
        __gm_value_type(__gmap) *__val_got, *__out = (_out);                  \
        __gm_get_p_from_fpos(&__gmap->__map, (_fpos), (void **)&__val_got);   \
        *__out = *__val_got;                                                  \
    } while (0)

int
__gm_get_p_from_fpos_safe(const __map_void_t *map, int64_t fpos, void **out);
#define gm_get_p_from_fpos_safe(_name, _gmap, _fpos, _out)                    \
    ({                                                                        \
        __auto_type __gmap = (_gmap);                                         \
        __gm_value_type(__gmap) *__val_got, **__out = (_out);                 \
        int __res = __gm_get_p_from_fpos_safe(&__gmap->__map, (_fpos),        \
                                              (void **)&__val_got);           \
        if (__res == 0) {                                                     \
            *__out = __val_got;                                               \
        }                                                                     \
        __res;                                                                \
    })

#define gm_get_from_fpos_safe(_name, _gmap, _fpos, _out)                      \
    ({                                                                        \
        __auto_type __gmap = (_gmap);                                         \
        __gm_value_type(__gmap) *__val_got, *__out = (__out);                 \
        int __res = __gm_get_p_from_fpos_safe(&__gmap->__map, (_fpos),        \
                                              (void **)&__val_got);           \
        if (__res == 0) {                                                     \
            *__out = *__val_got;                                              \
        }                                                                     \
        __res;                                                                \
    })

/* }}} */

int64_t __gm_find(const __map_void_t *map, const void *k,
                  gm_hash_cb_t hash_cb, g_cmp_cb_t cmp_cb);
#define gm_find(_name, _gmap, _key)                                           \
    ({                                                                        \
        __auto_type __gmap = (_gmap);                                         \
        __gm_key_type(__gmap) __key = _key;                                   \
        __gm_find(&(_gmap)->__map, &__key, gm_hash_cb_name(_name),            \
                  gm_cmp_cb_name(_name));                                     \
    })

void __gm_wipe(__map_void_t *map,
               void (*free_keys_cb)(void **),
               void (*free_values_cb)(void **));
#define gm_wipe(_gmap, free_keys_cb, free_values_cb)                          \
    __gm_wipe(&(_gmap)->__map, free_keys_cb, free_values_cb)

#endif
