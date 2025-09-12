#include <stdio.h>
#include <stdlib.h>
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

unsigned int remplissage_tab_prime(unsigned int* tabPrime, unsigned int lim)
{
    unsigned int i, j;
    bool *tab_bool;

    tab_bool = p_calloc(lim * sizeof(bool));

    remplissage_tab_prime_bool(tab_bool, lim);

    j = 1;
    tabPrime[0] = 2;
    for (i = 3; i <= lim; i = i + 2) {
        if (!tab_bool[i]) {
            tabPrime[j] = i;
            j++;
        }
    }

    free(tab_bool);

    return j - 1;
}


