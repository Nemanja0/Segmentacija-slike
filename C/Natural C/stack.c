#include "stack.h"

/// @brief push
/// @param po - Promjenjiva koja se postavlja na stek
void push (int po){
    int val;
    struct node *ptr = (struct node*)malloc(sizeof(struct node));

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

/// @brief pop
/// @return Vraca se promjenjiva koja se nalazi na vrhu stek-a ili ispisuje gresku ukoliko je stek prazan
int pop(){
    int item = -1;
    struct node *ptr;
    if (head == NULL)
        printf("Underflow State: can't remove any item");
    else{
        item = head->val;
        ptr = head;
        head = head->next;
        free(ptr);
    }
    return item;
}

/// @brief isEmpty
/// @return Vraca vrijednost 0 ukoliko stek nije prazan ili 1 ukoliko je stek prazan
int isEmpty(){
    if(head == NULL)
        return 1;
    return 0;
}
