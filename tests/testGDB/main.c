#include <stdlib.h>
#include <stdio.h>

int max(int a, int b){
    int c = a;
    if(a<b){
	c = b;
    }
    return c;
}

int f(int x){
    return 2*x+max(x+4, 2*x);
}

int main(){
    printf("%d, %d\n", f(3), f(5));

    return 0;
}
