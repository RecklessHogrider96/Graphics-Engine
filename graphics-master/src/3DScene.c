/*
  CS5310 - Roshan Bellary
	10-20-2021 (Fall 2021)

  2D View
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graphics.h"
#include "matrix.h"
#include "polygon.h"
#include "view3D.h"

void setSceneBG( Image *src );
void setSceneBG( Image *src ) 
{
  int i, j;
  Color white;

  // starry BG

  color_set(&white, 1, 1, 1);

  for(i=0;i<src->rows;i = i + returnRandom(60, 80)) 
  {
    for(j=0;j<src->cols;j = j + returnRandom(50, 70)) 
    {
      if (returnRandom(0,1))
      {
        image_setColor( src, i, j, white );
      }
    }
  }
}

int main(int argc, char *argv[]) 
{
	const int rows = 1000;
	const int cols = 1000;
	View3D view;
	Matrix vtm;
	Polygon arrowHead[4];
	Polygon tArrowHead;
	Point v[13];
	Point tempV[4];
	Color color[4];
	Image* src;
	char filename[100];

  // Silver Head a
	color_set(&color[0], 0.5, 0.5, 0.5);
  // Bright Silver stick a
	color_set(&color[1], 0.76, 0.07, 0.23);
  // Dark Silver Head b
	color_set(&color[2], 0.41, 0.41, 0.41);
  // Brigh Silver Stick b
	color_set(&color[3], 0.69, 0.13, 0.13);
	
	// initialize polygons
	polygon_init(&arrowHead[0]);
	polygon_init(&arrowHead[1]);
	polygon_init(&arrowHead[2]);
	polygon_init(&arrowHead[3]);

	// triangle of the arrowHead

  // top of the arrowHead
	point_set3D( &v[0], 0, 0, 5);
	point_set3D( &v[1], -2, 0, 2);
	point_set3D( &v[2], 2, 0, 2);

  // root
	point_set3D( &v[3], 1, 0, 2);
	point_set3D( &v[4], -1, 0, 2);
	point_set3D( &v[5], 1, 0, 0);
	point_set3D( &v[6], -1, 0, 0);

	// second arrowHead
	point_set3D( &v[7], 0, -2, 2);
	point_set3D( &v[8], 0, 2, 2);

  // root
	point_set3D( &v[9], 0, 1, 2);

	point_set3D( &v[10], 0, -1, 2);
	point_set3D( &v[11], 0, 1, 0);
	point_set3D( &v[12], 0, -1, 0);

	// first arrowHead (0,1,2)
	polygon_set(&arrowHead[0], 3, &v[0]);// triangle

  //  stick (3,4,5,6)
  point_copy(&tempV[0], &v[3]);
  point_copy(&tempV[1], &v[5]);
  point_copy(&tempV[2], &v[4]);
  point_copy(&tempV[3], &v[6]);
	polygon_set(&arrowHead[1], 4, &tempV[0]);// root

	// second arrowHead

	// triangle (0,7,8)
	point_copy(&tempV[0], &v[0]);
  point_copy(&tempV[1], &v[7]);
  point_copy(&tempV[2], &v[8]);
  polygon_set(&arrowHead[2], 3, &tempV[0]);
  
  // root (9,10,11,12)
  point_copy(&tempV[0], &v[9]);
  point_copy(&tempV[1], &v[11]);
  point_copy(&tempV[2], &v[10]);
  point_copy(&tempV[3], &v[12]);
  polygon_set(&arrowHead[3], 4, &tempV[0]);

  // setting the viewpoint and the view structure
  point_set3D( &(view.vrp), 3, 9, 3);
  // Look direction = (look at 0, 0, 0) - vrp
  vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]);
  // Up vector -> (0,1,0)
  vector_set( &(view.vup), 0, -1, 0 );
	
  view.du = 2;
	view.dv = view.du * (float)rows / cols;
	view.d = 2;  //  focal length

	view.f = 0; //  front clip plane
	view.b = 7; //  back clip plane

	view.screenx = cols;
	view.screeny = rows;

	matrix_setView3D( &vtm, &view );

  polygon_init( &tArrowHead);

  int i,j;
  double x= -5;
  for (j=0; j<50; j++)
  {
    src = image_create( rows, cols );
    point_set3D( &(view.vrp), x, 5, 3);
    vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]+j/10.0);
    matrix_setView3D( &vtm, &view );

    setSceneBG(src);

    for(i=0;i<4;i++) 
    {
      polygon_copy( &tArrowHead, &arrowHead[i] );
      matrix_xformPolygon( &vtm, &tArrowHead );

      //  normalize by homogeneous coordinate
      polygon_normalize( &tArrowHead );
      polygon_drawFill( &tArrowHead, src, color[i]);
    }
    
    x = x + 0.1;
    sprintf(filename, "3DScene_%04d.ppm", j);
    
    // write the image
    image_write(src, filename);
    
    // free resources
    image_free(src);
  }

  system("convert -delay 10 3DScene_*.ppm ../images/3DScene.gif");
	system("rm -f 3DScene_*");
}