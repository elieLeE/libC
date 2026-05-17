#ifndef __BIG_NUMBERS_H__
#define __BIG_NUMBERS_H__

#include "../vector/vector.h"

typedef struct big_number_t {
    gv_t(uint64) parts;
    bool positive_number;
    const unsigned long limit;
} big_number_t;

void bn_init(big_number_t *bn);
void bn_init_with_args(big_number_t *bn, long size, unsigned long limit);

void bn_set_limit(big_number_t *bn, unsigned long limit);

static inline void
bn_set_part(big_number_t *bn, unsigned long val, long idx)
{
    bn->parts.tab[idx] = val;

    if (idx >= bn->parts.len) {
        bn->parts.len = idx + 1;
    }
}

static inline void
bn_set_part_safe(big_number_t *bn, unsigned long val, long idx)
{
    if (idx >= bn->parts.size) {
        logger_fatal("idx (%ld) is bigger than the BN length (%ld)",
                     idx, bn->parts.len);
    }
    if (val >= bn->limit) {
        logger_fatal("value (%ld) is bigger than the BN limit (%ld)",
                     val, bn->limit);
    }
    bn_set_part(bn, val, idx);
}

/* Synaptic gets me an error here. I do not exactly why yet. For now,
 * the method is defined in the .c.
 * TODO: TO INVESTIGATE */
void bn_add_part(big_number_t *bn, unsigned long val);

static inline unsigned long
bn_get_part(const big_number_t *bn, long idx)
{
    return bn->parts.tab[idx];
}

static inline unsigned long
bn_get_part_safe(const big_number_t *bn, long idx)
{
    if (idx >= bn->parts.len) {
        logger_fatal("idx (%ld) is bigger than the BN length (%ld)",
                     idx, bn->parts.len);
    }
    return bn_get_part(bn, idx);
}

unsigned int bn_get_digits_count(const big_number_t *bn);

/* determine which number between the big number 'bn' and 'bn2/n' is the
 * biggest.
 * return
 *      > 0 if bn > bn2/n
 *      = 0 if bn = 0 (so if bn represents he same value as bn2/n)
 *      < 0 if bn < bn2/n
 */
int bn_cmp(const big_number_t *bn1, const big_number_t *bn2);
int bn_cmp_ul(const big_number_t *bn, unsigned long n);

void bn_set_from_bn(const big_number_t *src, big_number_t *out);
void bn_set_from_ul(unsigned long n, big_number_t *out);
void bn_set_from_l(long n, big_number_t *out);

/* Note: in all methods below, 'out' can be the same pointer than one of
 * the "in" big number argument" */
int bn_add_bn(const big_number_t *bn1, const big_number_t *bn2,
              big_number_t *out);
void bn_add_ul(const big_number_t *bn, unsigned long n, big_number_t *out);
void bn_add_l(const big_number_t *bn, long n, big_number_t *out);

void bn_sub_ul(const big_number_t *bn, unsigned long n, big_number_t *out);

char *bn_to_str(const big_number_t *bn);

void bn_fast_clear(big_number_t *bn);
void bn_clear(big_number_t *bn);
void bn_wipe(big_number_t *bn);

#endif
