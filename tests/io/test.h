#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../src/io/fichier.h"
#include "../../src/mem/alloc.h"

bool comparTab(unsigned int const *tab1, unsigned int const *tab2, unsigned int const col);
bool testLectureSimple();
bool testLectureMatrice();

#endif

