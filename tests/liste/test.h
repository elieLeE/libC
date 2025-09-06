#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include "../../src/liste/liste.h"
#include "../../src/liste/addElement.h"
#include "../../src/liste/removeElement.h"
#include "../../src/liste/trieListe.h"
#include "verif.h"

void testAddAndRemoveElement(liste *l);
void testAddElementTrieC(liste *l);
void testAddElementTrieD(liste *l);
void testRemoveElement(liste *l);
void testTriageListe(liste *l);

#endif
