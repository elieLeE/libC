#ifndef __TAB_HELPER_H__
#define __TAB_HELPER_H__

#include <stdlib.h>

/* Warning, this method uses "rand". Be sure having set the seed !! */
void suffle_tab(void *tab, unsigned int size_tab, size_t size_elem);

#endif
