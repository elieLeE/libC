#include "nbre.h"

int count_digits_in_nber(int n)
{
    int i = 1;
    int compt = 0;

    while (n >= i) {
        i = i * 10;
        compt++;
    }
    return compt;
}

unsigned int get_digits_from_number(unsigned int n, unsigned char *tab)
{
    unsigned int i = 0;

    while (n >= 1) {
        unsigned int n2 = n / 10;

        tab[i] = (n - n2 * 10);
        n = n / 10;
        i++;
    }

    return i;
}

unsigned int
get_digits_iterations_from_number(unsigned int n, unsigned char *tab)
{
    unsigned int i = 0;

    while (n >= 1) {
        unsigned int n2 = n / 10;

        tab[(n - n2 * 10)]++;
        n = n / 10;
        i++;
    }

    return i;
}

bool is_nber_a_palindrome(unsigned int n)
{
    static unsigned char tab_digit[6];
    unsigned int nbre_digits;

    nbre_digits = get_digits_from_number(n, tab_digit);

    for (unsigned int i = 0; i < nbre_digits / 2; i++) {
        if (tab_digit[i] != tab_digit[nbre_digits - i - 1]) {
            return false;
        }
    }

    return true;
}
