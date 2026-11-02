#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdlib.h>

/* Warning, this method uses "rand". Be sure having set the seed !! */
__attribute__((nonnull))
void ga_shuffle(void *tab, int32_t size_tab, size_t size_elem);

#endif
