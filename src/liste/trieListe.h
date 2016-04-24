#ifndef _TRIE_LISTE_H_
#define _TRIE_LISTE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "liste.h"

/*
 * implementation trie par fusion sur les listes
 * */

void trieListeFusion(liste *l, bool sensCroissant);
void division(liste *l);
void fusion(liste *l, bool sensCroissant);

#endif
