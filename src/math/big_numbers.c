#include "big_numbers.h"

#include <limits.h>

#include "../mem/mem.h"
#include "nber_helper.h"
#include "../utils.h"

#define LIMIT_MAX 100000000000000000

void bn_set_limit(big_number_t *bn, unsigned long limit)
{
    unsigned long _limit;
    unsigned long *limit_p;

    if (limit == 0 || limit > LIMIT_MAX) {
        _limit = LIMIT_MAX;
    } else {
        _limit = limit;
    }

    limit_p = unconst_cast(size_t, &bn->limit);
    if (limit_p == NULL) {
        logger_fatal("error when trying to set '__size_elem'");
    }
    *limit_p = _limit;
}

void bn_init_with_args(big_number_t *bn, long size, unsigned long limit)
{
    p_clear(bn, 1);

    bn_set_limit(bn, limit);

    bn->positive_number = true;

    gv_init_size(&(bn->parts), size);
}

void bn_init(big_number_t *bn)
{
    bn_init_with_args(bn, 0, LIMIT_MAX);
}

/* {{{ Helpers methods */

unsigned int bn_get_digits_count(const big_number_t *bn)
{
    unsigned int digits_count_limit;

    if (bn->parts.len == 0) {
        return 0;
    }

    if (bn->parts.len == 1) {
        return get_count_digits_of_n(bn->parts.tab[0]);
    }

    digits_count_limit = get_count_digits_of_n(bn->limit) - 1;

    return digits_count_limit * (bn->parts.len - 1) +
        get_count_digits_of_n(bn->parts.tab[bn->parts.len - 1]);
}

/* WARNING, this method could generate an overflow if it used on a "real" big
 * number, as to say a big number greater than ULONG_MAX.
 * Never use it. It is only dedicated to this module as it is used only when
 * it is known that the big number is below ULONG_MAX */
static unsigned long bn_get_ul(const big_number_t *bn)
{
    unsigned long n = bn->parts.tab[bn->parts.len - 1];

    for (int i = bn->parts.len - 2; i >= 0; i--) {
        n *= bn->limit;
        n += bn->parts.tab[i];
    }
    return n;
}

int bn_cmp_ul(const big_number_t *bn, unsigned long n)
{
    unsigned int digits_count_bn;
    unsigned long bn_ul;

#if __x86_64__
    const unsigned int digits_count_unsigned_long_max = 20;
#else
    const unsigned int digits_count_unsigned_long_max = 10;
#endif

    if (bn->parts.len == 0) {
        return -1;
    }

    if (bn->parts.len == 1) {
        if (bn->parts.tab[0] > n) {
            return 1;
        }
        return -1;
    }

    digits_count_bn = bn_get_digits_count(bn);
    if (digits_count_bn > digits_count_unsigned_long_max) {
        return 1;
    }

    bn_ul = bn_get_ul(bn);
    return g_cmp_uint64(&bn_ul, &n);
}

int bn_cmp(const big_number_t *bn1, const big_number_t *bn2)
{
    if (bn1->limit != bn2->limit) {
        /* TODO => Disable for now */
        logger_fatal("cannot compare 2 big numbers with different limits: "
                     "%ld - %ld",
                     bn1->limit, bn2->limit);
    }

    if (bn1->parts.len != bn2->parts.len) {
        return bn1->parts.len - bn2->parts.len;
    }

    for (int i = bn1->parts.len - 1; i >= 0; i--) {
        unsigned long n1 = bn1->parts.tab[i];
        unsigned long n2 = bn2->parts.tab[i];

        if (n1 != n2) {
            return n1 - n2;
        }
    }

    return 0;
}

/* }}} */
/* {{{ Setting methods */

void bn_set_from_bn(const big_number_t *src, big_number_t *dst)
{
    gv_set(&src->parts, &dst->parts);

    dst->positive_number = src->positive_number;
    bn_set_limit(dst, src->limit);
}

void bn_set_from_ul(unsigned long n, big_number_t *out)
{
    /* not indispensable but I think it is clearer with the definition of
     * this variable */
    unsigned long tmp = n;

    bn_fast_clear(out);

    while (tmp >= out->limit) {
        unsigned long carry = tmp / out->limit;

        tmp -= out->limit * carry;
        gv_add(&(out->parts), tmp);

        tmp = carry;
    }
    gv_add(&(out->parts), tmp);
}

void bn_set_from_l(long n, big_number_t *out)
{
    if (n >= 0) {
        bn_set_from_ul(n, out);
    } else {
        bn_set_from_ul(-n, out);
        out->positive_number = false;
    }
}

/* }}} */
/* {{{ Adding methods */
/* {{{ Same sign */

static void
__bn_add_ul(const big_number_t *bn, unsigned long n,
            long first_idx, big_number_t *out)
{
    unsigned long tmp;
    long idx_part = first_idx;

    if (out->parts.size < bn->parts.len) {
        gv_extend(&out->parts, bn->parts.len - out->parts.len);
    }

    /* len of the vector have been checked in the calling method */
    // ouverflow is not possible here. Case are handled in method 'bn_add_ul'.
    tmp = bn->parts.tab[idx_part] + n;
    out->parts.tab[idx_part] = tmp;

    while (tmp >= bn->limit) {
        unsigned int carry = tmp / bn->limit;

        tmp -= bn->limit * carry;
        out->parts.tab[idx_part] = tmp;

        idx_part++;

        if (idx_part > bn->parts.len - 1) {
            tmp = carry;
            break;
        }

        tmp = bn->parts.tab[idx_part] + carry;
        out->parts.tab[idx_part] = tmp;
    }

    if (idx_part > bn->parts.len - 1) {
        while (tmp >= bn->limit) {
            unsigned long carry = tmp / bn->limit;

            tmp -= bn->limit * carry;
            gv_add(&(out->parts), tmp);

            tmp = carry;
        }
        if (tmp != 0) {
            gv_add(&(out->parts), tmp);
        }
    }
}

static void
_bn_add_bn(const big_number_t *bn1, const big_number_t *bn2,
           big_number_t *out)
{
    unsigned long carry = 0;
    const big_number_t *shortest_bn, *longest_bn;
    long short_bn_len;

    if (bn1->parts.len >= bn2->parts.len) {
        shortest_bn = bn2;
        longest_bn = bn1;
        short_bn_len = bn2->parts.len;
    } else {
        shortest_bn = bn1;
        longest_bn = bn2;
        short_bn_len = bn1->parts.len;
    }

    /* cannot do that in the method 'bn_add_bn' because this methods can also
     * be called from 'bn_add_ul' and 'bn_sub_ul' */
    if (out != longest_bn && out != shortest_bn) {
        gv_set(&(longest_bn->parts), &(out->parts));
    }

    for (long i = 0; i < shortest_bn->parts.len; i++) {
        unsigned long tmp = bn1->parts.tab[i] + bn2->parts.tab[i] + carry;

        if (tmp >= out->limit) {
            carry = tmp / bn1->limit;
            tmp -= bn1->limit * carry;
        } else {
            carry = 0;
        }
        out->parts.tab[i] = tmp;
    }

    if (longest_bn != out) {
        long diff_len = longest_bn->parts.len - shortest_bn->parts.len;

        if (diff_len > 0) {
            gv_copy(&longest_bn->parts, shortest_bn->parts.len,
                    shortest_bn->parts.len, diff_len, &out->parts);
        }
    }

    if (carry == 0) {
        return;
    }

    if (longest_bn->parts.len == short_bn_len) {
        gv_add(&out->parts, carry);
        return;
    }

    __bn_add_ul(longest_bn, carry, short_bn_len, out);
}

static void
_bn_add_ul(const big_number_t *bn, unsigned long n, big_number_t *out)
{
    if (bn != out) {
        bn_set_from_bn(bn, out);
    }

    if (n < ULONG_MAX + bn->limit) {
        __bn_add_ul(bn, n, 0, out);
    } else {
        big_number_t tmp;

        bn_init_with_args(&tmp, 0, bn->limit);
        bn_set_from_ul(n, &tmp);

        _bn_add_bn(&tmp, bn, out);

        bn_wipe(&tmp);
    }
}

/* }}} */
/* {{{ Opposite sign */

static void
_bn_set_part_or_add(unsigned long val, long idx, big_number_t *out)
{
    if (idx > out->parts.len -1) {
        gv_add(&out->parts, val);
    } else {
        out->parts.tab[idx] = val;
    }
}

static inline unsigned long
_get_bn_part_sub_ul(unsigned long n1_part, unsigned long n2_part,
                    unsigned long bn_limit, bool is_n1_biggest,
                    unsigned long *carry)
{
    if (is_n1_biggest) {
        if (n1_part >= n2_part) {
            *carry = 0;
            return n1_part - n2_part;
        } else {
            *carry = 1;
            return bn_limit + n1_part - n2_part;
        }
    } else if (n1_part <= n2_part) {
        *carry = 0;
        return n2_part - n1_part;
    } else {
        *carry = 1;
        return bn_limit + n2_part - n1_part;
    }
}

/* This method only manages cases where n is below then bn->limit ! */
static void
__bn_sub_ul(const big_number_t *bn, unsigned long n,
           long first_idx, bool is_bn_biggest, big_number_t *out)
{
    long bn_part_idx = first_idx;
    unsigned long carry = n;

    do {
        unsigned long val;

        val = _get_bn_part_sub_ul(bn->parts.tab[bn_part_idx], carry, bn->limit,
                                  is_bn_biggest, &carry);
        _bn_set_part_or_add(val, bn_part_idx, out);

        bn_part_idx++;
    } while (carry != 0);

    if (bn_part_idx <= bn->parts.len - 1) {
        gv_copy(&bn->parts, bn_part_idx, bn_part_idx,
                (bn->parts.len - bn_part_idx), &out->parts);
    } else {
        long i = out->parts.len - 1;

        while (i >= 0 && out->parts.tab[i] == 0) {
            out->parts.len--;
            i--;
        }
    }
}

static void
_bn_sub_bn(const big_number_t *bn1, const big_number_t *bn2, big_number_t *out)
{
    unsigned long carry = 0;
    long bn_part_idx = 0;
    /* first bn is always the first one in this method */
    long short_bn_len = bn2->parts.len;

    if (bn1 != out && bn2 != out) {
        gv_set(&(bn1->parts), &(out->parts));
    }

    for (bn_part_idx = 0; bn_part_idx < short_bn_len - 1; bn_part_idx++) {
        unsigned long n1 = bn1->parts.tab[bn_part_idx];
        unsigned long n2 = bn2->parts.tab[bn_part_idx] + carry;
        unsigned long tmp = _get_bn_part_sub_ul(n1, n2, bn1->limit,
                                                true, &carry);

        if (tmp > bn1->limit) {
            logger_fatal("%ld is bigger than limit (%ld)\n", tmp, bn1->limit);
        }

        _bn_set_part_or_add(tmp, bn_part_idx, out);
    }

    __bn_sub_ul(bn1, bn2->parts.tab[short_bn_len - 1] + carry, bn_part_idx,
                true, out);

    return;
}

static void
_bn_sub_ul(const big_number_t *bn, unsigned long n, big_number_t *out)
{
    bool is_bn_biggest = true;
    int bn_cmp_n = bn_cmp_ul(bn, n);

    if (bn_cmp_n == 0) {
        bn_set_from_l(0, out);
        return;
    } else if (bn_cmp_n < 0) {
        is_bn_biggest = false;
    }

    if (bn->positive_number) {
        out->positive_number = is_bn_biggest;
    } else {
        out->positive_number = !is_bn_biggest;
    }

    if (bn->limit > n) {
        __bn_sub_ul(bn, n, 0, is_bn_biggest, out);
    } else {
        big_number_t tmp;

        bn_init_with_args(&tmp, 0, bn->limit);
        bn_set_from_ul(n, &tmp);

        if (is_bn_biggest) {
            _bn_sub_bn(bn, &tmp, out);
        } else {
            _bn_sub_bn(&tmp, bn, out);
        }

        bn_wipe(&tmp);
    }
}

/* }}} */

void bn_add_ul(const big_number_t *bn, unsigned long n, big_number_t *out)
{
    if (bn->parts.len == 0) {
        bn_set_from_ul(n, out);
        return;
    }

    if (bn != out) {
        bn_fast_clear(out);

        if (bn->limit != out->limit) {
            bn_set_limit(out, bn->limit);
        }
    }

    if (bn->positive_number) {
        _bn_add_ul(bn, n, out);
        out->positive_number = true;
    } else {
        /* bn here is not a positive one, but adding a positive number to
         * a negative number is equivalent to substracting a positive
         * number to another one */
        _bn_sub_ul(bn, n, out);
    }
}

void bn_sub_ul(const big_number_t *bn, unsigned long n, big_number_t *out)
{
    if (bn->parts.len == 0) {
        bn_set_from_ul(n, out);
        out->positive_number = false;

        return;
    }

    if (bn != out) {
        bn_fast_clear(out);

        if (bn->limit != out->limit) {
            bn_set_limit(out, bn->limit);
        }
    }

    if (bn->positive_number) {
        _bn_sub_ul(bn, n, out);
    } else {
        /* bn here is not a positive one, but adding a positive number to
         * a negative number is equivalent to substracting a positive
         * number to another one */
        _bn_add_ul(bn, n, out);
        out->positive_number = false;
    }
}

int bn_add_bn(const big_number_t *bn1, const big_number_t *bn2,
              big_number_t *out)
{
    if (bn1->limit != bn2->limit) {
        logger_error("operations between big numbers with different limit "
                     "has not been yet implemented, %ld - %ld",
                     bn1->parts.len, bn2->parts.len);
        return -1;
    }

    if (bn1 != out && bn2 != out) {
        bn_fast_clear(out);

        if (bn1->limit != out->limit) {
            bn_set_limit(out, bn1->limit);
        }
    }

    if (bn1->positive_number && bn2->positive_number) {
        _bn_add_bn(bn1, bn2, out);
        out->positive_number = true;

        return 0;
    } else if (!bn1->positive_number && !bn2->positive_number) {
        _bn_add_bn(bn1, bn2, out);
        out->positive_number = false;

        return 0;
    }

    logger_fatal("NOT YET IMPLEMENTED");
    return -1;
}

/* }}} */

void bn_add_part(big_number_t *bn, unsigned long val)
{
    gv_add(&(bn->parts), val);
}

char *bn_to_str(const big_number_t *bn)
{
    size_t str_size = bn->parts.len * 18 + 1;
    char *str = RETHROW_P(p_calloc(str_size));
    int charac_written = 0;
    const unsigned long min = bn->limit / 10;
    const unsigned int max_digit_by_part = get_count_digits_of_n(bn->limit) - 1;

    if (!bn->positive_number) {
        charac_written = snprintf(str + 0, str_size - charac_written, "-");
    }

    for (long pos = bn->parts.len - 1; pos >= 0; pos--) {
        int rc;
        unsigned long n = bn->parts.tab[pos];

        if (pos < bn->parts.len - 1 && n < min) {
            unsigned int digits_count = get_count_digits_of_n(n);
            unsigned tmp_max;

            if (digits_count == 0) {
                tmp_max = max_digit_by_part -1;
            } else if (digits_count > max_digit_by_part) {
                logger_fatal("'digits_count' (%d) should never be greater "
                             "than 'max_digit_by_part' (%d)",
                             digits_count, max_digit_by_part);
            } else {
                tmp_max = max_digit_by_part - digits_count;
            }

            for (unsigned int i = 0; i < tmp_max; i++) {
                rc = snprintf(str + charac_written, str_size - charac_written,
                              "0");
                charac_written += rc;
            }
        }
        rc = snprintf(str + charac_written, str_size - charac_written,
                      "%ld", n);

        charac_written += rc;
    }

    return str;
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
