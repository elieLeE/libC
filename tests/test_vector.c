#include "test_vector.h"
#include "../src/vector/vector.h"
#include "../src/macros.h"

static void test_fill_vector(void)
{
    int idx_tab = 0;
    gv_t(int32) vector;
    int32_t tab[5] = {1, 2, 3, 4, 5};

    gv_init(&vector);

    gv_add_elem_last(&vector, 1);
    gv_add_elem_last(&vector, 2);
    gv_add_elem_last(&vector, 3);
    gv_add_elem_last(&vector, 4);
    gv_add_elem_last(&vector, 5);

    ASSERT_EQUAL(vector.len, 5);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }

    gv_wipe(&vector, NULL);
}


module_tests_t *get_all_tests_vector(void)
{
    module_tests_t *module_tests = RETHROW_P(p_calloc(sizeof(module_tests_t)));

    set_module_name(module_tests, "VECTOR");
    ADD_TEST_TO_MODULE(module_tests, test_fill_vector);

    return module_tests;
}
