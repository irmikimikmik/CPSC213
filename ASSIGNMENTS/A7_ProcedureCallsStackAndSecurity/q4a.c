#include <stdio.h>

int *a;

void foo(int a0, int a1) {
    a[a1] = a0 + a[a1];
    return;
}


void main() {

    a = malloc(sizeof(int)*10);

    int l0 = 1;
    int l1 = 2;
    foo(3, 4);
    foo(l0, l1);
    
    for (int i = 0; i < 10; i++){
        printf("%d\n", a[i]);
    }
    
    return;
}