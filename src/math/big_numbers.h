#ifndef __BIG_NUMBERS_H__
#define __BIG_NUMBERS_H__

#include "../vector/vector.h"

typedef struct big_number_t {
    gv_t(uint64) parts;
    bool positive_number;
    unsigned long limit;
} big_number_t;

void bn_init(big_number_t *bn);
void bn_init_with_args(big_number_t *bn, long size, unsigned long limit);

void bn_set_from_bn(const big_number_t *in, big_number_t *out);
void bn_set_from_ul(big_number_t *bn, unsigned long n);
void bn_set_from_l(big_number_t *bn, long n);

void bn_fast_clear(big_number_t *bn);
void bn_clear(big_number_t *bn);
void bn_wipe(big_number_t *bn);

#endif
