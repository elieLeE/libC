#include "test_vector.h"
#include "../src/vector/vector.h"
#include "../src/macros.h"

static void check_vector_data(const gv_t(int32) *vec,
                              const int32_t expected_vals[],
                              int expected_vals_count)
{
    int idx_tab = 0;

    ASSERT_EQUAL(vec->len, expected_vals_count);

    gv_for_each_pos(pos, vec) {
        ASSERT_EQUAL(vec->tab[pos], expected_vals[idx_tab]);
        idx_tab++;
    }
}

static void test_add_elem_last(void)
{
    gv_t(int32) vector;
    int32_t tab[5] = {1, 2, 3, 4, 5};

    gv_init(&vector);

    gv_add_elem_last(&vector, 1);
    gv_add_elem_last(&vector, 2);
    gv_add_elem_last(&vector, 3);
    gv_add_elem_last(&vector, 4);
    gv_add_elem_last(&vector, 5);

    check_vector_data(&vector, tab, 5);

    gv_free(&vector, NULL);
}


module_tests_t *get_all_tests_vector(void)
{
    module_tests_t *module_tests = RETHROW_P(p_calloc(sizeof(module_tests_t)));

    set_module_name(module_tests, "VECTOR");
    ADD_TEST_TO_MODULE(module_tests, test_add_elem_last);

    return module_tests;
}
