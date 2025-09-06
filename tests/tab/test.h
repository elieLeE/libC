#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../src/tab/tab.h"
#include "../../src/tab/trieTab.h"
#include "fonctionTest.h"

#define TAILLE_TEST 1000000

bool testTrieFusion(int (*comparElement)(void const*, void const*));
bool testSearch(int (*comparElement)(void const*, void const*), void(*visuElem)(void const*));

void testQSort(int (*comparElement)(void const*, void const*));

#endif
