#ifndef _TEST_H_
#define _TEST_H_

#include <stdbool.h>

#define TAILLE_TEST 1000000

bool testTrieFusion(int (*comparElement)(void const*, void const*));
bool testSearch(int (*comparElement)(void const*, void const*), void(*visuElem)(void const*));

void testQSort(int (*comparElement)(void const*, void const*));

#endif
