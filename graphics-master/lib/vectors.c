#include <math.h>
#include "vectors.h"
#include "polygon.h"

// Set the Vector to (x, y, z, 0.0).
void vector_set(Vector *v, double x, double y, double z)
{
	v->val[0] = x;
	v->val[1] = y;
	v->val[2] = z;
	v->val[3] = 0.0;
}

//  Print out the Vector to stream fp in a pretty form.
void vector_print(Vector *v, FILE *fp)
{
	if (NULL != v && NULL != fp)
	{
		fprintf(fp, " ( %.3f %.3f %.3f %.3f )\n", v->val[0], v->val[1], v->val[2], v->val[3]);
	}
}

// Copy the src Vector into the dest Vector.
void vector_copy(Vector *dest, Vector *src)
{
	dest->val[0] = src->val[0];
	dest->val[1] = src->val[1];
	dest->val[2] = src->val[2];
	dest->val[3] = src->val[3];
}

// Returns the Euclidean length of the vector, assuming the homogeneous coordinate is 1.0.
double vector_length(Vector *v)
{
	double vx = v->val[0];
	double vy = v->val[1];
	double vz = v->val[2];

	return (sqrt((vx * vx) + (vy * vy) + (vz * vz)));
}

// Normalize the Vector to unit length. Do not modify the homogeneous coordinate.
void vector_normalize(Vector *v)
{
	double L = vector_length(v);
	v->val[0] = v->val[0] / L;
	v->val[1] = v->val[1] / L;
	v->val[2] = v->val[2] / L;
}

// Returns the scalar product of ~a and ~b.
double vector_dot(Vector *a, Vector *b)
{
	double ax = a->val[0];
	double bx = b->val[0];
	double ay = a->val[1];
	double by = b->val[1];
	double az = a->val[2];
	double bz = b->val[2];

	return ((ax * bx) + (ay * by) + (az * bz));
}

// Calculates the the cross product (vector product) of ~a and ~b and puts the result in ~c.
void vector_cross(Vector *a, Vector *b, Vector *c)
{
	double ax = a->val[0];
	double bx = b->val[0];
	double ay = a->val[1];
	double by = b->val[1];
	double az = a->val[2];
	double bz = b->val[2];

	c->val[0] = ay * bz - az * by;
	c->val[1] = az * bx - ax * bz;
	c->val[2] = ax * by - ay * bx;
}