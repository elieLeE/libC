#include "../../src/macros.h"
#include "../shared/ftest.h"
#include "test.h"

int main()
{
    BEGIN_TEST_MODULE("IO");

    CALL_TEST_FUNC(test_lecture_simple);
    CALL_TEST_FUNC(test_lecture_matrice);

    END_TEST_MODULE();
    return 0;
}
