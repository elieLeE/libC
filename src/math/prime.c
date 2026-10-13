#include <stdlib.h>

#include "prime.h"
#include "../mem/mem.h"
#include "../macros.h"

bool is_prime(uint64_t n)
{
    uint64_t i;

    if (n <= 1) {
        logger_error("calling 'is_prime' with a no positive number: %ld", n);
        return false;
    }

    if (n == 2) {
        return true;
    }

    if (n % 2 == 0) {
        return false;
    }

    i = 3;
    while (i * i <= n) {
        if (n % i == 0) {
            return false;
        }
        i += 2;
    }
    return true;
}

bool is_prime_from_smallest_primes(uint64_t n, const gv_t(uint64) *primes)
{
    gv_for_each_pos(pos, primes) {
        if (n % primes->tab[pos] == 0) {
            return false;
        }
    }
    return true;
}

/* This methods is very efficient but needs an array with all numbers to lim */
static void fill_tab_prime_bool(uint64_t lim, bool out[])
{
    uint64_t i, j;

    i = 4;
    while (i < lim) {
        out[i] = true;
        i = i + 2;
    }

    i = 3;
    while (i < lim) {
        while (i < lim && out[i]) {
            i += 2;
        }

        /* As i is an odd number, 2 * i will be an even number.
         * But all of them have already been checked on the previous loop.
         * So, skip 2 * i and directly pass to 3 * i. */
        j = 3 * i;
        while (j < lim) {
            out[j] = true;
            /* Same than previously: we skip the even numbers: i and j are odd.
             * So, 2 * i is even and j + 2 * i is odd. */
            j = j + 2 * i;
        }
        i += 2;
    }
}

int64_t get_all_primes_below_n(uint64_t lim, gv_t(uint64) *out)
{
    bool *tab_bool;

    tab_bool = RETHROW_PN(p_calloc(lim * sizeof(bool)));

    fill_tab_prime_bool(lim, tab_bool);

    gv_append(out, 2);
    for (uint64_t i = 3; i <= lim; i = i + 2) {
        if (!tab_bool[i]) {
            gv_append(out, i);
        }
    }

    free(tab_bool);

    return out->len;
}

void get_all_n_first_primes(int64_t count_asked, gv_t(uint64) *out)
{
    uint64_t current_nber;

    gv_append(out, 2);
    current_nber = 3;

    while (out->len < count_asked) {
        if (is_prime_from_smallest_primes(current_nber, out)) {
            gv_append(out, current_nber);
        }
        current_nber += 2;
    }
}

int64_t
get_all_primes_factors_of_n(uint64_t n, const gv_t(uint64) *primes,
                            gv_t(primes_factors) *out)
{
    if (n == 0) {
        return 0;
    }

    if (n == 1) {
        return 1;
    }

    gv_for_each_pos(pos, primes) {
        uint64_t current_prime = primes->tab[pos];
        /* first search if n is in the tab ?? => if yes => just him and 1 */

        if (n % current_prime == 0) {
            prime_factor_t *prime_factor;

            prime_factor = gv_grow1(out);
            prime_factor->prime = current_prime;
            prime_factor->iteration = 1;

            n = n / current_prime;
            while (n % current_prime == 0) {
                n = n / current_prime;
                prime_factor->iteration++;
            }

            if (n == 1) {
                break;
            }
        }
    }

    return out->len;
}

static void
get_all_divisors_of_n_from_prime_factors_rec(const gv_t(primes_factors) *in,
                                             uint64_t basis,
                                             int64_t first_pos,
                                             gv_t(uint64) *out)
{
    for (int64_t pos = first_pos; pos < in->len; pos++) {
        prime_factor_t *current_prime_factor = &(in->tab[pos]);
        uint64_t current_div = current_prime_factor->prime * basis;

        for (int32_t i = 0; i < current_prime_factor->iteration; i++) {
            gv_append(out, current_div);
            get_all_divisors_of_n_from_prime_factors_rec(in, current_div,
                                                         pos + 1, out);
            current_div *= current_prime_factor->prime;
        }
    }
}

void get_all_divisors_of_n_from_prime_factors(const gv_t(primes_factors) *in,
                                              gv_t(uint64) *out)
{
    /* 1 divides all the numbers */
    gv_append(out, 1);

    gv_for_each_pos(pos, in) {
        prime_factor_t *current_prime_factor = &(in->tab[pos]);
        uint64_t current_div = current_prime_factor->prime;

        for (int32_t i = 0; i < current_prime_factor->iteration; i++) {
            gv_append(out, current_div);
            get_all_divisors_of_n_from_prime_factors_rec(in, current_div,
                                                         pos + 1, out);
            current_div *= current_prime_factor->prime;
        }
    }
}

static int cmp_uint64(const void *_d1, const void *_d2)
{
    const uint64_t *d1 = _d1;
    const uint64_t *d2 = _d2;

    return *d1 - *d2;
}

int
get_all_proper_divisors_of_n_from_prime_factors(uint64_t n,
                                                const gv_t(primes_factors) *in,
                                                gv_t(uint64) *out)
{
    int64_t pos;

    get_all_divisors_of_n_from_prime_factors(in, out);

    pos = gv_find(out, n, GV_SEQUENTIAL_SEARCH, cmp_uint64);
    if (gv_remove(out, pos) < 0) {
        return -1;
    }

    return 0;
}

uint64_t
get_proper_divisors_sum_of_n(uint64_t n, const gv_t(uint64) *primes)
{
    uint64_t sum;
    gv_t(primes_factors) primes_factors;
    gv_t(uint64) divisors;

    gv_init(&primes_factors);
    gv_init(&divisors);

    get_all_primes_factors_of_n(n, primes, &primes_factors);
    get_all_divisors_of_n_from_prime_factors(&primes_factors, &divisors);
    sum = get_all_elem_sum(&divisors);

    gv_wipe(&divisors, NULL);
    gv_wipe(&primes_factors, NULL);

    return sum;
}

uint64_t get_divisors_count(const gv_t(primes_factors) *primes_factors)
{
    uint64_t count = 1;

    gv_for_each_pos(pos, primes_factors) {
        count *= (primes_factors->tab[pos].iteration + 1);
    }

    return count;
}

uint64_t
get_phi_from_primes_factors(uint64_t n,
                            const gv_t(primes_factors) *primes_factors)
{
    uint64_t res = n;

    gv_for_each_pos(pos, primes_factors) {
        uint64_t current_prime = primes_factors->tab[pos].prime;

        res = res * (current_prime - 1) / current_prime;
    }

    return res;
}

void get_all_phi_from_1_to_n(int64_t n, gv_t(int64) *phi)
{
    if (phi->len <= n) {
        gv_extend(phi, n + 1 - phi->len);
    }

    for (int64_t i = 0; i <= n; i++) {
        gv_append(phi, i);
    }

    for (int64_t i = 2; i <= n; i++) {
        if (phi->tab[i] == i) {
            for (int64_t j = i; j <= n; j += i) {
                phi->tab[j] = phi->tab[j] * (i - 1) / i;
            }
        }
    }
}
