/*
  CS5310 - Roshan Bellary
  10-17-2021 (Fall 2021)
  Contains the functions for a matrix
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graphics.h"
#include "matrix.h"
#include "vectors.h"

// Print out the matrix in a nice 4x4 arrangement with a blank line below.
void matrix_print(Matrix *m, FILE *fp)
{
	int row, cols;
	printf("\n----------------------------------------------------------------\n");
	for (row = 0; row < 4; row++)
	{
		for (cols = 0; cols < 4; cols++)
		{
			printf("| %f     ", m->mat[row][cols]);
		}
		printf("|\n----------------------------------------------------------------\n");
	}
}

// Set the matrix to all zeros.
void matrix_clear(Matrix *m)
{
	int row, col;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			m->mat[row][col] = 0.0;
		}
	}
}

// Set the matrix to the identity matrix.
void matrix_identity(Matrix *m)
{
	matrix_clear(m);

	m->mat[0][0] = 1;
	m->mat[1][1] = 1;
	m->mat[2][2] = 1;
	m->mat[3][3] = 1;
}

// Return the element of the matrix at row r, column c.
double matrix_get(Matrix *m, int r, int c)
{
	return m->mat[r][c];
}

// Set the element of the matrix at row r, column c to v.
void matrix_set(Matrix *m, int r, int c, double v)
{
	m->mat[r][c] = v;
}

// Copy the src matrix into the dest matrix.
void matrix_copy(Matrix *dest, Matrix *src)
{
	int row, col;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			dest->mat[row][col] = src->mat[row][col];
		}
	}
}

// Transpose the matrix m in place.
void matrix_transpose(Matrix *m)
{
	int n, n1;
	double tmp;

	for (n = 0; n < 3; n++)
	{
		for (n1 = n + 1; n1 < 4; n1++)
		{
			tmp = m->mat[n1][n];
			m->mat[n1][n] = m->mat[n][n1];
			m->mat[n][n1] = tmp;
		}
	}
}

// Multiply left and right and put the result in m.
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m)
{
	int i, j, k;
	Matrix temp;
	matrix_clear(&temp);
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			for (k = 0; k < 4; k++)
			{
				temp.mat[i][j] += left->mat[i][k] * right->mat[k][j];
			}
		}
	}

	// Assign temp values to matrix m
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			m->mat[i][j] = temp.mat[i][j];
		}
	}
}

// Transform the point p by the matrix m and put the result in q. For this function, p and q need to be
// different variables.
void matrix_xformPoint(Matrix *m, Point *p, Point *q)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		q->val[i] = p->val[0] * m->mat[i][0] + p->val[1] * m->mat[i][1] +
					p->val[2] * m->mat[i][2] + p->val[3] * m->mat[i][3];
	}
}

// Transform the vector p by the matrix m and put the result in q. For this function, p and q need to be
// different variables.
void matrix_xformVector(Matrix *m, Vector *p, Vector *q)
{
	Vector temp;
	int i;
	for (i = 0; i < 4; i++)
	{
		temp.val[i] = m->mat[i][0] * p->val[0] + m->mat[i][1] * p->val[1] + m->mat[i][2] * p->val[2] + m->mat[i][3] * p->val[3];
	}
	*q = temp;
}

// Transform the points and surface normals (if they exist) in the Polygon p by the matrix m.
void matrix_xformPolygon(Matrix *m, Polygon *p)
{
	double temp1, temp2, temp3, temp4;
	int i, k;
	for (k = 0; k < p->nVertex; k++)
	{
		temp1 = p->vertex[k].val[0];
		temp2 = p->vertex[k].val[1];
		temp3 = p->vertex[k].val[2];
		temp4 = p->vertex[k].val[3];
		for (i = 0; i < 4; i++)
		{
			p->vertex[k].val[i] = m->mat[i][0] * temp1 + m->mat[i][1] * temp2 + m->mat[i][2] * temp3 + m->mat[i][3] * temp4;
		}
	}
}

// Transform the points in the Polyline p by the matrix m.
void matrix_xformPolyline(Matrix *m, Polyline *p)
{
	Point temp;
	for (int x = 0; x < p->numVertex; x++)
	{
		matrix_xformPoint(m, &p->vertex[x], &temp);

		// TODO: replace with point_copy
		p->vertex[x].val[0] = temp.val[0];
		p->vertex[x].val[1] = temp.val[1];
		p->vertex[x].val[2] = temp.val[2];
		p->vertex[x].val[3] = temp.val[3];
	}
}

// Transform the points in line by the matrix m.
void matrix_xformLine(Matrix *m, Line *line)
{
	Point tempa;
	Point tempb;
	matrix_xformPoint(m, &line->a, &tempa);

	// TODO: replace with point_copy
	line->a.val[0] = tempa.val[0];
	line->a.val[1] = tempa.val[1];
	line->a.val[2] = tempa.val[2];
	line->a.val[3] = tempa.val[3];
	matrix_xformPoint(m, &line->b, &tempb);
	line->b.val[0] = tempb.val[0];
	line->b.val[1] = tempb.val[1];
	line->b.val[2] = tempb.val[2];
	line->b.val[3] = tempb.val[3];
}

// Premultiply the matrix by a scale matrix parameterized by sx and sy.
void matrix_scale2D(Matrix *m, double sx, double sy)
{
	Matrix scale2D;
	matrix_identity(&scale2D);
	scale2D.mat[0][0] = sx;
	scale2D.mat[1][1] = sy;
	matrix_multiply(&scale2D, m, m);
}

// Premultiply the matrix by a Z-axis rotation matrix parameterized by cos(✓) and sin(✓), where ✓ is
// the angle of rotation about the Z-axis. ASK
void matrix_rotateZ(Matrix *m, double cth, double sth)
{
	Matrix rotateZ;
	matrix_identity(&rotateZ);
	rotateZ.mat[0][0] = cth;
	rotateZ.mat[0][1] = -sth;
	rotateZ.mat[1][0] = sth;
	rotateZ.mat[1][1] = cth;
	matrix_multiply(&rotateZ, m, m);
}

// Premultiply the matrix by a 2D translation matrix parameterized by tx and ty
void matrix_translate2D(Matrix *m, double tx, double ty)
{
	Matrix translate2D;
	matrix_identity(&translate2D);
	translate2D.mat[0][3] = tx;
	translate2D.mat[1][3] = ty;
	matrix_multiply(&translate2D, m, m);
}

// Premultiply the matrix by a 2D shear matrix parameterized by shx and shy
void matrix_shear2D(Matrix *m, double shx, double shy)
{
	Matrix shear2D;
	matrix_identity(&shear2D);
	shear2D.mat[0][1] = shx;
	shear2D.mat[1][0] = shy;
	matrix_multiply(&shear2D, m, m);
}

// Premultiply the matrix by a translation matrix parameterized by tx, ty, and tz.
void matrix_translate(Matrix *m, double tx, double ty, double tz)
{
	Matrix translate;
	matrix_identity(&translate);
	translate.mat[0][0] = 1;
	translate.mat[0][3] = tx;
	translate.mat[1][1] = 1;
	translate.mat[1][3] = ty;
	translate.mat[2][2] = 1;
	translate.mat[2][3] = tz;
	translate.mat[3][3] = 1;
	matrix_multiply(&translate, m, m);
}

// Premultiply the matrix by a scale matrix parameterized by sx, sy, sz.
void matrix_scale(Matrix *m, double sx, double sy, double sz)
{
	Matrix scale;
	matrix_identity(&scale);
	scale.mat[0][0] = sx;
	scale.mat[1][1] = sy;
	scale.mat[2][2] = sz;
	scale.mat[3][3] = 1;
	matrix_multiply(&scale, m, m);
}

/* Premultiply the matrix by a X-axis rotation matrix parameterized by cos(✓) and sin(✓), where ✓ is
the angle of rotation about the X-axis.*/
void matrix_rotateX(Matrix *m, double cth, double sth)
{
	Matrix rotateX;
	matrix_identity(&rotateX);
	rotateX.mat[0][0] = 1;
	rotateX.mat[1][1] = cth;
	rotateX.mat[1][2] = -sth;
	rotateX.mat[2][1] = sth;
	rotateX.mat[2][2] = cth;
	rotateX.mat[3][3] = 1;
	matrix_multiply(&rotateX, m, m);
}

/*Premultiply the matrix by a Y-axis rotation matrix parameterized by cos(✓) and sin(✓), where ✓ is
the angle of rotation about the Y-axis*/
void matrix_rotateY(Matrix *m, double cth, double sth)
{
	Matrix rotateY;
	matrix_identity(&rotateY);
	rotateY.mat[0][0] = cth;
	rotateY.mat[0][2] = sth;
	rotateY.mat[1][1] = 1;
	rotateY.mat[2][0] = -sth;
	rotateY.mat[2][2] = cth;
	rotateY.mat[3][3] = 1;
	matrix_multiply(&rotateY, m, m);
}

/*Premultiply the matrix by an XYZ-axis rotation matrix parameterized by the vectors ~u, ~v, and w~,
where the three vectors represent an orthonormal 3D basis*/
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w)
{
	Matrix rotateXYZ;
	matrix_identity(&rotateXYZ);
	rotateXYZ.mat[0][0] = u->val[0];
	rotateXYZ.mat[0][1] = u->val[1];
	rotateXYZ.mat[0][2] = u->val[2];
	rotateXYZ.mat[1][0] = v->val[0];
	rotateXYZ.mat[1][1] = v->val[1];
	rotateXYZ.mat[1][2] = v->val[2];
	rotateXYZ.mat[2][0] = w->val[0];
	rotateXYZ.mat[2][1] = w->val[1];
	rotateXYZ.mat[2][2] = w->val[2];
	rotateXYZ.mat[3][3] = 1;
	matrix_multiply(&rotateXYZ, m, m);
}

// Premultiply the matrix by a shear Z matrix parameterized by shx and shy.
void matrix_shearZ(Matrix *m, double shx, double shy)
{
	Matrix shearZ;
	matrix_identity(&shearZ);
	shearZ.mat[0][0] = 1;
	shearZ.mat[0][2] = shx;
	shearZ.mat[1][1] = 1;
	shearZ.mat[1][2] = shy;
	shearZ.mat[2][2] = 1;
	shearZ.mat[3][3] = 1;
	matrix_multiply(&shearZ, m, m);
}

// Premultiply the matrix by a perspective matrix parameterized by d.
void matrix_perspective(Matrix *m, double d)
{
	Matrix persp;
	matrix_identity(&persp);
	persp.mat[3][2] = 1 / d;
	persp.mat[3][3] = 0;
	matrix_multiply(&persp, m, m);
}
