#define _GNU_SOURCE
/**
 * \author Bert Lagaisse
 *
 * main method that executes some test functions (without check framework)
 */

#include <stdio.h>
#include <stdbool.h>
#include "dplist.h"

void ck_assert_msg(bool result, char * msg){
    if(!result) printf("%s\n", msg);
}

int main(void)
{
    element_t a,b,c = NULL;
    a = "hello ";
    b = "world!";
    c = "123\e";

    dplist_t *numbers = NULL;
    numbers = dpl_create();

    ck_assert_msg(numbers != NULL, "numbers = NULL, List not created");
    ck_assert_msg(dpl_size(numbers) == 0, "Numbers may not contain elements.");

    dpl_insert_at_index(numbers, a, 0);
    ck_assert_msg(dpl_size(numbers) == 1, "Numbers must contain 1 element.");

    dpl_insert_at_index(numbers, b, -1);
    ck_assert_msg(dpl_size(numbers) == 2, "Numbers must contain 2 elements.");

    dpl_insert_at_index(numbers, c, 100);
    ck_assert_msg(dpl_size(numbers) == 3, "Numbers must contain 3 elements.");

    element_t element = dpl_get_element_at_index(numbers, 1);
    printf("%s",element);
    element = dpl_get_element_at_index(numbers, 2);
    printf("%s",element);
    element = dpl_get_element_at_index(numbers, 0);
    printf("%s",element);
    ck_assert_msg(element == b, "This is the wrong element");

    int index = dpl_get_index_of_element(numbers,c);
    printf("%d",index);
    index = dpl_get_index_of_element(numbers,a);
    printf("%d",index);
    index = dpl_get_index_of_element(numbers,b);
    printf("%d",index);
    ck_assert_msg(index == 0, "This the wrong index");

    dpl_remove_at_index(numbers,4);
    ck_assert_msg(dpl_size(numbers) == 2, "The element wasn't removed");

    element = dpl_get_element_at_index(numbers, 0);
    printf("%s",element);
    element = dpl_get_element_at_index(numbers, 1);
    printf("%s",element);

    dpl_free(&numbers);

    return 0;
}