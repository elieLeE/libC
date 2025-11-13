#include <time.h>

#include "test_tab_helper.h"
#include "../src/tab_helper/tab_helper.h"
#include "../src/macros.h"

static void test_tab_helper_shuffle(void)
{
    int tab[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int expected_tab[10] = {5, 8, 1, 9, 6, 4, 10, 3, 7, 2};

    /* set the seed to 0 in order to have always the same number from 'rand'
     * method and so be able to check the shuffle algorithm */
    srand(0);

    shuffle_tab((void **)&tab, 10, sizeof(int));

    /* As the seed of the rand is always set to 0 in this test, the shuffle of
     * the tab will be always the same */
    for (int i = 0; i < 10; i++) {
        ASSERT_EQUAL(tab[i], expected_tab[i]);
    }

    /* reset the seed */
    srand(time(NULL));
}

module_tests_t *get_all_tests_tab_helper(void)
{
    module_tests_t *module_tests = module_tests_new();

    set_module_name(module_tests, "TAB_HELPER");
    ADD_TEST_TO_MODULE(module_tests, test_tab_helper_shuffle);

    return module_tests;
}
