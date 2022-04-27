#ifndef VECTORS_H

#define VECTORS_H

#include "graphics.h"

// Vector Data structure
typedef Point Vector;

// Vector Functions

void vector_set(Vector *v, double x, double y, double z);
void vector_print(Vector *v, FILE *fp);
void vector_copy(Vector *dest, Vector *src);
double vector_length(Vector *v);
void vector_normalize(Vector *v);
double vector_dot(Vector *a, Vector *b);
void vector_cross(Vector *a, Vector *b, Vector *c);

#endif