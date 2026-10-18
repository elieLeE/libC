#ifndef __CONTAINERS_UTILS_H__
#define __CONTAINERS_UTILS_H__

#include <stdint.h>

typedef int (*g_cmp_cb_t)(const void *, const void *);

int64_t get_next_closest_power_of_two(int64_t n);

#endif
