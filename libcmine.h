#ifndef _LIBC_H_
#define _LIBC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct liste liste;
struct liste{
    struct liste *suiv;
    void *data;
};

/* swap */
extern void echangeGenerique(void* , void* , size_t );

/* IO */
extern FILE* ouv_fichier(char* , char* );
extern void fermer_fichier(FILE **);
extern void lire_matrice(FILE *, unsigned int** , int, int);

/* MATH */
/* NBRE */
extern void nbreToDigits(unsigned int , unsigned int *);
/*extern unsigned long min(unsigned long , unsigned long );
extern unsigned long max(unsigned long , unsigned long );*/
extern int nbreDigits(int );
/* PRIME */
extern bool isPrime(unsigned long );
extern void remplissageTabPrimeBool(bool *, unsigned int );
extern void remplissageTabPrime(unsigned int* , unsigned int );
/* SUM */
extern unsigned long sum1ToN(unsigned long );
extern unsigned long sunCarre(unsigned long );

/* MEM */
extern void* p_calloc(size_t );
extern void* p_realloc(void *, size_t );
extern void** alloc_tab_2d(int , int , int );
extern void free_tab_2d(void**, int );

/* TAB */
extern void visuTab(unsigned int* , unsigned int , void (*)(void *e), char* );
extern int searchInd(void *, int , size_t, void * , int (*)(void*, void*));


/* LISTE */
liste* creerListe();
bool listeEmpty(liste *l);
void addElementFirst(liste** l, void *e);
void addElementLast(liste *l, void* e);
void visuListe(liste *l, void (*visuElement)(void *data), char* s);
void removeFirstElement(liste **l);
void freeListe(liste **l);

#endif





