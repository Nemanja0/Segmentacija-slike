#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void push (int po){
    int val;
    struct node *ptr = (struct node*)heap_malloc(1, sizeof(struct node));

    if(head == NULL){
        ptr->val = po;
        ptr->next = NULL;
        head = ptr;
    }
    else{
        ptr->val = po;
        ptr->next = head;
        head=ptr;
    }
}

void pop(int* item){
    *item = -1;
    struct node *ptr;
    if (head == NULL)
        printf("Underflow State: can't remove any item");
    else{
        *item = head->val;
        ptr = head;
        head = head->next;
        heap_free(1, ptr);
    }
}


/*int isEmpty(){
    if(head == NULL)
        return 1;
    return 0;
}*/

void isEmpty(int* emp){
    if(head == NULL){
        *emp = 1;
        return;
    }
    *emp = 0;
}