#include "verif.h"
#include "../../src/mem/alloc.h"

bool verifPointeur(liste l){
    int n = l.nbreElement;
    int i;
    bool b = true;

    int *tab = my_calloc(n * sizeof(*tab));
    int *tab2 = my_calloc(n * sizeof(*tab2));

    element *p = l.first;

    i = 0;
    while(!elemEmpty(p)){
        tab[i] = *((int *)p->data);
        i++;
        p = p->suiv;
    }

    p = l.end;
    i = n-1;
    while(!elemEmpty(p)){
        tab2[i] = *((int*)p->data);
        i--;
        p = p->prec;
    }

    /*printf("\nn : %d\n", n);
      for(i=0; i<n; i++){
      printf("%d, %d\n", tab[i], tab2[i]);
      }*/

    for(i=0; i<n; i++){
        if(tab[i] != tab2[i]){
            b = false;
            break;
        }
    }

    free(tab);
    free(tab2);

    return b;
}
