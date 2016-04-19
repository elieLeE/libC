#ifndef _LIBC_H_
#define _LIBC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* swap */
extern void echangeGenerique(void* , void* , size_t );

/* IO */
extern FILE* ouvFichier(char* , char* );
extern void fermerFichier(FILE **);
extern void lireMatrice(FILE *, unsigned int** , int, int);

/* MATH */
/* NBRE */
extern void nbreToDigits(unsigned int , unsigned int *);
extern unsigned long min(unsigned long , unsigned long );
extern unsigned long max(unsigned long , unsigned long );
extern int nbreDigits(int );
/* PRIME */
extern bool isPrime(unsigned long );
extern void remplissageTabPrimeBool(bool *, unsigned int );
extern void remplissageTabPrime(unsigned int* , unsigned int );
/* SUM */
extern unsigned long sum1ToN(unsigned long );
extern unsigned long sunCarre(unsigned long );

/* MEM */
extern void* my_calloc(size_t );
extern void* my_realloc(void *, size_t );
extern void** allocTab2D(int , int , int );
extern void liberationTab2D(void**, int );

/* TAB */
/*extern void visuTab(unsigned int* , unsigned int , void (*)(void *e), char* );
extern int searchInd(void *, int , size_t, void * , int (*)(void*, void*));
*/
#endif





