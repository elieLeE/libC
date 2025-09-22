#include <stdbool.h>

#include "nbre.h"

unsigned int count_digits_in_nber(unsigned int n)
{
    unsigned int i = 1;
    unsigned int compt = 0;

    while (n >= i) {
        i = i * 10;
        compt++;
    }
    return compt;
}

unsigned int get_digits_from_number(unsigned int n, unsigned char *out)
{
    unsigned int i = 0;

    while (n >= 1) {
        unsigned int n2 = n / 10;

        out[i] = (n - n2 * 10);
        n = n / 10;
        i++;
    }

    return i;
}

int get_digits_iterations_from_number(unsigned int n, unsigned char *out)
{
    unsigned int i = 0;

    while (n >= 1) {
        unsigned int n2 = n / 10;

        out[(n - n2 * 10)]++;
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

    for (unsigned int i = 0; i < 10; i++) {
        if (digits_n[i] != digits_n2[i]) {
            return false;

        }
    }
    return true;
}

unsigned long
build_number_from_digits(const unsigned char digits[], unsigned int first_idx,
                         unsigned int last_idx)
{
    unsigned long n = 0;

    for (unsigned int i = first_idx; i <= last_idx; i++) {
       n =  (n * 10) + digits[i];
    }
    return n;
}

unsigned long
build_number_from_digits_rev(const unsigned char digits[],
                             unsigned int first_idx, unsigned int last_idx)
{
    unsigned long n = 0;

    for (unsigned int i = last_idx; i >= first_idx; i--) {
       n =  (n * 10) + digits[i];
    }
    return n;
}

bool is_a_increasing_number(unsigned char digits[], unsigned int max_idx)
{
    for (unsigned int i = 0; i < max_idx; i++) {
        if (digits[i] > digits[i + 1]) {
            return false;
        }
    }
    return true;
}

bool is_a_decreasing_number(unsigned char digits[], unsigned int max_idx)
{
    for (unsigned int i = 0; i < max_idx; i++) {
        if (digits[i] < digits[i + 1]) {
            return false;
        }
    }
    return true;
}

bool is_a_bouncy_number(unsigned char digits[], unsigned int max_idx)
{
    return !is_a_increasing_number(digits, max_idx) &&
        !is_a_decreasing_number(digits, max_idx);
}
