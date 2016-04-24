#include "addElement.h"

void addElementFirst(liste* l, void *data){
    element* new = my_calloc(sizeof(*new));
    new->data = data;
    new->suiv = l->first;
    new->prec = NULL;
    if(!listeEmpty(l)){
	l->first->prec = new;
    }
    else{
	l->end = new;
    }
    l->first = new;
    (l->nbreElement)++;
}

void addElementLast(liste *l, void *data){
    element* new = my_calloc(sizeof(*new));
    new->data = data;
    new->suiv = NULL;
    new->prec = l->end;
    if(!listeEmpty(l)){
	l->end->suiv = new;
    }
    else{
	l->first = new;
    }
    l->end = new;
    (l->nbreElement)++;
}

void addElementTrie(liste *l, void *data, bool sensCroissant){
    if(!(l->comparElement == NULL)){
	if(listeEmpty(l)){
	    addElementFirst(l, data);
	}
	else{
	    if(sensCroissant){
		addElementTrieC(l, data);
	    }
	    else{
		addElementTrieD(l, data);
	    }
	}
    }
    else{
	fprintf(stderr, "comparElement NULL\n");
    }
}

void addElementTrieC(liste *l, void *data){
    if((*(l->comparElement))(l->first->data, data) >= 0){
	addElementFirst(l, data);
    }
    else if((*(l->comparElement))(l->end->data, data) <= 0){
	addElementLast(l, data);
    }
    else{
	element *p = l->first;
	if((*(l->comparElement))(data, l->first->data) > 0){
	    while((!elemEmpty(p->suiv)) && ((*(l->comparElement))(p->suiv->data, data) < 0)){
		p = p->suiv;
	    }
	}
	addElementNext(p, data);
	(l->nbreElement)++;
    }
}

void addElementNext(element *e, void *data){
    /*printf("\ndans addElementNext : \n");
    printf("e : %d, e->suiv : %d\n", *((int*)e->data), *((int*)e->suiv->data));*/
    element *new = my_calloc(sizeof(*new));
    new->data = data;
    new->suiv = e->suiv;
    new->prec = e;
    e->suiv->prec = new;
    e->suiv = new;
    //printf("\n");
}

void addElementTrieD(liste *l, void *data){
    if((*(l->comparElement))(l->first->data, data) <= 0){
	addElementFirst(l, data);
    }
    else if((*(l->comparElement))(l->end->data, data) >= 0){
	addElementLast(l, data);
    }
    else{
	element *p = l->first;
	if((*(l->comparElement))(data, l->first->data) < 0){
	    while((!elemEmpty(p->suiv)) && ((*(l->comparElement))(p->suiv->data, data) > 0)){
		p = p->suiv;
	    }
	}
	addElementNext(p, data);
	(l->nbreElement)++;
    }
}

