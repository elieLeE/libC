#include <stdbool.h>

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

int get_digits_iterations_from_number(unsigned int n, unsigned char *tab)
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

bool are_permutation_nbers(unsigned int n, unsigned int n2)
{
    unsigned char digits_n[10] = {0};
    unsigned char digits_n2[10] = {0};
    unsigned int digits_count_n, digits_count_n2;

    digits_count_n = get_digits_iterations_from_number(n, digits_n);
    digits_count_n2 = get_digits_iterations_from_number(n2, digits_n2);

    if (digits_count_n != digits_count_n2) {
        return false;
    }

    for (unsigned char i = 0; i < 10; i++) {
        if (digits_n[i] != digits_n2[i]) {
            return false;

        }
    }
    return true;
}

unsigned long
build_number_from_digits(unsigned char digits[10], int first_idx, int last_idx)
{
    unsigned long n = 0;

    for (int i = first_idx; i < last_idx; i++) {
       n =  (n * 10) + digits[i];
    }
    return n;
}

