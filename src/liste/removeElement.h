#ifndef _REMOVE_ELEMENT_H_
#define _REMOVE_ELEMENT_H_

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "../mem/alloc.h"
#include "type.h"
#include "liste.h"

void removeData(liste const *l, void *data);
void removeFirstElement(liste *l);
void removeLastElement(liste *l);
void removeNextElement(liste *l, element *e);
void removeElementN(liste *l, unsigned int const n);
//void removeElementAdrrData(liste *l, void *d);
//void removeElementDoublons(liste *l);
//void removeElementValData(liste *l, void *data);

#endif
