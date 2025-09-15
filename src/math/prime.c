#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "prime.h"
#include "../mem/mem.h"

bool is_prime(unsigned long n)
{
    unsigned long i;

    if(n <= 1) {
        fprintf(stderr, "n <= 1, fichier %s, ligne %d\n", __FILE__, __LINE__);
        return false;
    }

    if(n == 2) {
        return true;
    }

    if(n % 2 == 0) {
        return false;
    }

    i = 3;
    while(i * i <= n) {
        if(n % i == 0) {
            return false;
        }
        i = i+2;
    }
    return true;
}

void remplissage_tab_prime_bool(bool *tab, unsigned int lim)
{
    unsigned int i, j;

    j = 4;
    while (j < lim) {
        tab[j] = true;
        j = j + 2;
    }

    i = 3;
    while (i < lim) {
        while (tab[i]) {
            i += 2;
        }

        /* As i is an odd number, 2 * i will be an even number.
         * But all of them have already been checked on the previous loop.
         * So, skip 2 * i and directly pass to " * i. */
        j = 3 * i;
        while (j < lim) {
            tab[j] = true;
            /* Same the previously: we skip the even numbers */
            j = j + 2 * i;
        }
        i += 2;
    }
}

unsigned int remplissage_tab_prime(unsigned int* tab_prime, unsigned int lim)
{
    unsigned int i, j;
    bool *tab_bool;

    tab_bool = p_calloc(lim * sizeof(bool));

    remplissage_tab_prime_bool(tab_bool, lim);

    j = 1;
    tab_prime[0] = 2;
    for (i = 3; i <= lim; i = i + 2) {
        if (!tab_bool[i]) {
            tab_prime[j] = i;
            j++;
        }
    }

    free(tab_bool);

    return j - 1;
}

static int
get_first_diviseur_idx(unsigned int nbre, unsigned int primes_nber[],
                       unsigned int idx_max, unsigned int *out)
{
    unsigned int i = 0;

    while (primes_nber[i] <= nbre && i <= idx_max) {
        if (nbre % primes_nber[i] == 0) {
            *out = i;
            return 0;
        }
        i++;
    }
    return -1;
}

unsigned int get_phi(unsigned int n, unsigned int primes_nber[],
                     unsigned int idx_max, bool stop_on_firt_divisor)
{
    unsigned int debut;
    double num = n * 1.0, denom = 1.0;

    if (get_first_diviseur_idx(n, primes_nber, idx_max, &debut) == 0) {
        unsigned int i, divisor;
        double other_divisor;

        divisor = primes_nber[debut];
        other_divisor = ((double)n) / divisor;

        num = num * (divisor - 1) * ((unsigned int)(other_divisor) - 1);
        denom *= divisor * (unsigned int)other_divisor;

        if (!stop_on_firt_divisor) {
            for (i = debut + 1; (i * divisor <= n) & (i < idx_max); i++)
            {
                divisor = primes_nber[i];
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

