#include <stdlib.h>
#include <stdio.h>

// pos. 0x200
int b[4] = {0, 0, 0, 0};

void main(int a0, int a1, int a2) {
    b[3] = q2(b[0], b[1], b[2]);
}

int q2(int a0, int a1, int a2){
    if(a0 < 10 || a0 > 18){
        return 0;
    }
    switch(a0 - 10){
        case 0:
            a2 += a1;
            break;
        case 1:
            a2 = 0;
            break;
        case 2:
            a2 = a1 - a2;
            break;
        case 3:
            a2 = 0;
            break;
        case 4:
            if(a1 > a2) {
                a2 = 1;
            } else {
                a2 = 0;
            }
            break;
        case 5:
            a2 = 0;
            break;
        case 6:
            if(a2 > a1){
                a2 = 1;
            } else{
                a2 = 0;
            }
            break;
        case 7:
            a2 = 0;
            break;
        case 8:
            if(a2 == a1){
                a2 = 1;
            } else {
                a2 = 0;
            }
            break;
    }
    
    return a2;
}