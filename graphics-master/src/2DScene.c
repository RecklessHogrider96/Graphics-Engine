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
#include "view2D.h"

void setSceneBG( Image *src );
void setSceneBG( Image *src ) 
{
  int i, j;
  Color blue;

  color_set(&blue, 0, 0, 0);

  for(i=0;i<src->rows;i++) 
  {
    for(j=0;j<src->cols;j++) 
    {
        image_setColor( src, i, j, blue );
    }
  }
}

int main(int argc, char *argv[]) 
{
  const int nFrames = 100;
  const int rows = 500;
  const int cols = 500;
  View2D view;
  Matrix vtm;
  Matrix ltm;

  Polygon bus, tbus, window, twindow, shadow, tshadow;
  Circle wheels[2], lights[2];
  Point busVertexs[4], windowVertexs[5], wheelVertexs[2], lightsVertexs[2], twheels[2], tlights[2], sahdowVertexs[4];
  Color red,yellow,white,black,grey;
  int t, i;
  float step;
  char filename[256];
  Image *src;

  src = image_create( rows, cols );

  color_set(&red, 1.0, 0.4, 0.1);
  color_set(&white, 1.0, 1.0, 1.0);
  color_set(&yellow, 1.0, 1.0, 0.0);
  color_set(&grey, 0.3, 0.3, 0.3);
  color_set(&black, 0.2, 0.2, 0.2);

  point_set2D(&busVertexs[0],0,0);
  point_set2D(&busVertexs[1],0,25);
  point_set2D(&busVertexs[2],35,25);
  point_set2D(&busVertexs[3],35,0);

  point_set2D(&windowVertexs[0],30,15);
  point_set2D(&windowVertexs[1],30,25);
  point_set2D(&windowVertexs[2],35,25);
  point_set2D(&windowVertexs[3],35,15);

  point_set2D(&wheelVertexs[0],10,0);
  point_set2D(&wheelVertexs[1],25,0);

  point_set2D(&lightsVertexs[0],35,3);
  point_set2D(&lightsVertexs[1],35,5);

  point_set2D(&sahdowVertexs[0], 0, -10);
  point_set2D(&sahdowVertexs[1], 20, -10);
  point_set2D(&sahdowVertexs[2],25,15);
  point_set2D(&sahdowVertexs[3],25,0);

  // set up a view centered on (1.8, 1.8) with x pointing right
  point_set2D( &(view.vrp), 1.8, 1.8 );
  view.du = 100.0;
  vector_set( &(view.x), 1.0, 0.0, 0.0 );
  view.screenx = cols;
  view.screeny = rows;

  matrix_setView2D( &vtm, &view );
  printf("vtm:\n");
  matrix_print(&vtm, stdout);

  // local translation
  polygon_set(&bus,4,busVertexs);
  polygon_set(&window,4,windowVertexs);
  polygon_set(&shadow,4,sahdowVertexs);

  // loop for some number of frames
  i = 0 ;
  for(t=0;t<nFrames;t++) 
  {
    setSceneBG( src );

    // copies polygons to eliminate change of original points
    polygon_copy(&tbus,&bus);
    polygon_copy(&twindow,&window);
    polygon_copy(&tshadow,&shadow);

    // copy circle points
    point_copy(&twheels[0],&wheelVertexs[0]);
    point_copy(&twheels[1],&wheelVertexs[1]);

    // copy light points
    point_copy(&tlights[0],&lightsVertexs[0]);
    point_copy(&tlights[1],&lightsVertexs[1]);

    // local translation
    matrix_identity(&ltm);
    matrix_scale2D(&ltm, 1 - ( t * .01), 1 - ( t * 0.01));
    matrix_translate2D(&ltm, -45 + ( t * 0.01), -30 + ( t * .01));
    // Color_set(&red,1,0,0);

    // bus and window setting
    matrix_xformPolygon(&ltm,&tbus);
    matrix_xformPolygon(&vtm,&tbus);

    matrix_xformPolygon(&ltm,&twindow);
    matrix_xformPolygon(&vtm,&twindow);

    matrix_xformPolygon(&ltm,&tshadow);
    matrix_xformPolygon(&vtm,&tshadow);

    // wheels setting
    matrix_xformPoint(&ltm,&twheels[0],&twheels[0]);
    matrix_xformPoint(&ltm,&twheels[1],&twheels[1]);
    matrix_xformPoint(&vtm,&twheels[0],&twheels[0]);
    matrix_xformPoint(&vtm,&twheels[1],&twheels[1]);

    circle_set(&wheels[0],twheels[0],20-(t*.1));
    circle_set(&wheels[1],twheels[1],20-(t*.1));

    // lights setting
    matrix_xformPoint(&ltm,&tlights[0],&tlights[0]);
    matrix_xformPoint(&ltm,&tlights[1],&tlights[1]);
    matrix_xformPoint(&vtm,&tlights[0],&tlights[0]);
    matrix_xformPoint(&vtm,&tlights[1],&tlights[1]);

    circle_set(&lights[0],tlights[0],15-(t*.1));
    circle_set(&lights[1],tlights[1],15-(t*.1));

    // drawing
    polygon_drawFill(&tshadow,src,black);
    polygon_drawFill(&tbus,src,red);
    polygon_drawFill(&twindow,src,white);
    circle_drawFill(&wheels[0],src,grey);
    circle_drawFill(&wheels[1],src,grey);
    circle_drawFill(&lights[0],src,yellow);
    circle_drawFill(&lights[1],src,yellow);

    printf("writing file\n");
    sprintf(filename, "frame-%04d.ppm", t );
    image_write( src, filename );

    // translate the view across the scene
    if (t < 41)
    {
      // Move up.
      step = 160 * ( t + 1 ) / nFrames; 
      point_set2D( &(view.vrp), 1.8, 1.8 - step);
    }
    else
    {
      // Move down.
      i = step;
      point_set2D( &(view.vrp), 1.8, 1.8 - i + ((t + 1) / nFrames));
    }
    
    matrix_setView2D( &vtm, &view );
    matrix_print( &vtm, stdout );
  }

  printf("Make a .gif?");
  system("convert -delay 10 frame-*.ppm ../images/2DScene.gif");
  system("rm -f frame-*");

  image_free( src );

  return(0);
}