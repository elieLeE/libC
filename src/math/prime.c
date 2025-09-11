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
    unsigned int i = 2, j;

    while(i < lim) {
        while(tab[i]) {
            i++;
        }

        j = i + i;
        while(j < lim) {
            tab[j] = true;
            j = j + i;
        }
        i++;
    }
}

void remplissage_tab_prime(unsigned int* tabPrime, unsigned int lim)
{
    unsigned int i, j;
    bool *tab_bool;

    tab_bool = p_calloc(lim * sizeof(bool));

    remplissage_tab_prime_bool(tab_bool, lim);

    j = 1;
    tabPrime[0] = 2;
    for(i = 3; i <= lim; i = i + 2) {
        if(!tab_bool[i]) {
            tabPrime[j] = i;
            j++;
        }
    }

    free(tab_bool);
}


