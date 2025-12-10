#include "big_numbers.h"

#include "../mem/mem.h"

#define LIMIT_MAX 100000000000000000

void bn_init_with_args(big_number_t *bn, long size, unsigned long limit)
{
    p_clear(bn, 1);

    bn->positive_number = true;
    if (limit == 0 || limit > LIMIT_MAX) {
        bn->limit = LIMIT_MAX;
    } else {
        bn->limit = limit;
    }

    gv_init_size(&(bn->parts), size);
}

void bn_init(big_number_t *bn)
{
    bn_init_with_args(bn, 0, LIMIT_MAX);
}

void bn_fast_clear(big_number_t *bn)
{
    gv_fast_clear(&(bn->parts));
    bn->positive_number = true;
}

void bn_clear(big_number_t *bn)
{
    gv_clear(&(bn->parts), NULL);
    bn->positive_number = true;
}

void bn_wipe(big_number_t *bn)
{
    gv_wipe(&(bn->parts), NULL);
}
