#include <stdio.h>

int main( ){
    printf("the size of int: %lu\n", sizeof(int));
    printf("the size of float: %lu\n", sizeof(float));
    printf("the size of double: %lu\n", sizeof(double));
    printf("the size of void: %lu\n", sizeof(void));
    printf("the size of int*: %lu\n", sizeof(int*));

    printf("the size of short int: %lu\n", sizeof(short int));
    printf("the size of int: %lu\n", sizeof(int));
    printf("the size long int: %lu\n", sizeof(long int));

    char a = 255;
    if(a < 0){
        printf("this is signed");
    }
    else {
        printf("this is unsigned");
    }
}
