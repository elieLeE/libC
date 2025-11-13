#include "utils.h"
#include "mem/mem.h"

void swap_cpy_fast(void *d1, void *d2, void *tmp, size_t size_elem)
{
    memcpy(tmp, d1, size_elem);
    memcpy(d1, d2, size_elem);
    memcpy(d2, tmp, size_elem);
}

void swap_cpy(void *d1, void *d2, size_t size_elem)
{
    void *tmp = p_malloc(size_elem);

    swap_cpy_fast(d1, d2, tmp, size_elem);

    p_free((void **)&tmp);
}
