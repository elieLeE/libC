#ifndef __HELP_TEST_H__
#define __HELP_TEST_H__

 #include "../liste/liste.h"
 #include "../mem/mem.h"

typedef struct test_t {
    char *name;
    void (*test_cb)(void);
} test_t;

typedef struct module_tests_t {
    char *name;
    generic_liste_t tests;
} module_tests_t;

void init_module_test(module_tests_t *module_test);

int set_module_name(module_tests_t *module_test, const char *name);
int add_test(module_tests_t *module_test, const char *test_name,
             void (*test_cb)(void));

int run_all_modules_tests(const generic_liste_t *modules_tests,
                          const char *module_name, const char *test_name);

void free_all_module_test(generic_liste_t *modules_tests);

#define ADD_TEST_TO_MODULE(_module_test, _func)                               \
    do {                                                                      \
       add_test(_module_test, #_func, _func);                                 \
    } while (0);

#endif
