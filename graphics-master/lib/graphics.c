/*
  CS5310 - Roshan Bellary
  09-28-2021 (Fall 2021)

  Graphics Base / Primitives.
*/

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graphics.h"
#include "datastructures.h"

#define USECPP 0
#define ROUND(a) ((int)(a + 0.5))

// Point Primitive Functions

// Set the first two values of the vector to x and y. Set the third value to 0.0 and the fourth value to 1.0.
void point_set2D(Point *p, double x, double y)
{
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = 0.0;
  p->val[3] = 1.0;
}

// Set the point’s values to x and y and z. Set the homogeneous coordinate to 1.0.
void point_set3D(Point *p, double x, double y, double z)
{
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = z;
  p->val[3] = 1.0;
}

// Set the four values of the vector to x, y, z, and h, respectively.
void point_set(Point *p, double x, double y, double z, double h)
{
  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = z;
  p->val[3] = h;
}

// Normalize the x and y values of a point by its homogeneous coordinate: x = x/h, y = y/h.
void point_normalize(Point *p)
{
  p->val[0] = p->val[0] / p->val[3];
  p->val[1] = p->val[1] / p->val[3];
}

// Copy the point data structure.
void point_copy(Point *to, Point *from)
{
  to->val[0] = from->val[0];
  to->val[1] = from->val[1];
  to->val[2] = from->val[2];
  to->val[3] = from->val[3];
}

// Draw the point into src using Color c.
void point_draw(Point *p, Image *src, Color c)
{
  int x = (int)p->val[1];
  int y = (int)p->val[0];

  image_setc(src, x, y, 0, c.c[0]);
  image_setc(src, x, y, 1, c.c[1]);
  image_setc(src, x, y, 2, c.c[2]);
  image_setz(src, x, y, p->val[2]);
}

// Draw the point into src using FPixel c.
void point_drawf(Point *p, Image *src, FPixel c)
{
  double x = p->val[1];
  double y = p->val[0];

  image_setf(src, x, y, c);
  image_setz(src, x, y, p->val[2]);
}

// Use fprintf to print the contents of the Point to the stream fp. The FILE pointer fp can be an actual
// file opened using fopen or a standard output stream like stdout or stderr, both defined in stdio.h. The
// point print function doesn’t care which it is, it just uses fprintf, which takes a FILE pointer as the first
// argument.
void point_print(Point *p, FILE *fp)
{
  printf("(%f, %f, %f, %f)\n", p->val[0], p->val[1], p->val[2], p->val[3]);
}

// Line Primitive Functions

// Initialize a 2D line.
void line_set2D(Line *l, double x0, double y0, double x1, double y1)
{
  point_set2D(&(l->a), x0, y0);
  point_set2D(&(l->b), x1, y1);
  line_zBuffer(l, 1);
}

// Initialize a line to ta and tb.
void line_set(Line *l, Point ta, Point tb)
{
  point_copy(&(l->a), &ta);
  point_copy(&(l->b), &tb);
  line_zBuffer(l, 1);
}

// Set the z-buffer flag to the given value.
void line_zBuffer(Line *l, int flag)
{
  l->zBuffer = flag;
}

// Normalize the x and y values of the endpoints by their homogeneous coordinate.
void line_normalize(Line *l)
{
  point_normalize(&(l->a));
  point_normalize(&(l->b));
}

// Copy the line data structure.
void line_copy(Line *to, Line *from)
{
  point_copy(&(to->a), &(from->a));
  point_copy(&(to->b), &(from->b));
  to->zBuffer = from->zBuffer;
}

static double sign(double x)
{
  if (x > 0)
  {
    return 1;
  }
  else if (x < 0)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

// Draw the line into src using color c and the z-buffer, if appropriate.
void line_draw(Line *l, Image *src, Color c)
{
  int x, y, dy, swap, temp, s1, s2, e;
  double dz, dx, zInv, ztemp;
  float zBuf;

  // 	get values from Line l
  double x0 = l->a.val[0];
  double y0 = l->a.val[1];
  double x1 = l->b.val[0];
  double y1 = l->b.val[1];
  double z0 = l->a.val[2];
  double z1 = l->b.val[2];

  zInv = 1 / z0;
  // 	printf("%f\n", z0);

  x = x0;
  y = y0;

  // 	get absolute value of dx and dx and check the signs
  dx = fabs(x1 - x0);
  dy = fabs(y1 - y0);
  s1 = sign(x1 - x0);
  s2 = sign(y1 - y0);

  // 	if the dy value is greater than the dx value, swap them
  if (dy > dx)
  {
    temp = dx;
    dx = dy;
    dy = temp;
    swap = 1;

    ztemp = z0;
    z0 = z1;
    z1 = ztemp;
  }
  else
  {
    swap = 0;
  }

  dz = (1 / z1) - (1 / z0) / dx;

  // set error value
  e = 3 * dy - 2 * dx;

  // 	Draw line pixels based on octant
  image_setColor(src, y, x, c);

  if (l->zBuffer == 1)
  {
    for (int i = 0; i < dx; i++)
    {
      // 	set zBuffer value for current pixel
      if (x > 0 && x < src->cols && y > 0 && y < src->rows)
      {
        zBuf = image_getz(src, y, x);

        if (zInv > zBuf)
        {
          image_setz(src, y, x, zInv);
          image_setColor(src, y, x, c);
        }
      }
      if (e < 0)
      {
        if (swap == 1)
        {
          y += s2;
        }
        else
        {
          x += s1;
        }
        e = e + (2 * dy);
      }
      else
      {
        y += s2;
        x += s1;
        e = e + (2 * dy - 2 * dx);
      }
      zInv += dz;
    }
  }

  // 	if the zBuffer flag is 0
  else
  {
    for (int i = 0; i < dx; i++)
    {
      image_setColor(src, y, x, c);

      if (e < 0)
      {
        if (swap == 1)
        {
          y += s2;
        }
        else
        {
          x += s1;
        }
        e = e + (2 * dy);
      }
      else
      {
        y += s2;
        x += s1;
        e = e + (2 * dy - 2 * dx);
      }
      image_setColor(src, y1, x1, c);
    }
  }
}

// Draw the line into src using color c and the z-buffer, if appropriate.
void line_draw_dotted(Line *l, Image *src, Color c)
{
  int x, y, dy, swap, temp, s1, s2, e;
  double dz, dx, zInv, ztemp;
  float zBuf;

  // 	get values from Line l
  double x0 = l->a.val[0];
  double y0 = l->a.val[1];
  double x1 = l->b.val[0];
  double y1 = l->b.val[1];
  double z0 = l->a.val[2];
  double z1 = l->b.val[2];

  zInv = 1 / z0;
  // 	printf("%f\n", z0);

  x = x0;
  y = y0;

  // 	get absolute value of dx and dx and check the signs
  dx = fabs(x1 - x0);
  dy = fabs(y1 - y0);
  s1 = sign(x1 - x0);
  s2 = sign(y1 - y0);

  // 	if the dy value is greater than the dx value, swap them
  if (dy > dx)
  {
    temp = dx;
    dx = dy;
    dy = temp;
    swap = 1;

    ztemp = z0;
    z0 = z1;
    z1 = ztemp;
  }
  else
  {
    swap = 0;
  }

  dz = (1 / z1) - (1 / z0) / dx;

  // set error value
  e = 3 * dy - 2 * dx;

  // 	Draw line pixels based on octant
  image_setColor(src, y, x, c);

  if (l->zBuffer == 1)
  {
    for (int i = 0; i < dx; i++)
    {
      // 	set zBuffer value for current pixel
      if (x > 0 && x < src->cols && y > 0 && y < src->rows)
      {
        zBuf = image_getz(src, y, x);

        if (zInv > zBuf)
        {
          image_setz(src, y, x, zInv);
          image_setColor(src, y, x, c);
        }
      }
      if (e < 0)
      {
        if (swap == 1)
        {
          y += s2;
        }
        else
        {
          x += s1;
        }
        e = e + (2 * dy);
      }
      else
      {
        y += s2;
        x += s1;
        e = e + (2 * dy - 2 * dx);
      }
      zInv += dz;
    }
  }

  // 	if the zBuffer flag is 0
  else
  {
    for (int i = 0; i < dx; i++)
    {
      image_setColor(src, y, x, c);

      if (e < 0)
      {
        if (swap == 1)
        {
          y += s2;
        }
        else
        {
          x += s1;
        }
        e = e + (2 * dy);
      }
      else
      {
        y += s2;
        x += s1;
        e = e + (2 * dy - 2 * dx);
      }
      image_setColor(src, y1, x1, c);
    }
  }
}

// Circle Primitive Functions

// Initialize to center tc and radius tr.
void circle_set(Circle *c, Point tc, double tr)
{
  point_copy(&(c->c), &tc);
  c->r = tr;
}

// Draw the circle into src using color p.
void circle_draw(Circle *c, Image *src, Color p)
{
  int x = 0;
  int y = c->r;
  int i = 3 - (2 * (c->r));

  /* Plot first set of points */
  circlePlotPoints(src, c->c.val[0], c->c.val[1], x, y, p);

  while (y >= x)
  {
    x++;
    if (i > 0)
    {
      y--;
      i = i + 4 * (x - y) + 10;
    }
    else
    {
      i = i + 4 * x + 6;
    }
    circlePlotPoints(src, c->c.val[0], c->c.val[1], x, y, p);
  }
}

// Draw the points of Circle on the Image.
void circlePlotPoints(Image *src, int xCenter, int yCenter, int x, int y, Color p)
{
  Point xy;

  point_set2D(&xy, xCenter + x, yCenter + y);
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter - x, yCenter + y);
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter + x, yCenter - y);
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter - x, yCenter - y);
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter + y, yCenter + x);
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter - y, yCenter + x);
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter + y, yCenter - x);
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter - y, yCenter - x);
  point_draw(&xy, src, p);
}

// Draw a filled circle into src using color p.
void circle_drawFill(Circle *c, Image *src, Color p)
{
  int x = 0;
  int y = c->r;
  int i = 3 - (2 * (c->r));

  /* Plot first set of points */
  circleFillPlotPoints(src, c->c.val[0], c->c.val[1], x, y, p);

  while (y >= x)
  {
    x++;
    if (i > 0)
    {
      y--;
      i = i + 4 * (x - y) + 10;
    }
    else
    {
      i = i + 4 * x + 6;
    }
    circleFillPlotPoints(src, c->c.val[0], c->c.val[1], x, y, p);
  }
}

// Draw the points and lines between boundaries on the image.
void circleFillPlotPoints(Image *src, int xCenter, int yCenter, int x, int y, Color p)
{
  Point xy;
  Line l;

  point_set2D(&xy, xCenter + x, yCenter + y); //1
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter - x, yCenter + y); //1
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter + y, yCenter + x); //2
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter - y, yCenter + x); //2
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter + x, yCenter - y); //3
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter - x, yCenter - y); //3
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter + y, yCenter - x); //4
  point_draw(&xy, src, p);

  point_set2D(&xy, xCenter - y, yCenter - x); //4
  point_draw(&xy, src, p);

  //1( p.x+x, p.y+y ) to ( p.x-x, p.y+y )
  //2( p.x+y, p.y+x ) to ( p.x-y, p.y+x )
  //3( p.x+x, p.y-y ) to ( p.x-x, p.y-y )
  //4( p.x+y, p.y-x ) to ( p.x-y, p.y-x )

  line_set2D(&l, xCenter + x, yCenter + y, xCenter - x, yCenter + y); //1
  line_draw(&l, src, p);

  line_set2D(&l, xCenter + y, yCenter + x, xCenter - y, yCenter + x); //2
  line_draw(&l, src, p);

  line_set2D(&l, xCenter + x, yCenter - y, xCenter - x, yCenter - y); //3
  line_draw(&l, src, p);

  line_set2D(&l, xCenter + y, yCenter - x, xCenter - y, yCenter - x); //4
  line_draw(&l, src, p);
}

// Ellipse Primitive Functions

// Initialize an ellipse to location tc and radii ta and tb.
void ellipse_set(Ellipse *e, Point tc, double ta, double tb)
{
  point_copy(&(e->c), &tc);
  e->ra = ta;
  e->rb = tb;
}

// Draw into src using color p.
void ellipse_draw(Ellipse *e, Image *src, Color z)
{
  int xCenter, yCenter;
  int Rx, Ry;

  Rx = e->ra;
  Ry = e->rb;

  int Rx2 = Rx * Rx;
  int Ry2 = Ry * Ry;
  int twoRx2 = 2 * Rx2;
  int twoRy2 = 2 * Ry2;
  int p;
  int x = 0;
  int y = Ry;
  int px = 0;
  int py = twoRx2 * y;

  xCenter = e->c.val[0];
  yCenter = e->c.val[1];

  /* Plot the first set of points */
  ellipsePlotPoints(src, xCenter, yCenter, x, y, z);

  /* Region 1 */
  p = ROUND(Ry2 - (Rx2 * Ry) + (0.25 * Rx2));
  while (px < py)
  {
    x++;
    px += twoRy2;
    if (p < 0)
      p += Ry2 + px;
    else
    {
      y--;
      py -= twoRx2;
      p += Ry2 + px - py;
    }
    ellipsePlotPoints(src, xCenter, yCenter, x, y, z);
  }

  /* Region 2 */
  p = ROUND(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
  while (y > 0)
  {
    y--;
    py -= twoRx2;
    if (p > 0)
      p += Rx2 - py;
    else
    {
      x++;
      px += twoRy2;
      p += Rx2 - py + px;
    }
    ellipsePlotPoints(src, xCenter, yCenter, x, y, z);
  }
}

// Draw the points on the Image.
void ellipsePlotPoints(Image *src, int xc, int yc, int x, int y, Color p)
{
  Point xy;

  point_set2D(&xy, xc + x, yc + y);
  point_draw(&xy, src, p);

  point_set2D(&xy, xc - x, yc + y);
  point_draw(&xy, src, p);

  point_set2D(&xy, xc + x, yc - y);
  point_draw(&xy, src, p);

  point_set2D(&xy, xc - x, yc - y);
  point_draw(&xy, src, p);
}

// Draw a filled ellipse into src using color p.
void ellipse_drawFill(Ellipse *e, Image *src, Color z)
{
  int xCenter, yCenter;
  int Rx, Ry;

  Rx = e->ra;
  Ry = e->rb;

  int Rx2 = Rx * Rx;
  int Ry2 = Ry * Ry;
  int twoRx2 = 2 * Rx2;
  int twoRy2 = 2 * Ry2;
  int p;
  int x = 0;
  int y = Ry;
  int px = 0;
  int py = twoRx2 * y;

  xCenter = e->c.val[0];
  yCenter = e->c.val[1];

  /* Plot the first set of points */
  ellipseFillPlotPoints(src, xCenter, yCenter, x, y, z);

  /* Region 1 */
  p = ROUND(Ry2 - (Rx2 * Ry) + (0.25 * Rx2));
  while (px < py)
  {
    x++;
    px += twoRy2;
    if (p < 0)
      p += Ry2 + px;
    else
    {
      y--;
      py -= twoRx2;
      p += Ry2 + px - py;
    }
    ellipseFillPlotPoints(src, xCenter, yCenter, x, y, z);
  }

  /* Region 2 */
  p = ROUND(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
  while (y > 0)
  {
    y--;
    py -= twoRx2;
    if (p > 0)
      p += Rx2 - py;
    else
    {
      x++;
      px += twoRy2;
      p += Rx2 - py + px;
    }
    ellipseFillPlotPoints(src, xCenter, yCenter, x, y, z);
  }
}

// Draw the points and lines between boundaries on the image.
void ellipseFillPlotPoints(Image *src, int xc, int yc, int x, int y, Color p)
{
  Point xy;
  Line l;

  point_set2D(&xy, xc + x, yc + y);
  point_draw(&xy, src, p);

  point_set2D(&xy, xc - x, yc + y);
  point_draw(&xy, src, p);

  point_set2D(&xy, xc + x, yc - y);
  point_draw(&xy, src, p);

  point_set2D(&xy, xc - x, yc - y);
  point_draw(&xy, src, p);

  //1( p.x+x, p.y+y ) to ( p.x-x, p.y+y )
  //2( p.x+x, p.y-y ) to ( p.x-x, p.y-y )

  line_set2D(&l, xc + x, yc + y, xc - x, yc + y); //1
  line_draw(&l, src, p);

  line_set2D(&l, xc + x, yc - y, xc - x, yc - y); //2
  line_draw(&l, src, p);
}

// Polyline Primitive Functions

// Returns an allocated Polyline pointer initialized so that numVertex is 0 and vertex is NULL.
Polyline *polyline_create()
{
  Polyline *polyline = malloc(sizeof(Polyline));

  polyline->numVertex = 0;
  polyline->vertex = NULL;
  polyline->zBuffer = 1;

  return polyline;
}

// Returns an allocated Polyline pointer with the vertex list initialized to the points in vlist.
Polyline *polyline_createp(int numV, Point *vlist)
{
  Polyline *polyline = malloc(sizeof(Polyline));

  polyline->numVertex = numV;

  polyline->vertex = malloc(sizeof(Point *) * numV);

  for (int i = 0; i < numV; i++)
  {
    point_copy(&polyline->vertex[i], &vlist[i]);
  }

  polyline->zBuffer = 1;

  return polyline;
}

// Frees the internal data and the Polyline pointer.
void polyline_free(Polyline *p)
{
  //TODO: Do you need to free vertex too?
  //free(p->vertex);
  if (p->vertex != NULL)
  {
    free(p->vertex);
  }

  free(p);
}

// Initializes the pre-existing Polyline to an empty Polyline.
void polyline_init(Polyline *p)
{
  p->numVertex = 0;
  p->vertex = NULL;
  p->zBuffer = 1;
}

// Initializes the vertex list to the points in vlist. De-allocates/allocates the vertex list for p, as necessary.
void polyline_set(Polyline *p, int numV, Point *vlist)
{
  p->numVertex = numV;

  // Check if NULL, if NULL -> malloc. If not NULL, free then malloc.

  if (p->vertex != NULL)
  {
    free(p->vertex);
  }

  p->vertex = malloc(sizeof(Point *) * p->numVertex);
  for (int i = 0; i < numV; i++)
  {
    point_copy(&p->vertex[i], &vlist[i]);
  }

  p->zBuffer = 1;
}

// Frees the internal data for a Polyline, if necessary, and sets numVertex to 0 and vertex to NULL.
void polyline_clear(Polyline *p)
{
  if (p->vertex != NULL)
  {
    free(p->vertex);
  }

  p->vertex = NULL;
  p->numVertex = 0;
  p->zBuffer = 1;
}

// Sets the z-buffer flag to the given value.
void polyline_zBuffer(Polyline *p, int flag)
{
  p->zBuffer = flag;
}

// De-allocates/allocates space as necessary in the destination Polyline data structure and copies the
// vertex data from the source polyline (from) to the destination (to).
void polyline_copy(Polyline *to, Polyline *from)
{
  to->numVertex = from->numVertex;

  if (to->vertex != NULL)
  {
    free(to->vertex);
  }

  to->vertex = malloc(sizeof(Point *) * to->numVertex);

  for (int i = 0; i < to->numVertex; i++)
  {
    point_copy(&to->vertex[i], &from->vertex[i]);
  }

  to->zBuffer = from->zBuffer;
}

// Prints Polyline data to the stream designated by the FILE pointer.
void polyline_print(Polyline *p, FILE *fp)
{
  int i;
  fprintf(fp, "%d\n", p->numVertex); // writes number of vers
  fprintf(fp, "%d\n", p->zBuffer);   // writes the zBuffer value

  for (i = 0; i < p->numVertex; i++)
  {
    fprintf(fp, "%f %f %f %f\n", p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
  }
}

// Normalize the x and y values of each vertex by the homogeneous coordinate.
void polyline_normalize(Polyline *p)
{
  for (int i = 0; i < p->numVertex; i++)
  {
    point_normalize(&(p->vertex[i]));
  }
}

// Draw the polyline using color c and the z-buffer, if appropriate.
void polyline_draw(Polyline *p, Image *src, Color c)
{
  if (p->numVertex == 1)
  {
    point_draw(p->vertex, src, c);
    return;
  }

  Line l;

  for (int i = 0; i < p->numVertex - 1; i++)
  {
    line_set(&l, p->vertex[i], p->vertex[i + 1]);
    line_draw(&l, src, c);
  }
}

// Fill Functions

// Flood fill algorithm - Using Stack
void floodFill(Point center, Image *src, Color new, Color outline)
{
  st *s = (st *)malloc(sizeof(st));

  createEmptyStack(s);

  Point x;
  point_copy(&x, &center);
  point_draw(&x, src, new);

  Color Black, Temp;
  color_set(&Black, 0.0, 0.0, 0.0);

  push(s, x);

  while (!isempty(s))
  {
    int res = 0;
    Point y;
    x = pop(s);

    //N
    point_set2D(&y, x.val[0], x.val[1] - 1);
    res = image_getColor(src, y.val[1], y.val[0], &Temp);
    if (res != 0 && !color_equals(&Temp, &outline) && !color_equals(&Temp, &new))
    {
      point_draw(&y, src, new);
      push(s, y);
    }

    //S
    point_set2D(&y, x.val[0], x.val[1] + 1);
    res = image_getColor(src, y.val[1], y.val[0], &Temp);
    if (res != 0 && !color_equals(&Temp, &outline) && !color_equals(&Temp, &new))
    {
      point_draw(&y, src, new);
      push(s, y);
    }

    //E
    point_set2D(&y, x.val[0] + 1, x.val[1]);
    res = image_getColor(src, y.val[1], y.val[0], &Temp);
    if (res != 0 && !color_equals(&Temp, &outline) && !color_equals(&Temp, &new))
    {
      point_draw(&y, src, new);
      push(s, y);
    }

    //W
    point_set2D(&y, x.val[0] - 1, x.val[1]);
    res = image_getColor(src, y.val[1], y.val[0], &Temp);
    if (res != 0 && !color_equals(&Temp, &outline) && !color_equals(&Temp, &new))
    {
      point_draw(&y, src, new);
      push(s, y);
    }
  }
}

// Flood Fill algorithm - Using Recursion
void floodFillRecursion(Point center, Image *src, Color new, Color outline)
{
  Point local;
  point_copy(&local, &center);

  Color Temp;

  int res = image_getColor(src, center.val[1], center.val[0], &Temp);
  ;
  int a = color_equals(&Temp, &outline);
  int b = color_equals(&Temp, &new);

  if (res && !a && !b)
  {
    // put new pixel with new color
    point_draw(&center, src, new);

    // recursive call for bottom pixel fill
    point_set2D(&local, center.val[0] + 1, center.val[1]);
    floodFillRecursion(local, src, new, outline);

    // recursive call for top pixel fill
    point_set2D(&local, center.val[0] - 1, center.val[1]);
    floodFillRecursion(local, src, new, outline);

    // recursive call for right pixel fill
    point_set2D(&local, center.val[0], center.val[1] + 1);
    floodFillRecursion(local, src, new, outline);

    // recursive call for left pixel fill
    point_set2D(&local, center.val[0], center.val[1] - 1);
    floodFillRecursion(local, src, new, outline);
  }
}