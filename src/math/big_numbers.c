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

void bn_set_from_bn(const big_number_t *in, big_number_t *out)
{
    bn_fast_clear(out);

    if (out->parts.size < in->parts.len) {
        gv_extend(&(out->parts), (in->parts.len - out->parts.size));
    }
    memcpy(out->parts.tab, in->parts.tab,
           in->parts.len * in->parts.__size_elem);

    out->parts.len = in->parts.len;

    out->positive_number = in->positive_number;
    bn_set_limit(out, in->limit);
}

void bn_set_from_ul(big_number_t *bn, unsigned long n)
{
    /* not indispensable but I think it is clearer with the definition of
     * this variable */
    unsigned long tmp = n;

    bn_fast_clear(bn);

    while (tmp >= bn->limit) {
        unsigned long carry = tmp / bn->limit;

        tmp -= bn->limit * carry;
        gv_add(&(bn->parts), tmp);

        tmp = carry;
    }
    gv_add(&(bn->parts), tmp);
}

void bn_set_from_l(big_number_t *bn, long n)
{
    if (n > 0) {
        bn_set_from_ul(bn, n);
    } else {
        bn_set_from_ul(bn, -n);
    }

    if (n < 0) {
        bn->positive_number = false;
    }
}

/* }}} */

char *bn_to_str(const big_number_t *bn)
{
    size_t str_size = bn->parts.len * 18 + 1;
    char *str = RETHROW_P(p_calloc(str_size));
    int charac_written = 0;
    const unsigned long min = bn->limit / 10;
    const unsigned int max_digit_by_part = count_digits_in_nber(bn->limit) - 1;

    if (!bn->positive_number) {
        charac_written = snprintf(str + 0, str_size - charac_written, "-");
    }

    for (long pos = bn->parts.len - 1; pos >= 0; pos--) {
        int rc;
        unsigned long n = bn->parts.tab[pos];

        if (pos < bn->parts.len - 1 && n < min) {
            unsigned int digits_count = count_digits_in_nber(n);
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
