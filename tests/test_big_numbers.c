#include "test_big_numbers.h"

#include "../src/macros.h"

module_tests_t *get_all_tests_big_numbers(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "BIG_NUMBERS");

    return module_tests;
}
