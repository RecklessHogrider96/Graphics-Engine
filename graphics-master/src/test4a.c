/*
  Assignment 4, required image 1

  C version

  CS5310 - Roshan Bellary
	10-05-2021 (Fall 2021)
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

/*
  Program to test polygon functionality
*/
int main(int argc, char *argv[]) {
  Image *src;
  const int rows = 300;
  const int cols = 400;
  Polygon *p;
  Color Red;
  Color Orange;
  Color White;
  Color Blue;
  Point pt[100];
  int i;
  
  srand(42);

  color_set(&Red, 0.9, 0.2, 0.1 );
  color_set(&Orange, 0.95, 0.7, 0.3 );
  color_set(&White, 1.0, 1.0, 1.0 );
  color_set(&Blue, 0.2, 0.1, 0.95 );

  src = image_create(rows, cols);

  // make a simple square to test proper areas and locations
  // the square ought to be 20x20, include pixel (30,30) and exclude pixel (50, 50)
  point_set2D(&(pt[0]), 30, 30);
  point_set2D(&(pt[1]), 50, 30);
  point_set2D(&(pt[2]), 50, 50);
  point_set2D(&(pt[3]), 30, 50);

  p = polygon_createp(4, pt);

  printf("drawing a square\n");
  //polygon_draw(p, src, White);
  polygon_drawFill(p, src, Blue);

  // something more interesting
  for(i=0;i<50;i++) {
    float dr = rand() % 20;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(70 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(70 + dr));
  }
  polygon_set(p, 50, pt);

  for (int i = 0; i < 50; i++)
  {
    printf("P vertices (%f,%f) \n", pt[i].val[0], pt[i].val[1]);
  }
  

  printf("drawing first big polygon\n");
  //polygon_draw(p, src, White);
  polygon_drawFill(p, src, Red);

  for(i=0;i<50;i++) {
    float dr = rand() % 15;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(50 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(50 + dr));
  }

  printf("set 2nd polygon\n");
  polygon_set(p, 50, pt);

  printf("drawing second big polygon\n");
  //polygon_draw(p, src, White);
  polygon_drawFill(p, src, Orange);

  for(i=0;i<50;i++) {
    float dr = rand() % 10;
    point_set2D(&(pt[i]), 
    200 + cos((float)i * M_PI * 2.0 / 50.0)*(30 + dr),
    150 + sin((float)i * M_PI * 2.0 / 50.0)*(30 + dr));
  }

  printf("set 3rd polygon\n");
  polygon_set(p, 50, pt);

  printf("drawing third big polygon\n");
  //polygon_draw(p, src, White);
  polygon_drawFill(p, src, White);

  printf("writing output\n");
  image_write(src, "test4a.ppm");

  image_free(src);

  return(0);
}
