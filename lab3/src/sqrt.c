#include <stdio.h>
#include <stdlib.h>

// Non optimized
int custom_sqrt(int number){
    int i = 0;
    for(; i < number/2; ++i){
        if(i * i == number){
            break;
        }
    }

    if(i == number/2){
        i = -1;
    }

    return i;
}

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Enter a number please\n");
        return 1;
    }

    int nbr = atoi(argv[1]);
    printf("sqrt(%d) = %d\n", nbr, custom_sqrt(nbr));
    return 0;
}
