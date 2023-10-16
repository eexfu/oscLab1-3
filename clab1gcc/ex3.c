#include <stdio.h>
#include <string.h>

int main(){
    int max=20;
    char first[max];
    char second[max];
    char name[2 * max];
    char str[max];

    printf("please enter your first name:\n");
    scanf("%19s", first);

    printf("please enter your second name:\n");
    scanf("%19s", second);

    int counter = 0;

    for(int i=0; second[i] != '\0' && i < max; i++){
        if(second[i] >= 'A' && second[i] <= 'Z'){ //upper case
            str[i] = second[i];
        }
        else if (second[i] >='a' && second[i] <= 'z')  {
            str[i] = second[i] - 32;
        }
        else {
            counter++;
            printf("you might enter %d invalid char(s) in your second name\n", counter);
        }
    }

    printf("Your second name in upper case:\n");
    printf("%s\n",str);

    if(strcmp(second, str) == 0){
        printf("second and str are the same\n");
    }

    strncat(name, first, 20);
    strncat(name, second, 20);
    printf("%s\n", name);

    printf("please enter your year\n");
    char year[5];
    scanf("%4s",year);
    snprintf(name, sizeof(name), "%s %s %s", first, second, year);
    printf("%s",name);

    sscanf(name, "%s %s %s", first, second, year);
    printf("\nthis is your first name: %s\n", first);
    printf("this is your second name: %s\n", second);
    printf("this is your year: %s\n", year);
}
