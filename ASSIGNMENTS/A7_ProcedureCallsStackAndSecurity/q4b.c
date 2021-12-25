#include <stdio.h>

static long x[8] = {1, 2, 3, -1, -2, 0, 184, 340057058};
static int y[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int f(long a) {
    int c = 0;

    while (a != 0)
    {
        if ((a & 0x80000000) != 0)
        {
            c++;
        }          
        a = (a << 1); 
    }
    return c;
}

int main() {

    int i = 8;
    while (i != 0) {
        i--;
        y[i] = f(x[i]);
    }
    for (int b = 0; b < 8; b++)
    {
        printf("%ld\n", x[b]);
    }

    for (int b = 0; b < 8; b++)
    {
        printf("%d\n", y[b]);
    }
    
}