#ifndef GRAPHICS_H

#define GRAPHICS_H

#include <image.h>

// Point Data structure
typedef struct
{
  double val[4];
} Point;

// Line Data structure
typedef struct
{
  int zBuffer;
  Point a;
  Point b;
} Line;

// Circle Data structure
typedef struct
{
  double r;
  Point c;
} Circle;

// Ellipse Data structure
typedef struct
{
  double ra;
  double rb;
  Point c;
  double a;
} Ellipse;

// Polyline Data Structure
typedef struct
{
  int zBuffer;
  int numVertex;
  Point *vertex;
} Polyline;

// Point Primitive Functions
void point_set2D(Point *p, double x, double y);
void point_set3D(Point *p, double x, double y, double z);
void point_set(Point *p, double x, double y, double z, double h);
void point_normalize(Point *p);
void point_copy(Point *to, Point *from);
void point_draw(Point *p, Image *src, Color c);
void point_drawf(Point *p, Image *src, FPixel c);
void point_print(Point *p, FILE *fp);

// Line Primitive Functions
void line_set2D(Line *l, double x0, double y0, double x1, double y1);
void line_set(Line *l, Point ta, Point tb);
void line_zBuffer(Line *l, int flag);
void line_normalize(Line *l);
void line_copy(Line *to, Line *from);
void line_draw(Line *l, Image *src, Color c);
void line_draw_dotted(Line *l, Image *src, Color c);

// Circle Primitive Functions
void circle_set(Circle *c, Point tc, double tr);
void circle_draw(Circle *c, Image *src, Color p);
void circle_draw_dotted(Circle *c, Image *src, Color p);
void circle_drawFill(Circle *c, Image *src, Color p);
void circlePlotPoints(Image *src, int xc, int yc, int x, int y, Color p);
void circleFillPlotPoints(Image *src, int xc, int yc, int x, int y, Color p);

// Ellipse Primitive Functions
void ellipse_set(Ellipse *e, Point tc, double ta, double tb);
void ellipse_draw(Ellipse *e, Image *src, Color p);
void ellipse_draw_dotted(Ellipse *e, Image *src, Color p);
void ellipse_drawFill(Ellipse *e, Image *src, Color p);
void ellipsePlotPoints(Image *src, int xc, int yc, int x, int y, Color p);
void ellipseFillPlotPoints(Image *src, int xc, int yc, int x, int y, Color p);

// Polyline Primitive Functions
Polyline *polyline_create(void);
Polyline *polyline_createp(int numV, Point *vlist);
void polyline_free(Polyline *p);

void polyline_init(Polyline *p);
void polyline_set(Polyline *p, int numV, Point *vlist);
void polyline_clear(Polyline *p);

void polyline_zBuffer(Polyline *p, int flag);
void polyline_copy(Polyline *to, Polyline *from);
void polyline_print(Polyline *p, FILE *fp);
void polyline_normalize(Polyline *p);
void polyline_draw(Polyline *p, Image *src, Color c);

// Fill Algorithms

void floodFill(Point center, Image *src, Color c, Color outline);
void floodFillRecursion(Point center, Image *src, Color c, Color outline);

#endif