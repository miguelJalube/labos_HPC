#include <stdio.h>
#include <stdlib.h>

// Func from bit hacks
unsigned int log2_cust(unsigned int v){
    const unsigned int b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    const unsigned int S[] = {1, 2, 4, 8, 16};
    int i;

    register unsigned int r = 0; // result of log2(v) will go here
    for (i = 4; i >= 0; i--) // unroll for speed...
    {
        if (v & b[i])
        {
            v >>= S[i];
            r |= S[i];
        } 
    }

    return r;
}

// optimized
unsigned int custom_sqrt_opt(unsigned int number){
    unsigned i = 0;
    while(i * i < number){
        ++i;
    }

    if(i * i < number){
        i = -1;
    }

    return i;
}

// non optimized
unsigned custom_sqrt(unsigned number){
    unsigned i = 0;
    for(; i <= number/2; ++i){
        if(i * i == number){
            break;
        }
    }

    if(i > number/2){
        i = -1;
    }

    return i;
}

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Enter a number please\n");
        return 1;
    }

    unsigned nbr = atoi(argv[1]);
    printf("non opt :    sqrt(%d) = %d\n", nbr, custom_sqrt(nbr));
    printf("opt :        sqrt(%d) = %d\n", nbr, custom_sqrt_opt(nbr));
    return 0;
}
