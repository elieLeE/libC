#include "nbre.h"

int nbre_digits(int n)
{
    int i = 1;
    int compt = 0;

    while(n >= i) {
        i = i * 10;
        compt++;
    }
    return compt;
}

unsigned int get_digits_from_number(unsigned int n, unsigned char *tab)
{
    unsigned int i = 0;

    while(n >= 1) {
        unsigned int n2 = n / 10;

        tab[i] = (n - n2 * 10);
        n = n / 10;
        i++;
    }

    return i;
}
