/*
  CS5310 - Roshan Bellary
	19-12-2021 (Fall 2021)

  Draw a BB8 from star wars!
 */

// edit the include files as necessary
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "graphics.h"
#include "polygon.h"
#include "view2D.h"
#include "vectors.h"
#include "matrix.h"

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
  Color Yellow;
  Color Brown;
  Color DarkGreen;
  Color Blue;
  Color Black;
  Color Cyan;
  Color Orange;
  Point p;
  Line line;
  Ellipse ellipse1;
  Point pt[100];

  const int nLines = 50;
  const int nFrames = 50;
  const int rows = 180;
  const int cols = 320;
  View2D view;
  Matrix vtm;
  Matrix ltm;
  Line lines[nLines];
  Line tline;
  float orient[nLines];
  float freq[nLines];
  Color color[nLines];
  int i, itemInScene;
  char filename[256];

  Polygon *polygon;

  color_set( &White, 1.0, 1.0, 1.0 );
  color_set( &Green, 0.0, 1.0, 0.0 );
  color_set( &Black, 0.0, 0.0, 0.0 );
  color_set( &Yellow, 1.0, 0.85, 0.01 );
  color_set( &DarkGreen, 0.2, 0.4, 0.1 );
  color_set( &Brown, 0.39, 0.26, 0.12 );
  color_set( &Red, 0.9, 0.05, 0.05 );
  color_set( &Blue, 0.1, 0.15, 0.7 );
  color_set( &Cyan, 0.396, 0.709, 0.674 );
  color_set( &Orange, 1.0, 0.647, 0.0 );

  printf("1\n");

  src = image_create( 500, 500 );

  //BG cyan
  point_set2D( &p, 1, 1 );
  floodFill(p, src, Cyan, White);

  //Head
  point_set2D( &p, 250, 108 );
  ellipse_set( &ellipse1, p, 70, 70);
  ellipse_drawFill(&ellipse1, src, White);
  ellipse_draw(&ellipse1, src, Black);
  
  //Eye
  point_set2D( &p, 250, 70 );
  ellipse_set( &ellipse1, p, 15, 15);
  ellipse_drawFill(&ellipse1, src, Black);

  //Eye retina
  point_set2D( &p, 250, 70 );
  ellipse_set( &ellipse1, p, 5, 5);
  ellipse_drawFill(&ellipse1, src, Red);

  //Dimple
  point_set2D( &p, 280, 80 );
  ellipse_set( &ellipse1, p, 5, 5);
  ellipse_drawFill(&ellipse1, src, Black);

  //Antenna
  line_set2D( &line, 280, 43, 280, 0 );
  line_draw( &line, src, Black );

  //Semicircle illusion
  //point_set2D(&(pt[4]), 30, 30);
  point_set2D(&(pt[0]), 180, 109);
  point_set2D(&(pt[1]), 321, 109);
  point_set2D(&(pt[2]), 321, 180);
  point_set2D(&(pt[3]), 180, 180);
  polygon = polygon_createp(4, pt);
  polygon_drawFill(polygon, src, Cyan);

  //Body
  point_set2D( &p, 250, 240 );
  ellipse_set( &ellipse1, p, 130, 130);
  ellipse_drawFill(&ellipse1, src, Black);
  ellipse_drawFill(&ellipse1, src, White);
  ellipse_draw(&ellipse1, src, Black);
  
  //Neck
  point_set2D(&(pt[0]), 180, 108);
  point_set2D(&(pt[1]), 320, 108);
  point_set2D(&(pt[2]), 300, 123);
  point_set2D(&(pt[3]), 200, 123);
  polygon_set(polygon, 4, pt);
  polygon_drawFill(polygon, src, White);
  polygon_draw(polygon, src, Black);

  //Smthg
  point_set2D(&(pt[0]), 190, 98);
  point_set2D(&(pt[1]), 210, 98);
  point_set2D(&(pt[2]), 210, 108);
  point_set2D(&(pt[3]), 190, 108);
  polygon_set(polygon, 4, pt);
  polygon_drawFill(polygon, src, Orange);
  polygon_draw(polygon, src, Black);

  //Smthg 2
  point_set2D(&(pt[0]), 215, 98);
  point_set2D(&(pt[1]), 220, 98);
  point_set2D(&(pt[2]), 220, 108);
  point_set2D(&(pt[3]), 215, 108);
  polygon_set(polygon, 4, pt);
  polygon_drawFill(polygon, src, Black);
  polygon_draw(polygon, src, Black);

  //Smthg 3
  point_set2D(&(pt[0]), 290, 98);
  point_set2D(&(pt[1]), 300, 98);
  point_set2D(&(pt[2]), 300, 108);
  point_set2D(&(pt[3]), 290, 108);
  polygon_set(polygon, 4, pt);
  polygon_drawFill(polygon, src, Blue);
  polygon_draw(polygon, src, Black);

  //Concentric Circles
  point_set2D( &p, 250, 240 );
  ellipse_set( &ellipse1, p, 60, 60);
  ellipse_drawFill(&ellipse1, src, Orange);
  ellipse_draw(&ellipse1, src, Black);

  point_set2D( &p, 250, 240 );
  ellipse_set( &ellipse1, p, 50, 50);
  ellipse_draw(&ellipse1, src, Black);

  point_set2D( &p, 250, 240 );
  ellipse_set( &ellipse1, p, 40, 40);
  ellipse_drawFill(&ellipse1, src, White);
  ellipse_draw(&ellipse1, src, Black);

  //Compartment Lines
  line_set2D( &line, 310, 245, 340, 245 );
  line_draw( &line, src, Black );

  line_set2D( &line, 310, 240, 350, 240 );
  line_draw( &line, src, Black );

  line_set2D( &line, 310, 235, 340, 235 );
  line_draw( &line, src, Black );

  line_set2D( &line, 350, 240, 377, 220 );
  line_draw( &line, src, Black );

  line_set2D( &line, 190, 235, 160, 235 );
  line_draw( &line, src, Black );
  
  line_set2D( &line, 190, 240, 150, 240 );
  line_draw( &line, src, Black );

  line_set2D( &line, 190, 245, 160, 245 );
  line_draw( &line, src, Black );

  line_set2D( &line, 150, 240, 122, 220 );
  line_draw( &line, src, Black );

  //Screws
  point_set2D( &p, 250, 240 );
  point_draw(&p, src, Black);
  ellipse_set( &ellipse1, p, 5, 5);
  ellipse_draw(&ellipse1, src, Black);
  ellipse_set( &ellipse1, p, 7, 7);
  ellipse_draw(&ellipse1, src, Black);

  point_set2D( &p, 180, 170 );
  point_draw(&p, src, Black);
  ellipse_set( &ellipse1, p, 5, 5);
  ellipse_draw(&ellipse1, src, Black);
  ellipse_set( &ellipse1, p, 7, 7);
  ellipse_draw(&ellipse1, src, Black);

  point_set2D( &p, 320, 170 );
  point_draw(&p, src, Black);
  ellipse_set( &ellipse1, p, 5, 5);
  ellipse_draw(&ellipse1, src, Black);
  ellipse_set( &ellipse1, p, 7, 7);
  ellipse_draw(&ellipse1, src, Black);

  // Start creating 2D Scene

  // set up a view centered on (1.5, 1.5) with x pointing right
  point_set2D( &(view.vrp), 1.8, 1.8 );
  view.du = 1.0;
  vector_set( &(view.x), 1.0, 0.0, 0.0 );
  view.screenx = cols;
  view.screeny = rows;

  // Set View 2D
  matrix_setView2D( &vtm, &view );
  printf("vtm:\n");
  matrix_print(&vtm, stdout);

  // loop for some number of frames
  for(t=0;t<nFrames;t++) 
  {
    for (size_t i = 0; i < itemInScene; i++)
    {
      /* code */
    }
    
    printf("writing file\n");
    sprintf(filename, "2D_R2D2-%04d.ppm", t );
    image_write( src, filename );

    // translate the view across the scene
    point_set2D( &(view.vrp), 1.8 - 2.4*(t+1)/nFrames, 1.8 - 2.4*(t+1)/nFrames );
    matrix_setView2D( &vtm, &view );
    matrix_print( &vtm, stdout );
  }


  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
  printf("fun() took %f seconds to execute \n", time_taken);

  //image_write( src, "2D_R2D2.ppm" );

  image_free( src );

  return(0);
}
