#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
/*typedef struct{
    int x, y;
} Points;*/

struct node{
    int val;
    struct node *next;
};

struct node *head;

void push(int);
void pop(int*);
void isEmpty(int*);

#endif
