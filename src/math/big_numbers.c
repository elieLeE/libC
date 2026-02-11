#include "big_numbers.h"

#include "../mem/mem.h"
#include "nber_helper.h"

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

/* {{{ Setting methods */

void bn_set_from_bn(const big_number_t * const src, big_number_t *dst)
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
    if (n > 0) {
        bn_set_from_ul(n, out);
    } else {
        bn_set_from_ul(-n, out);
    }

    if (n < 0) {
        out->positive_number = false;
    }
}

/* }}} */
/* {{{ Adding methods */

static void
_bn_pos_add_ul(const big_number_t * const bn, unsigned long n,
               long first_idx, big_number_t *out)
{
    unsigned long tmp;
    long idx_part = first_idx;

    if (out->parts.size < bn->parts.len) {
        gv_extend(&out->parts, bn->parts.len - out->parts.len);
    }

    /* len of the vector have been checked in the calling method */
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

static int
bn_add_pos_pos(const big_number_t * const bn1,
               const big_number_t * const bn2, big_number_t *out)
{
    unsigned long carry = 0;
    const big_number_t * const shortest_bn =
        bn1->parts.len >= bn2->parts.len ? bn2 : bn1;
    const big_number_t * const longest_bn =
        bn1->parts.len >= bn2->parts.len ? bn1 : bn2;

    if (out != longest_bn && out != shortest_bn) {
        bn_set_from_bn(longest_bn, out);
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
        return 0;
    }

    if (longest_bn->parts.len == shortest_bn->parts.len) {
        gv_add(&out->parts, carry);
        return 0;
    }

    _bn_pos_add_ul(longest_bn, carry, shortest_bn->parts.len, out);

    return 0;
}

int bn_add(const big_number_t * const bn1, const big_number_t * const bn2,
           big_number_t *out)
{
    if (bn1->limit != bn2->limit) {
        logger_error("operations between big numbers with different limit "
                     "has not been yet implemented, %ld - %ld",
                     bn1->parts.len, bn2->parts.len);
        return -1;
    }

    if (bn1->positive_number && bn2->positive_number) {
        return bn_add_pos_pos(bn1, bn2, out);
    } else if (!bn1->positive_number && !bn2->positive_number) {
        out->positive_number = false;
        return bn_add_pos_pos(bn1, bn2, out);
    }

    logger_fatal("NOT YET IMPLEMENTED");
    return -1;
}

void bn_add_ul(big_number_t *bn, unsigned long n, big_number_t *out)
{
    if (bn->parts.len == 0) {
        bn_set_from_ul(n, out);
    } else {
        if (bn->limit != out->limit) {
            bn_set_limit(out, bn->limit);
        }

        if (bn->positive_number) {
            _bn_pos_add_ul(bn, n, 0, out);
        } else {
            logger_fatal("NOT YET IMPLEMENTED");
        }
    }
}

/* }}} */

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
