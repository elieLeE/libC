#include <assert.h>

#include "test_prime.h"
#include "../src/math/prime.h"
#include "../src/mem/mem.h"
#include "../src/macros.h"
#include "../libc.h"

static void test_is_prime(void)
{
    assert(is_prime(2));
    assert(!is_prime(4));
    assert(is_prime(5));
    assert(is_prime(79));
    assert(!is_prime(323));
}

module_tests_t *get_all_tests_prime(void)
{
    module_tests_t *module_tests = RETHROW_P(p_calloc(sizeof(module_tests_t)));

    set_module_name(module_tests, "PRIMES");
    ADD_TEST_TO_MODULE(module_tests, test_is_prime);

    return module_tests;

}
