#ifndef POLYGON_H

#define POLYGON_H

#include "graphics.h"
#include "vectors.h"
#include "drawstate.h"

// Polygon Data Structure
typedef struct
{
  int onesided;
  int nVertex;
  Point *vertex;
  Color *color;
  Vector *normal;
  int zBuffer;
} Polygon;

// Polygon Primitive Funtions

Polygon *polygon_create(void);
Polygon *polygon_createp(int numV, Point *vlist);
void polygon_free(Polygon *p);
void polygon_init(Polygon *p);
void polygon_set(Polygon *p, int numV, Point *vlist);
void polygon_clear(Polygon *p);
void polygon_setSided(Polygon *p, int oneSided);
void polygon_copy(Polygon *to, Polygon *from);
void polygon_print(Polygon *p, FILE *fp);
void polygon_normalize(Polygon *p);
void polygon_draw(Polygon *p, Image *src, Color c);
void polygon_drawFill(Polygon *p, Image *src, DrawState *ds);
void polygon_drawFillB(Polygon *p, Image *src, Color c);
double barycentric_points(double x0, double y0, double x1, double y1, double x, double y);
void polygon_drawFrame(Polygon *p, Image *src, Color c);
float Aux_implicit_line(Point p1, Point p2, float x, float y);

#endif