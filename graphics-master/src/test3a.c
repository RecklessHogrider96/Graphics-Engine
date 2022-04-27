/*
  CS5310 - Roshan Bellary
	09-28-2021 (Fall 2021)

  Test function 1 for the graphics primitives assignment
 */

// edit the include files as necessary
#include <stdio.h>
#include "graphics.h"

// draw a box
static int box( Image *src, Color color, int x, int y, int dx, int dy );
static int box( Image *src, Color color, int x, int y, int dx, int dy ) {
  Line l;
  
  line_set2D( &l, x, y, x, y+dy );
  line_draw( &l, src, color );

  line_set2D( &l, x, y+dy, x + dx, y+dy );
  line_draw( &l, src, color );

  line_set2D( &l, x+dx, y+dy, x+dx, y );
  line_draw( &l, src, color );

  line_set2D( &l, x+dx, y, x, y );
  line_draw( &l, src, color );

  return(0);
}

// draw a few boxes, a circle, and some lines.
int main(int argc, char *argv[]) {
  Image *src;
  Color White;
  Color Red;
  Point p;
  Circle circ;
  Line line;
  Ellipse ellipse;

  color_set( &White, 1.0, 1.0, 1.0 );
  color_set( &Red, 0.9, 0.05, 0.05 );

  printf("1\n");

  src = image_create( 400, 600 );

  printf("2\n");

  box( src, White, 100, 100, 100, 20 );
  printf("3\n");

  box( src, White, 100, 200, 100, 20 );
  printf("4\n");

  box( src, White, 150, 140, 70, 40 );
  printf("5\n");

  box( src, White, 160, 120, 10, 20 );
  printf("6\n");

  box( src, White, 160, 180, 10, 20 );
  printf("7\n");

  point_set2D( &p, 260, 160 );
  circle_set( &circ, p, 60 );
  printf("8\n");

  circle_draw( &circ, src, White );
  printf("9\n");

  point_set2D( &p, 260, 160 );
  ellipse_set( &ellipse, p, 30, 60);
  printf("9a\n");

  ellipse_draw(&ellipse, src, White );
  printf("9b\n");

  line_set2D( &line, 290, 210, 470, 300);
  line_draw( &line, src, Red );
  printf("10\n");

  line_set2D( &line, 295, 205, 475, 280);
  line_draw( &line, src, Red );
  printf("11\n");
  
  image_write( src, "test3a.ppm" );

  image_free( src );

  return(0);
}
