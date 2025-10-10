#include "test.h"
#include "../../src/macros.h"

int main()
{
    BEGIN_TEST_MODULE("LISTE");

    CALL_TEST_FUNC(test_get_elem_data);
    CALL_TEST_FUNC(test_add_and_remove_element);
    CALL_TEST_FUNC(test_add_element_trie);
    CALL_TEST_FUNC(test_remove_element);

    END_TEST_MODULE();

    return 0;
}
