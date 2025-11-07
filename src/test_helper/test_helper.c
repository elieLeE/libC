#include <strings.h>

#include "test_helper.h"
#include "../macros.h"

int set_module_name(module_tests_t *module_test, const char *name)
{
    /* Has to allocate place for character '\0' => + 1 */
    module_test->name = RETHROW_PN(p_calloc(strlen(name) + 1));

    strcpy(module_test->name, name);
    return 0;
}

int add_test(module_tests_t *module_test, const char *test_name,
             void (*test_cb)(void))
{
    test_t *new_test = RETHROW_PN(p_calloc(sizeof(test_t)));

    /* Has to allocate place for character '\0' => + 1 */
    new_test->name = RETHROW_PN(p_calloc(strlen(test_name) + 1));
    strcpy(new_test->name, test_name);

    new_test->test_cb = test_cb;

    gl_add_elem_last(&module_test->tests, new_test);

    return 0;
}

static void run_test(test_t *test)
{
    logger_test_start(test->name);
    test->test_cb();
    logger_test_ok(test->name);
}

static int run_all_tests_of_module(const module_tests_t *module_test,
                                   const char *test_name)
{
    logger_test_begin_module(module_test->name);

    gl_for_each(elem, module_test->tests.first) {
        test_t *test = elem->data;

        if (test->name == NULL) {
            logger_error("a test name is NULL in module %s",
                         module_test->name);
            return -1;
        }

        if (test->test_cb == NULL) {
            logger_error("the test %s has its callback NULL", test->name);
            return -1;
        }

        if (test_name != NULL) {
            if (strcasecmp(test->name, test_name) == 0) {
                run_test(elem->data);
                return 0;
            }
        }
        run_test(elem->data);
    }

    logger_test_end_module(module_test->name);

    return 0;
}

int run_all_modules_tests(const generic_liste_t *modules_tests,
                          const char *module_name, const char *test_name)
{
    gl_for_each(elem, modules_tests->first) {
        module_tests_t *module_tests = elem->data;

        if (module_tests->name == NULL) {
            logger_error("the name of a module is NULL");
            return -1;
        }

        if (module_name != NULL) {
            if (strcasecmp(module_tests->name, module_name) == 0) {
                return run_all_tests_of_module(module_tests, test_name);
            }
        } else {
            RETHROW(run_all_tests_of_module(elem->data, test_name));
        }
    }

    return 0;
}

static void free_test_data(void *_module_test)
{
    test_t *module_test = _module_test;

    p_free((void **)&module_test->name);
    p_free((void **)&module_test);
}

static void free_module_test(void *_module_tests)
{
    module_tests_t *module_tests = _module_tests;

    gl_free(&module_tests->tests, free_test_data);
    p_free((void **)&module_tests->name);
    p_free((void **)&module_tests);
}

void free_all_module_test(generic_liste_t *modules_tests)
{
    gl_free(modules_tests, free_module_test);
}
