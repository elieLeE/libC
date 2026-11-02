#include <time.h>
#include <stdio.h>

#include "array.h"
#include "../utils.h"
#include "../mem/mem.h"

void shuffle_tab(void *tab, int32_t size_tab, size_t size_elem)
{
    void *tmp = p_malloc(size_elem);

    for (int32_t i = size_tab -1; i > 0; i--) {
        int32_t j = rand() % i;
        void *a = tab + i * size_elem;
        void *b = tab + j * size_elem;

        swap_cpy_fast(a, b, tmp, size_elem);
    }

    p_free((void **)&tmp);
}
