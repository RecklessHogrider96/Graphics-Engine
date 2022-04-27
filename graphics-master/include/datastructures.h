#ifndef DATASTRUCTURES_H

#define DATASTRUCTURES_H

#define MAX 100000

#include "graphics.h"

// Creating a stack
struct stack
{
  Point items[MAX];
  int top;
};
typedef struct stack st;

void createEmptyStack(st *s);

int isfull(st *s);

int isempty(st *s);

void push(st *s, Point newitem);

Point pop(st *s);

void printStack(st *s);

#endif