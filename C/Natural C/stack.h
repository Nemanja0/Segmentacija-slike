#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>


struct node{
    int val;
    struct node *next;
};

struct node *head;

void push(int);
int pop();
int isEmpty();

#endif
