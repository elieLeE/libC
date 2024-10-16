#ifndef __MAP_H__
#define __MAP_H__

#include <stdlib.h>
#include <stdint.h>

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

#endif
