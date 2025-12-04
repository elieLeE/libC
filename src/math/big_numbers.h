#ifndef __BIG_NUMBERS_H__
#define __BIG_NUMBERS_H__

#include "../vector/vector.h"

typedef struct big_number_t {
    gv_t(uint64) parts;
    bool positive_number;
} big_number_t;

#endif
