#include <stdlib.h>

#include "prime.h"
#include "../mem/mem.h"
#include "../macros.h"

bool is_prime(unsigned long n)
{
    unsigned long i;

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
        i = i+2;
    }
    return true;
}

/* This methods is very efficient but needs an array with all numbers to lim */
static void fill_tab_prime_bool(unsigned long lim, bool out[])
{
    unsigned long i, j;

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
            /* Same the previously: we skip the even numbers */
            j = j + 2 * i;
        }
        i += 2;
    }
}

long get_all_primes_below_n(unsigned long lim, gv_t(uint64) *out)
{
    bool *tab_bool;

    tab_bool = RETHROW_PN(p_calloc(lim * sizeof(bool)));

    fill_tab_prime_bool(lim, tab_bool);

    gv_add(out, 2);
    for (unsigned long i = 3; i <= lim; i = i + 2) {
        if (!tab_bool[i]) {
            gv_add(out, i);
        }
    }

    free(tab_bool);

    return out->len;
}

void get_all_n_first_primes(long count_asked, gv_t(uint64) *out)
{
    unsigned long current_nber;

    gv_add(out, 2);
    current_nber = 3;

    while (out->len < count_asked) {
        bool current_nber_is_prime = true;

        gv_for_each_pos(pos, out) {
            if (current_nber % out->tab[pos] == 0) {
                current_nber_is_prime = false;
                break;
            }
        }

        if (current_nber_is_prime) {
            gv_add(out, current_nber);
        }
        current_nber += 2;
    }
}

unsigned long
get_all_primes_factors_of_n(unsigned long n, const gv_t(uint64) *primes,
                            gv_t(primes_factors) *out)
{
    if (n == 0) {
        return 0;
    }

    if (n == 1) {
        return 1;
    }

    gv_for_each_pos(pos, primes) {
        unsigned long current_prime = primes->tab[pos];
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
                                             long basis, long first_pos,
                                             gv_t(uint64) *out)
{
    for (int pos = first_pos; pos < in->len; pos++) {
        prime_factor_t *current_prime_factor = &(in->tab[pos]);
        long current_div = current_prime_factor->prime * basis;

        for (unsigned int i = 0; i < current_prime_factor->iteration; i++) {
            gv_add(out, current_div);
            get_all_divisors_of_n_from_prime_factors_rec(in, current_div,
                                                         pos + 1, out);
            current_div *= current_prime_factor->prime;
        }
    }
}

void get_all_divisors_of_n_from_prime_factors(const gv_t(primes_factors) *in,
                                              gv_t(uint64) *out)
{
    gv_for_each_pos(pos, in) {
        prime_factor_t *current_prime_factor = &(in->tab[pos]);
        long current_div = current_prime_factor->prime;

        for (unsigned int i = 0; i < current_prime_factor->iteration; i++) {
            gv_add(out, current_div);
            get_all_divisors_of_n_from_prime_factors_rec(in, current_div,
                                                         pos + 1, out);
            current_div *= current_prime_factor->prime;
        }
    }
}

unsigned long get_divisors_count(const gv_t(primes_factors) *primes_factors)
{
    unsigned long count = 1;

    gv_for_each_pos(pos, primes_factors) {
        count *= (primes_factors->tab[pos].iteration + 1);
    }
    return count;
}

unsigned long
get_phi_from_primes_factors(unsigned long n,
                            const gv_t(primes_factors) *primes_factors)
{
    unsigned long res = n;

    gv_for_each_pos(pos, primes_factors) {
        unsigned long current_prime = primes_factors->tab[pos].prime;

        res = res * (current_prime - 1) / current_prime;
    }

    return res;
}

void get_all_phi_from_1_to_n(long n, gv_t(int64) *phi)
{
    if (phi->len <= n) {
        gv_extend(phi, n + 1 - phi->len);
    }
    for (long i = 0; i <= n; i++) {
        gv_add(phi, i);
    }
    for (long i = 2; i <= n; i++) {
        if (phi->tab[i] == i) {
            for (long j = i; j <= n; j += i) {
                phi->tab[j] = phi->tab[j] * (i - 1) / i;
            }
        }
    }
}
