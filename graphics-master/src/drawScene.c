/*
  CS5310 - Roshan Bellary
	09-28-2021 (Fall 2021)

  Draw a pretty picture!
 */

// edit the include files as necessary
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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

  clock_t t;
  t = clock();

  Image *src;
  Color White;
  Color Red;
  Color Green;
  Color Black;
  Point p;
  Circle circ;
  Line line;
  Ellipse ellipse;
  Ellipse ellipse1;
  Ellipse ellipse2;

  color_set( &White, 1.0, 1.0, 1.0 );
  color_set( &Green, 0.0, 1.0, 0.0 );
  color_set( &Black, 0.0, 0.0, 0.0 );
  color_set( &Red, 0.9, 0.05, 0.05 );

  printf("1\n");

  src = image_create( 500, 500 );

  point_set2D( &p, 250, 250 );
  for (int i = 10; i < 400; i = i + 10)
  {
    circle_set( &circ, p, i );
    circle_draw( &circ, src, White );
  }

  int alt = 0;
  for (int i = 0; i < 400; i = i + 1)
  {
    if (alt)
    {
      floodFill(p, src, Red, White);
      alt = 0;
    }
    else
    {
      floodFill(p, src, Green, White);
      alt = 1;
    }
    p.val[1] += i;
  }
  
  for(int i=0;i<10000;i++) 
  {
    point_set2D( &(p), drand48()*src->cols, drand48()*src->rows);
    point_draw(&p, src, Black);
  }

  point_set2D( &p, 250, 250 );

  for (int i = 0; i < 20; i++)
  {
    line_set2D( &line, p.val[1], p.val[0], p.val[1] + (250.0 * cos(i*18)), p.val[0] + (250.0 * sin(i*18)));
    line_draw( &line, src, Black );
  }
  
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
  printf("fun() took %f seconds to execute \n", time_taken);

  image_write( src, "finalArt.ppm" );

  image_free( src );

  return(0);
}
