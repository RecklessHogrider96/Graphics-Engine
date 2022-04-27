#ifndef VIEW2D_H

#define VIEW2D_H

#include "graphics.h"
#include "vectors.h"

// View 2D structure
typedef struct
{
	// 	center of view
	Point vrp;
	// 	width of view
	double du;
	// 	normalized vector
	Vector x;
	// 	number of cols
	int screenx;
	//  number of rows
	int screeny;
} View2D;

// Sets vtm to be the view transformation defined by the 2DView structure.
void matrix_setView2D(Matrix *vtm, View2D *view);

// Sets the values to the structure.
void view2D_set(View2D *view, Point *vrp, double d, Vector *orientation, double x, double y);

#endif