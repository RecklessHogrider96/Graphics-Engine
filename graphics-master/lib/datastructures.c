/* 
  CS5310 - Roshan Bellary
  09-23-2021 (Fall 2021)

  Data Structures in C:
  * Stack implementation in C
*/

#include <stdio.h>
#include <stdlib.h>

#include "datastructures.h"

int count = 0;

void createEmptyStack(st *s)
{
  s->top = -1;
}

// Check if the stack is full
int isfull(st *s)
{
  if (s->top == MAX - 1)
    return 1;
  else
    return 0;
}

// Check if the stack is empty
int isempty(st *s)
{
  if (s->top == -1)
    return 1;
  else
    return 0;
}

// Add elements into stack
void push(st *s, Point newitem)
{
  if (!isfull(s))
  {
    s->top++;
    s->items[s->top] = newitem;
    //printf("Item pushed= (%f,%f) \n", newitem.val[0], newitem.val[1]);
  }
  count++;
}

// Remove element from stack
Point pop(st *s)
{
  Point popped;
  point_set2D(&popped, 0, 0);

  if (isempty(s))
  {
    printf("\n STACK EMPTY \n");
  }
  else
  {
    //printf("Item popped= (%f,%f) \n", s->items[s->top].val[0], s->items[s->top].val[1]);
    point_copy(&popped, &s->items[s->top]);
    s->top--;
  }
  count--;

  return popped;
  //printf("\n");
}

// Print elements of stack
void printStack(st *s)
{
  printf("Stack: ");
  for (int i = 0; i < count; i++)
  {
    printf("(%f,%f) \n", s->items[i].val[0], s->items[i].val[1]);
  }
  printf("\n");
}