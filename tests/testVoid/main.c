#include <stdio.h>
#include <stdlib.h>

int main(){
    void **tab = malloc(10*sizeof(void*));

    if(tab == NULL){
	printf("erreur allocation\n");
	return 0;
    }

    int a = 2;

    tab[0] = (void*)&a;

    printf("%d\n", *(int *)tab[0]);

    free(tab);

    return 0;
}
	
