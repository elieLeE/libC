#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
/* not necessary for compiling but without vim indicates an error and that's
 * annoying */
#include <getopt.h>

#include "../src/macros.h"
#include "../src/utils.h"
#include "../src/test_helper/test_helper.h"

#include "test_liste.h"
#include "test_io.h"
#include "test_prime.h"
#include "test_vector.h"
#include "test_tab_helper.h"
#include "test_calculs.h"
#include "test_big_numbers.h"

static void test_swap(void)
{
    int a = 5;
    int b = 13;

    SWAP(int, &a, &b);

    ASSERT_EQUAL_INT(a, 13);
    ASSERT_EQUAL_INT(b, 5);
}

static module_tests_t *get_all_tests_without_modules(void)
{
    module_tests_t *module_tests = module_tests_new();

    set_module_name(module_tests, "WITHOUT MODULE");
    ADD_TEST_TO_MODULE(module_tests, test_swap);

    return module_tests;
}

static int get_all_tests(generic_liste_t *modules_tests)
{
    module_tests_t *module_tests;

    module_tests = RETHROW_PN(get_all_tests_without_modules());
    gl_add_elem_last(modules_tests, module_tests);

    module_tests = RETHROW_PN(get_all_tests_liste());
    gl_add_elem_last(modules_tests, module_tests);

    module_tests = RETHROW_PN(get_all_tests_io());
    gl_add_elem_last(modules_tests, module_tests);

    module_tests = RETHROW_PN(get_all_tests_prime());
    gl_add_elem_last(modules_tests, module_tests);

    module_tests = RETHROW_PN(get_all_tests_vector());
    gl_add_elem_last(modules_tests, module_tests);

    module_tests = RETHROW_PN(get_all_tests_tab_helper());
    gl_add_elem_last(modules_tests, module_tests);

    module_tests = RETHROW_PN(get_all_tests_calculs());
    gl_add_elem_last(modules_tests, module_tests);

    module_tests = RETHROW_PN(get_all_tests_big_numbers());
    gl_add_elem_last(modules_tests, module_tests);

    return 0;
}

static void print_usage(const char *argv0)
{
    printf("usage: %s [-m 'module name' [-t 'test name']]\n", argv0);
}

int main(int argc, char **argv)
{
    int opt;
    generic_liste_t modules_tests;
    const char *module_name = NULL;
    const char *test_name = NULL;

    srand(time(NULL));

    while ((opt = getopt(argc, argv, "hm:t:")) != -1) {
        switch(opt) {
        case 'h':
            print_usage(argv[0]);
            return 0;

        case 'm':
            module_name = optarg;
            break;

        case 't': {
            if (module_name == NULL) {
                print_usage(argv[0]);
                return 0;
            }
            test_name = optarg;
        }
            break;

        default:
            print_usage(argv[0]);
            return 0;
        }
    }

    gl_init(&modules_tests);

    if (get_all_tests(&modules_tests) < 0) {
        logger_error("error when getting the tests callback");
    }
    run_all_modules_tests(&modules_tests, module_name, test_name);

    free_all_module_test(&modules_tests);

    return 0;
}
