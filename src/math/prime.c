#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "prime.h"
#include "../mem/mem.h"

bool is_prime(unsigned long n)
{
    unsigned long i;

    if (n <= 1) {
        fprintf(stderr, "n <= 1, fichier %s, ligne %d\n", __FILE__, __LINE__);
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

static void fill_tab_prime_bool(unsigned int lim, bool out[])
{
    unsigned int i, j;

    j = 4;
    while (j < lim) {
        out[j] = true;
        j = j + 2;
    }

    i = 3;
    while (i < lim) {
        while (out[i]) {
            i += 2;
        }

        /* As i is an odd number, 2 * i will be an even number.
         * But all of them have already been checked on the previous loop.
         * So, skip 2 * i and directly pass to " * i. */
        j = 3 * i;
        while (j < lim) {
            out[j] = true;
            /* Same the previously: we skip the even numbers */
            j = j + 2 * i;
        }
        i += 2;
    }
}

unsigned int
get_all_primes_below_n(unsigned long lim, unsigned int size_tab_out,
                       unsigned long *out)
{
    unsigned int i, j;
    bool *tab_bool;

    tab_bool = p_calloc(lim * sizeof(bool));

    fill_tab_prime_bool(lim, tab_bool);

    j = 1;
    out[0] = 2;
    for (i = 3; i <= lim; i = i + 2) {
        if (!tab_bool[i]) {
            if (j == size_tab_out) {
                fprintf(stderr, "array is too short, fichier %s, ligne %d\n"
                        "size array: %d, index: %d\n", __FILE__, __LINE__,
                        size_tab_out, j);
                break;
            }
            out[j] = i;
            j++;
        }
    }

    free(tab_bool);

    return j - 1;
}

unsigned int
get_all_n_first_primes(unsigned long lim, unsigned int size_tab_out,
                       unsigned long *out)
{
    unsigned int counter;
    unsigned long current_nber;

    out[0] = 2;
    current_nber = 3;
    counter = 1;

    while (counter < lim && counter < size_tab_out) {
        bool current_nber_is_prime = true;

        for (unsigned int i = 0; i < counter; i++) {
            if (current_nber % out[i] == 0) {
                current_nber_is_prime = false;
                break;
            }
        }

        if (current_nber_is_prime) {
            out[counter] = current_nber;
            counter++;
        }
        current_nber += 2;
    }

    if (counter == lim) {
        return 0;
    }

    fprintf(stderr, "array have been full filled before getting "
            "the %ld° prime, file '%s', line '%d'",
            lim, __FILE__, __LINE__);
    return -1;
}

unsigned int get_all_primes_factors_of_n(unsigned long n,
                                         const unsigned long primes[],
                                         unsigned int primes_tab_size,
                                         unsigned int size_tab_out,
                                         prime_factor_t out[])
{
    unsigned int primes_factors_idx = 0;
    unsigned x = n;

    for (unsigned int i = 0; (i < primes_tab_size) && (i < size_tab_out); i++)
    {
        while (x % ((unsigned long)primes[i]) == 0) {
            out[primes_factors_idx].prime = primes[i];
            out[primes_factors_idx].iteration++;

            x = x / ((unsigned long)primes[i]);
        }
        primes_factors_idx++;
    }

    return primes_factors_idx;
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

