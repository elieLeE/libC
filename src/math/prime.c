#include "prime.h"

bool isPrime(unsigned long n){
    unsigned long i;
    if(n%2==0){
	return false;
    }
    i = 3;
    while(i*i<=n){
	if(n%i == 0){
	    return false;
	}
	i = i+2;
    }
    return true;
}

void remplissageTabPrimeBool(bool *tab, unsigned int lim){
    unsigned int i = 2, j;
    while(i<lim){
	while(tab[i]){
	    i++;
	}
	j = i+i;
	while(j<lim){
	    tab[j] = true;
	    j = j + i;
	}
	i++;
    }
}

void remplissageTabPrime(unsigned int* tabPrime, unsigned int lim){
    unsigned int i, j;
    //bool *tabBool = my_calloc(lim*sizeof(bool));
    bool *tabBool = calloc(lim, sizeof(bool));

    remplissageTabPrimeBool(tabBool, lim);

    j = 1;
    tabPrime[0] = 2;
    for(i=3; i<=lim; i=i+2){
	if(!tabBool[i]){
	    tabPrime[j] = i;
	    j++;
	}
    }
    free(tabBool);
}


