#include <strings.h>
#include <getopt.h>

#include "test_helper.h"
#include "../macros.h"

int parse_args(int argc, char **argv, void (*usage_cb)(const char *),
               char **module_name, char **test_name)
{
    int opt;

    while ((opt = getopt(argc, argv, "hm:t:")) != -1) {
        switch(opt) {
        case 'h':
            usage_cb(argv[0]);
            return -1;

        case 'm':
            *module_name = optarg;
            break;

        case 't': {
            if (*module_name == NULL) {
                usage_cb(argv[0]);
                return -1;
            }
            *test_name = optarg;

            break;
        }

        default:
            usage_cb(argv[0]);
            return -1;
        }
    }

    return 0;
}

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

static int run_all_tests_of_module(const module_tests_t *module_tests,
                                   const char *test_name)
{
    logger_test_begin_module(module_tests->name);

    gl_for_each(elem, module_tests->tests.first) {
        test_t *test = elem->data;

        if (test->name == NULL) {
            logger_error("a test name is NULL in module %s",
                         module_tests->name);
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
        } else {
            run_test(elem->data);
        }
    }

    logger_test_end_module(module_tests->name);

    if (test_name != NULL) {
        logger_error("the test '%s' has not been found", test_name);
        printf("Here are the available tests in the module '%s' "
               "(the case has none effect):\n",
               module_tests->name);
        gl_for_each(elem, module_tests->tests.first) {
            module_tests_t *test = elem->data;

            printf("'%s'\n", test->name);
        }
    }

    return 0;
}

int _run_all_modules_tests(const generic_liste_t *modules_tests,
                           const char *module_name, const char *test_name)
{
    bool modules_found = false;

    gl_for_each(elem, modules_tests->first) {
        module_tests_t *module_tests = elem->data;

        if (module_tests->name == NULL) {
            logger_error("the name of a module is NULL");
            return -1;
        }

        if (module_name != NULL) {
            if (strncasecmp(module_tests->name, module_name,
                            strlen(module_name)) == 0)
            {
                modules_found = true;
                RETHROW(run_all_tests_of_module(module_tests, test_name));
            }
        } else {
            RETHROW(run_all_tests_of_module(elem->data, test_name));
        }
    }

    if (module_name != NULL && !modules_found) {
        logger_error("the module '%s' has not been found", module_name);
        printf("Here are the available modules (the case has none effect):\n");
        gl_for_each(elem, modules_tests->first) {
            module_tests_t *m = elem->data;

            printf("'%s'\n", m->name);
        }
    }

    return 0;
}

int run_all_modules_tests(const generic_liste_t *modules_tests,
                          const char *module_name, const char *test_name)
{
    char *complete_test_name = NULL;
    size_t n;
    int res;

    if (test_name == NULL) {
        return _run_all_modules_tests(modules_tests, module_name, test_name);
    }

    if (strncmp(test_name, "test_", strlen("test_")) == 0) {
        return _run_all_modules_tests(modules_tests, module_name, test_name);
    }

    n = strlen(test_name) + 1 + 5;
    complete_test_name = p_malloc(n);
    snprintf(complete_test_name, n, "test_%s", test_name);

    res = _run_all_modules_tests(modules_tests, module_name,
                                 complete_test_name);

    p_free((void **)&complete_test_name);

    return res;
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

    gl_wipe(&module_tests->tests, free_test_data);
    p_free((void **)&module_tests->name);
    p_free((void **)&module_tests);
}

void free_all_module_test(generic_liste_t *modules_tests)
{
    gl_wipe(modules_tests, free_module_test);
}
