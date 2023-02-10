/*#include "stack.h"

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

void pop(int* item){
    *item = -1;
    struct node *ptr;
    if (head == NULL)
        printf("Underflow State: can't remove any item");
    else{
        *item = head->val;
        ptr = head;
        head = head->next;
        free(ptr);
    }
}

int isEmpty(){
    if(head == NULL)
        return 1;
    return 0;
}*/

/*void isEmpty(int* emp){
    if(head == NULL){
        printf("OOOO\n");
        *emp = 1;
        return;
    }
    *emp = 0;
}*/

#include "stack.h"

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

int isEmpty(){
    if(head == NULL)
        return 1;
    return 0;
}
