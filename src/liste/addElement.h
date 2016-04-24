#ifndef _ADD_ELEMENT_H_
#define _ADD_ELEMENT_H_

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "../mem/alloc.h"
#include "type.h"
#include "liste.h"

void addElementFirst(liste* l, void *e);
void addElementLast(liste *l, void *e);
void addElementTrie(liste *l, void *e, bool sensCroissant);
void addElementTrieC(liste *l, void *e);
void addElementTrieD(liste *l, void *e);
void addElementNext(element *e, void *data);

#endif
