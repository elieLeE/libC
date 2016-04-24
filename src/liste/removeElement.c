#include "removeElement.h"

void removeData(liste const *l, void *data){
    if(l->removeData != NULL){
	(*(l->removeData))(data);
    }
}

void removeFirstElement(liste *l){
    if(!listeEmpty(l)){
	element *p = l->first;
	l->first = l->first->suiv;
	(l->nbreElement)--;
	removeData(l, p->data);
	free(p);
	if(!listeEmpty(l)){
	    l->first->prec = NULL;
	}
    }
}

void removeLastElement(liste* l){
    if(!listeEmpty(l)){
	element *p = l->end;
	l->end = l->end->prec;
	(l->nbreElement)--;
	removeData(l, p->data);
	free(p);
	if(!listeEmpty(l)){
	    l->end->suiv = NULL;
	}
    }
}

void removeElementN(liste *l, unsigned int const n){
    printf("removeElementN NOT YET FONCTIONNAL\n");
    return;
    if(((int)n > 0) && (n <= l->nbreElement)){
	if(n == 1){
	    removeFirstElement(l);
	}
	else if(n == l->nbreElement){
	    removeLastElement(l);
	}
	else{
	    printf("suppresion milieu : %d\n", n);
	    element *p = NULL;
	    unsigned int compt = 1;
	    p = l->first;
	    while(compt<n-1){
		p = p->suiv;
	    }
	    /*if(n<n/2){
		p = l->first;
		while(compt<n-1){
		    p = p->suiv;
		}
	    }
	    else{
		n = l->nbreElement - n;
		p = l->end;
		while(compt<n-1){
		    p = p->prec;
		}
	    }*/
	    removeNextElement(l, p);
	}
    }
    printf("\n");
}

void removeNextElement(liste *l, element *e){
    printf("removeNextElement NOT YET FONCTIONNAL\n");
    return;
    //printf("removeNextElement : e : %d, e->suiv : %d\n", *((int*)e->data), *((int*)e->suiv->data));
    element *p = e->suiv;
    e->suiv = e->suiv->suiv;
    e->suiv->prec = e;
    //printf("removeNextElement : e : %d, e->suiv : %d\n", *((int*)e->data), *((int*)e->suiv->data));
    removeData(l, e->data);
    free(p);
    //printf("sorti removeNextElement\n");
}

