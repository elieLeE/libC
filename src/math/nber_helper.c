#include <stdbool.h>

#include "nber_helper.h"

uint32_t get_count_digits_of_n(uint64_t n)
{
    uint64_t i = 1;
    uint32_t compt = 0;

    while (n >= i) {
        i = i * 10;
        compt++;
    }

    return compt;
}

void get_digits_from_number(uint64_t n, gv_t(uint8) *out)
{
    while (n >= 1) {
        uint64_t n2 = n / 10;

        gv_append(out, (n - n2 * 10));
        n = n / 10;
    }
}

uint32_t get_digits_iterations_from_number(uint64_t n, uint32_t *out)
{
    uint32_t i = 0;

    while (n >= 1) {
        unsigned long n2 = n / 10;

        out[(n - n2 * 10)]++;
        n = n / 10;
        i++;
    }

    return i;
}

bool is_nber_a_palindrome(uint64_t n)
{
    bool res = true;
    gv_t(uint8) digits;
    uint32_t nbre_digits;

    gv_init(&digits);

    get_digits_from_number(n, &digits);

    nbre_digits = digits.len;

    for (int64_t i = 0; i < digits.len / 2; i++) {
        if (digits.tab[i] != digits.tab[nbre_digits - i - 1]) {
            res = false;
            break;
        }
    }

    gv_wipe(&digits, NULL);

    return res;
}

bool are_permutation_nbers(uint64_t n, uint64_t n2)
{
    /* do not use vector here as the size of the vector is absolute (digits
     * are < 10) and it is more efficient */
    uint32_t digits_n[10] = {0};
    uint32_t digits_n2[10] = {0};
    uint32_t digits_count_n, digits_count_n2;

    digits_count_n = get_digits_iterations_from_number(n, digits_n);
    digits_count_n2 = get_digits_iterations_from_number(n2, digits_n2);

    if (digits_count_n != digits_count_n2) {
        return false;
    }

    for (uint32_t i = 0; i < 10; i++) {
        if (digits_n[i] != digits_n2[i]) {
            return false;

        }
    }

    return true;
}

uint64_t build_number_from_digits(const uint8_t digits[],
                                  uint32_t first_idx, uint32_t last_idx)
{
    uint64_t n = digits[first_idx];

    for (uint32_t i = first_idx + 1; i <= last_idx; i++) {
       n =  (n * 10) + digits[i];
    }

    return n;
}

uint64_t build_number_from_digits_rev(const uint8_t digits[],
                                      uint32_t first_idx, uint32_t last_idx)
{
    uint64_t n = digits[last_idx];

    for (uint32_t i = last_idx - 1; i >= first_idx; i--) {
       n =  (n * 10) + digits[i];
    }

    return n;
}

bool is_a_increasing_number(const uint8_t digits[], uint32_t max_idx)
{
    for (uint32_t i = 0; i < max_idx; i++) {
        if (digits[i] > digits[i + 1]) {
            return false;
        }
    }

    return true;
}

bool is_a_decreasing_number(const uint8_t digits[], uint32_t max_idx)
{
    for (uint32_t i = 0; i < max_idx; i++) {
        if (digits[i] < digits[i + 1]) {
            return false;
        }
    }

    return true;
}

bool is_a_bouncy_number(const uint8_t digits[], uint32_t max_idx)
{
    return !is_a_increasing_number(digits, max_idx) &&
        !is_a_decreasing_number(digits, max_idx);
}

