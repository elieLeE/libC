#include <stdbool.h>

#include "nber_helper.h"

unsigned int count_digits_in_nber(unsigned long n)
{
    unsigned long i = 1;
    unsigned int compt = 0;

    while (n >= i) {
        i = i * 10;
        compt++;
    }
    return compt;
}

void get_digits_from_number(unsigned long n, gv_t(uint8) *out)
{
    while (n >= 1) {
        unsigned long n2 = n / 10;

        gv_add(out, (n - n2 * 10));
        n = n / 10;
    }
}

int get_digits_iterations_from_number(unsigned long n, unsigned char *out)
{
    unsigned int i = 0;

    while (n >= 1) {
        unsigned long n2 = n / 10;

        out[(n - n2 * 10)]++;
        n = n / 10;
        i++;
    }

    return i;
}

bool is_nber_a_palindrome(unsigned long n)
{
    bool res = true;
    gv_t(uint8) digits;
    unsigned int nbre_digits;

    gv_init(&digits);

    get_digits_from_number(n, &digits);

    nbre_digits = digits.len;

    for (unsigned int i = 0; i < digits.len / 2; i++) {
        if (digits.tab[i] != digits.tab[nbre_digits - i - 1]) {
            res = false;
            break;
        }
    }

    gv_wipe(&digits, NULL);

    return res;
}

bool are_permutation_nbers(unsigned long n, unsigned long n2)
{
    /* do not use vector here as the size of the vector is absolute (digits
     * are < 10) and it is more efficient */
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
    unsigned long n = digits[first_idx];

    for (unsigned int i = first_idx + 1; i <= last_idx; i++) {
       n =  (n * 10) + digits[i];
    }
    return n;
}

unsigned long build_number_from_digits_rev(const unsigned char digits[],
                                           int first_idx, int last_idx)
{
    unsigned long n = digits[last_idx];

    for (int i = last_idx - 1; i >= first_idx; i--) {
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

