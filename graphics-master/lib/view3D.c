/*
  CS5310 - Roshan Bellary
  10-17-2021 (Fall 2021)
  View 3D graphic Primitives
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graphics.h"
#include "matrix.h"
#include "vectors.h"
#include "view3D.h"

// When the function returns, the vtm should contain the complete view matrix. Inside the function, begin by
// initializing VTM to the identity. Do not modify any of the values in the PerspectiveView structure inside the
// function (no side-effects).
void matrix_setView3D(Matrix *vtm, View3D *view)
{
	// 	Initialize variables
	Vector u;
	Point vrp;
	Vector vpn;
	Vector vup;
	double d = view->d;
	double du = view->du;
	double dv = view->dv;
	double b = view->b;
	int screenx = view->screenx;
	int screeny = view->screeny;

	// 	Make copy of point and vectors
	point_copy(&vrp, &view->vrp);
	vector_copy(&vpn, &view->vpn);
	vector_copy(&vup, &view->vup);

	// 	Setup matrix and viewing vectors
	matrix_identity(vtm);
	vector_cross(&vup, &vpn, &u);
	vector_cross(&vpn, &u, &vup);

	// 	Translate VRP to origin
	matrix_translate(vtm, -vrp.val[0], -vrp.val[1], -vrp.val[2]);

	// 	Normalize vectors
	vector_normalize(&u);
	vector_normalize(&vup);
	vector_normalize(&vpn);

	// 	Align the axes
	matrix_rotateXYZ(vtm, &u, &vup, &vpn);

	// 	Translate COP to the origin
	matrix_translate(vtm, 0, 0, d);

	// 	Scale to the canonical view volume
	double Bp = d + b;
	double sx = (2 * d) / (Bp * du);
	double sy = (2 * d) / (Bp * dv);
	double sz = 1 / Bp;

	matrix_scale(vtm, sx, sy, sz);

	//  Apply perspective
	double dp = d / Bp;
	matrix_perspective(vtm, dp);

	// 	Scale to image coordinates
	double sx2 = -screenx / (2 * dp);
	double sy2 = -screeny / (2 * dp);
	matrix_scale2D(vtm, sx2, sy2);

	// 	Translate to image coordinates
	matrix_translate2D(vtm, screenx / 2, screeny / 2);
}
