/**
 * \author Jeroen Van Aken, Bert Lagaisse, Ludo Bruynseels
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "dplist.h"



/*
 * The real definition of struct list / struct node
 */
struct dplist_node {
    dplist_node_t *prev, *next;
    element_t element;
};

struct dplist {
    dplist_node_t *head;
    // more fields will be added later
};

dplist_t *dpl_create() {
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    list->head = NULL;
    return list;
}

void dpl_free(dplist_t **list) {

    //TODO: add your code here
    //Do extensive testing with valgrind.

    int size = dpl_size(*list);
    dplist_node_t *node_pointer = dpl_get_reference_at_index(*list, size);
    for(int i=0;i<size;i++){
        if(node_pointer->prev!=NULL){
            node_pointer = node_pointer->prev;
            free(node_pointer->next);
        }
        else{
            free((*list)->head);
        }
    }

    free(*list);
}

/* Important note: to implement any list manipulation operator (insert, append, delete, sort, ...), always be aware of the following cases:
 * 1. empty list ==> avoid errors
 * 2. do operation at the start of the list ==> typically requires some special pointer manipulation
 * 3. do operation at the end of the list ==> typically requires some special pointer manipulation
 * 4. do operation in the middle of the list ==> default case with default pointer manipulation
 * ALWAYS check that you implementation works correctly in all these cases (check this on paper with list representation drawings!)
 **/


dplist_t *dpl_insert_at_index(dplist_t *list, element_t element, int index) {
    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL) return NULL;

    list_node = malloc(sizeof(dplist_node_t));

    list_node->element = element;
    // pointer drawing breakpoint
    if (list->head == NULL) { // covers case 1
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
        // pointer drawing breakpoint
    } else if (index <= 0) { // covers case 2
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
        // pointer drawing breakpoint
    } else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        // pointer drawing breakpoint
        if (index < dpl_size(list)) { // covers case 4
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
            // pointer drawing breakpoint
        } else { // covers case 3
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
            // pointer drawing breakpoint
        }
    }

    return list;
}

dplist_t *dpl_remove_at_index(dplist_t *list, int index) {

    //TODO: add your code here
    dplist_node_t *node_pointer = dpl_get_reference_at_index(list, index);
    if(node_pointer == NULL){
        list = NULL;
    }
    else if(index<=0){
        list->head = node_pointer->next;
        node_pointer->next->prev = NULL;
        free(node_pointer);
    }
    else if(index>dpl_size(list)){
        node_pointer->prev->next = NULL;
        free(node_pointer);
    }
    else{
        node_pointer->next->prev = node_pointer->prev;
        node_pointer->prev->next = node_pointer->next;
        free(node_pointer);
    }
    return list;
}

int dpl_size(dplist_t *list) {

    //TODO: add your code here
    dplist_node_t * node_pointer = list->head;
    if(list == NULL){
        return -1;
    }
    else{
        if(node_pointer == NULL){
            return 0;
        }
        else{
            int size = 0;
            while(node_pointer != NULL){
                size++;
                node_pointer = node_pointer->next;
            }
            printf("\n");
            return size;
        }
    }
}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index) {
    //int count = 0 ;
    dplist_node_t *dummy = NULL;

    //TODO: add your code here
    int size = dpl_size(list);
    if(list == NULL){

    }
    else if(list->head == NULL){

    }
    else if(index <= 0){
        dummy = list->head;
    }
    else if(index >= size){
        dummy = list->head;
        for(int i=1;i<size;i++){
            dummy = dummy->next;
        }
    }
    else{
        dummy = list->head;
        for(int i=0;i<index;i++){
            dummy = dummy->next;
        }
    }

    return dummy;
}

element_t dpl_get_element_at_index(dplist_t *list, int index) {

    //TODO: add your code here

    element_t element = ' ';
    dplist_node_t *node_pointer = dpl_get_reference_at_index(list, index);
    if(node_pointer==NULL){
        element = 0;
    }
    else{
        element = node_pointer->element;
    }

    return element;
}

int dpl_get_index_of_element(dplist_t *list, element_t element) {

    //TODO: add your code here

    int index = -1;
    dplist_node_t *node_pointer = list->head;
    if(list == NULL){

    }
    else if(list->head == NULL){

    }
    else{
        for(int i=0;i<dpl_size(list);i++){
            if(node_pointer->element == element){
                index = i;
            }
            node_pointer = node_pointer->next;
        }
    }

    return index;
}



