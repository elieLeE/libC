#include <stdlib.h>
#include <math.h>

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
static void fill_tab_prime_bool(unsigned int lim, bool out[])
{
    unsigned int i, j;

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
    gv_for_each_pos(pos, primes) {
        unsigned long current_prime = primes->tab[pos];
        /* first search if n is in the tab ?? => if yes => just him and 1 */

        if (n % current_prime == 0) {
            prime_factor_t *prime_factor;

            prime_factor = gv_grow1(out);
            prime_factor->prime = current_prime;
            prime_factor->iteration++;

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

unsigned long get_divisors_count(const gv_t(primes_factors) *primes_factors)
{
    unsigned long count = 1;

    gv_for_each_pos(pos, primes_factors) {
        count *= (primes_factors->tab[pos].iteration + 1);
    }
    return count;
}

static int
get_first_diviseur_idx(unsigned long nbre, const unsigned long primes[],
                       unsigned int idx_max, unsigned int *out)
{
    unsigned int i = 0;

    while (primes[i] <= nbre && i <= idx_max) {
        if (nbre % primes[i] == 0) {
            *out = i;
            return 0;
        }
        i++;
    }
    return -1;
}

unsigned int get_phi(unsigned long n, const unsigned long primes[],
                     unsigned int idx_max, bool stop_on_firt_divisor)
{
    unsigned int debut;
    double num = n * 1.0, denom = 1.0;

    if (get_first_diviseur_idx(n, primes, idx_max, &debut) == 0) {
        unsigned int i, divisor;
        double other_divisor;

        divisor = primes[debut];
        other_divisor = ((double)n) / divisor;

        num = num * (divisor - 1) * ((unsigned int)(other_divisor) - 1);
        denom *= divisor * (unsigned int)other_divisor;

        if (!stop_on_firt_divisor) {
            for (i = debut + 1; (i * divisor <= n) & (i < idx_max); i++) {
                divisor = primes[i];
                other_divisor = ((double)n) / ((double)divisor);

                if (floor(other_divisor) == other_divisor) {
                    num = num * (divisor - 1) *
                        ((unsigned int)(other_divisor) - 1);
                    denom *= divisor * (unsigned int)other_divisor;
                }
            }
        }
        return (unsigned int)(num / denom);
    }

    // prime number.
    return n - 1;
}
