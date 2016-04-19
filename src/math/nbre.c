#include "nbre.h"

unsigned long min(unsigned long a, unsigned long b){
    if(a < b){
	return a;
    }
    return b;
}

unsigned long max(unsigned long a, unsigned long b){
    if(a > b){
	return a;
    }
    return b;
}

int nbreDigits(int n){
    int i = 1;
    int compt = 0;
    while(n>=i){
	i = i*10;
	compt++;
    }
    return compt;
}

void nbreToDigits(unsigned int n, unsigned int *tab){
    unsigned int n2, i = 0;
    while(n>=1){
	n2 = n/10;
	tab[i] = (n - n2*10);
	n = n/10;
    }
}
