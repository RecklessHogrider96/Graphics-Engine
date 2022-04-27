/*
  CS5310 - Roshan Bellary
  10-17-2021 (Fall 2021)
  View 2D Grapihcs Primitives
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graphics.h"
#include "matrix.h"
#include "vectors.h"
#include "view2D.h"

// Sets vtm to be the view transformation defined by the 2DView structure.
void matrix_setView2D(Matrix *vtm, View2D *view)
{
	// screen rows and cols
	matrix_identity(vtm);

	int c = view->screenx;
	int r = view->screeny;

	// center of view
	double vx = view->vrp.val[0];
	double vy = view->vrp.val[1];

	// orient vector
	Vector x = view->x;
	vector_normalize(&x);
	double nx = x.val[0];
	double ny = x.val[1];

	double du = view->du;
	double dv = (du * r) / c;

	// translate to origin
	matrix_translate2D(vtm, -vx, -vy);

	// rotate
	matrix_rotateZ(vtm, nx, -ny);

	// scale
	matrix_scale2D(vtm, c / du, -r / dv);

	// translate to image cords
	matrix_translate2D(vtm, c / 2.0, r / 2.0);
}

// Sets the values to the structure.
void view2D_set(View2D *view, Point *vrp, double d, Vector *orientation, double x, double y)
{
	view->du = d;
	view->vrp = *vrp;
	view->x = *orientation;
	view->screenx = x;
	view->screeny = y;
}
