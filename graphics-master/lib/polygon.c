#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polygon.h"

#define USECPP 0

// Polygon Primitive Funtions

// Returns an allocated Polygon pointer initialized so that numVertex is 0 and vertex is NULL.
Polygon *polygon_create()
{
  Polygon *polygon = malloc(sizeof(Polygon));

  polygon->nVertex = 0;
  polygon->vertex = NULL;
  polygon->normal = NULL;
  polygon->zBuffer = 1;

  return polygon;
}

// Returns an allocated Polygon pointer with the vertex list initialized to a copy of the points in vlist.
Polygon *polygon_createp(int numV, Point *vlist)
{
  Polygon *polygon = malloc(sizeof(Polygon));
  ;

  polygon->nVertex = numV;

  polygon->vertex = malloc(sizeof(Point) * numV);

  for (int i = 0; i < numV; i++)
  {
    point_copy(&polygon->vertex[i], &vlist[i]);
  }

  polygon->zBuffer = 1; // [Done] BAM set this to 1 by default

  return polygon;
}

// Frees the internal data for a Polygon and the Polygon pointer.
void polygon_free(Polygon *p)
{
  // TODO: Do you need to free vertex?
  // [Done] BAM free only if p->vertex is NULL
  if (p->vertex != NULL)
  {
    free(p->vertex);
  }
  free(p);
}

// Initializes the existing Polygon to an empty Polygon
void polygon_init(Polygon *p)
{
  p->nVertex = 0;
  // [Done] BAM: the init function assumes all information in the structure is garbage, just set it to NULL
  p->zBuffer = 1;
  p->vertex = NULL;
  p->normal = NULL;
  p->color = NULL;
  p->onesided = 1;
}

// Initializes the vertex array to the points in vlist.
void polygon_set(Polygon *p, int numV, Point *vlist)
{
  p->nVertex = numV;

  if (p->vertex != NULL)
  {
    free(p->vertex);
  }

  p->vertex = malloc(sizeof(Point) * numV);

  for (int i = 0; i < numV; i++)
  {
    point_copy(&p->vertex[i], &vlist[i]);
  }

  p->zBuffer = 1;
}

// Frees the internal data and resets the fields.
void polygon_clear(Polygon *p)
{
  if (p->vertex != NULL)
  {
    free(p->vertex);
  }

  if (p->color != NULL)
  {
    //free(p->color);
  }

  p->zBuffer = 1;
  p->nVertex = 0;
  p->vertex = NULL;
  p->color = NULL;
  p->onesided = 1;
}

// Sets the oneSided field to the value.
void polygon_setSided(Polygon *p, int oneSided)
{
  p->onesided = oneSided;
}

// De-allocates/allocates space and copies the vertex and color data from one polygon to the other.
void polygon_copy(Polygon *to, Polygon *from)
{
  int i;
  int numVertex = from->nVertex;
  if (!(to->nVertex == numVertex))
  {
    if (to->vertex)
    {
      free(to->vertex);
    }
    to->vertex = malloc(sizeof(Point) * numVertex);
    to->nVertex = numVertex;
  }

  for (i = 0; i < numVertex; i++)
  {
    to->vertex[i] = from->vertex[i];
  }

  to->zBuffer = from->zBuffer;
}

// Prints polygon data to the stream designated by the FILE pointer.
void polygon_print(Polygon *p, FILE *fp)
{
  printf("Polygon: %d vertices\n", p->nVertex);
  for (int i = 0; i < p->nVertex; i++)
  {
    point_print(&p->vertex[i], fp);
  }
}

// Normalize the x and y values of each vertex by the homogeneous coord
void polygon_normalize(Polygon *p)
{
  //go through every point in vertex list
  for (int i = 0; i < p->nVertex; i++)
  {
    double h = p->vertex[i].val[3];
    double x = p->vertex[i].val[0];
    double y = p->vertex[i].val[1];
    p->vertex[i].val[0] = x / h;
    p->vertex[i].val[1] = y / h;
  }
}

// Draw the outline of the polygon using color c
void polygon_draw(Polygon *p, Image *src, Color c)
{
  if (p->nVertex == 1)
  {
    point_draw(p->vertex, src, c);
    return;
  }

  Line l;

  for (int i = 0; i < p->nVertex - 1; i++)
  {
    line_set(&l, p->vertex[i], p->vertex[i + 1]);
    line_draw(&l, src, c);
  }

  // Join the first and last point
  line_set(&l, p->vertex[0], p->vertex[p->nVertex - 1]);
  line_draw(&l, src, c);
}

/*
Auxilary function to be used with qsort
*/
static int compare(const void *a, const void *b)
{
  return (*(int *)a - *(int *)b);
}

// Draw the filled polygon using color c with the Barycentric coordinates algorithm.
void polygon_drawFillB(Polygon *p, Image *src, Color c)
{
  const float epsilon = 0.00001;
  float alpha, beta, gamma;
  float xlist[3] = {p->vertex[0].val[0], p->vertex[1].val[0], p->vertex[2].val[0]};
  float ylist[3] = {p->vertex[0].val[1], p->vertex[1].val[1], p->vertex[2].val[1]};
  qsort(xlist, 3, sizeof(float), compare);
  qsort(ylist, 3, sizeof(float), compare);
  int Xmin = (int)(xlist[0] + 0.5);
  int Ymin = (int)(ylist[0] + 0.5);
  int Xmax = (int)(xlist[2] + 0.5);
  int Ymax = (int)(ylist[2] + 0.5);

  for (int i = Ymin; i < Ymax; i++)
  {
    for (int j = Xmin; j < Xmax; j++)
    {
      beta = Aux_implicit_line(p->vertex[0], p->vertex[2], j, i) / Aux_implicit_line(p->vertex[0], p->vertex[2], p->vertex[1].val[0], p->vertex[1].val[1]);
      gamma = Aux_implicit_line(p->vertex[0], p->vertex[1], j, i) / Aux_implicit_line(p->vertex[0], p->vertex[1], p->vertex[2].val[0], p->vertex[2].val[1]);
      alpha = 1 - beta - gamma;

      if (beta < -epsilon || gamma < -epsilon || alpha < -epsilon)
      {
        continue;
      }
      else
      {
        image_setColor(src, i, j, c);
      }
    }
  }
}

/*
Auxilary function to calculate the distance of point p_in from
the line segment p1p2
*/

float Aux_implicit_line(Point p1, Point p2, float x, float y)
{
  float x0 = p1.val[0];
  float y0 = p1.val[1];

  float x1 = p2.val[0];
  float y1 = p2.val[1];
  x += 0.5;
  y += 0.5;
  return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

// Calculate  and return f01(x,y).
double barycentric_points(double x0, double y0, double x1, double y1, double x, double y)
{
  double res = 0;

  // f01(x,y) = (y0-y1) * x + (x1 - x0) * y + (x0*y1) - (x1 * y0)
  res = ((y0 - y1) * x) + ((x1 - x0) * y) + (x0 * y1) - (x1 * y0);

  return res;
}

// Draw just the outline of the polygon, similar to the polyline_draw function,
// except it needs to connect the last and first points to complete the shape.
void polygon_drawFrame(Polygon *p, Image *src, Color c)
{
  Line line;
  int x0, x1, y0, y1;
  //go through every point in vertex list
  for (int i = 0; i < p->nVertex - 1; i++)
  {
    //get x and y values
    x0 = p->vertex[i].val[0];
    y0 = p->vertex[i].val[1];
    x1 = p->vertex[i + 1].val[0];
    y1 = p->vertex[i + 1].val[1];
    //set line
    line_set2D(&line, x0, y0, x1, y1);
    //draw line
    line_draw(&line, src, c);
  }
  x0 = p->vertex[0].val[0];
  y0 = p->vertex[0].val[1];
  line_set2D(&line, x0, y0, x1, y1);
  line_draw(&line, src, c);
}
