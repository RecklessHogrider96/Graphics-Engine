/*
  CS5310 - Roshan Bellary
  11-18-2021 (Fall 2021)

  3D View - Z Depth Test Scene - Confetti!
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "vectors.h"
#include "matrix.h"
#include "view3D.h"
#include "module.h"

int main(int argc, char *argv[])
{
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Module *cube, *scene;
  int rows = 360;
  int cols = 640;

  Color Grey;
  Color Yellow;
  Color Blue;

  color_set(&Grey, 220 / 255.0, 50 / 255.0, 50 / 255.0);
  color_set(&Yellow, 50 / 255.0, 220 / 255.0, 50 / 255.0);
  color_set(&Blue, 50 / 255.0, 60 / 255.0, 220 / 255.0);

  DrawState *ds;
  View3D view;

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View parameters
  point_set3D(&(view.vrp), 9, 11, -9);
  vector_set(&(view.vpn), -5, -5, 5);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.6;
  view.dv = .9;
  view.f = 0.0;
  view.b = 15;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // make a simple cube module
  cube = module_create();
  module_scale(cube, 3, 3, 3);
  module_color(cube, &Yellow);
  module_cube(cube, 1);

  scene = module_create();
  module_module(scene, cube);

  module_identity(cube);
  module_color(cube, &Blue);
  module_scale(cube, 2, 2, 2);
  module_translate(cube, 0, 2.5, 0);
  module_cube(cube, 1);

  module_module(scene, cube);

  module_identity(cube);
  module_color(cube, &Grey);
  module_scale(cube, 1, 1, 1);
  module_translate(cube, 0, 4, 0);
  module_cube(cube, 1);

  module_module(scene, cube);

  ds = drawstate_create();
  ds->shade = ShadeDepth;
  ds->scaleFactor = 4;

  for (int i = 0; i < 100; i++)
  {
    char buffer[256];

    image_reset(src);

    matrix_identity(&GTM);
    matrix_rotateX(&GTM, cos(i * 2 * M_PI / 36.0), sin(i * 2 * M_PI / 36.0));
    matrix_rotateY(&GTM, cos(i * 2 * M_PI / 36.0), sin(i * 2 * M_PI / 36.0));
    matrix_rotateZ(&GTM, cos(i * 2 * M_PI / 36.0), sin(i * 2 * M_PI / 36.0));

    if (i > 30 && i < 60)
    {
      matrix_translate(&GTM, cos(i * 2 * M_PI / 15.0), sin(i * 2 * M_PI / 15.0), cos(i * 2 * M_PI / 15.0));
    }
    else if (i > 60)
    {
      matrix_translate(&GTM, cos(i * 4 * M_PI / 36.0), sin(i * 4 * M_PI / 36.0), cos(i * 4 * M_PI / 36.0));
    }

    // write out the image
    module_draw(scene, &VTM, &GTM, ds, NULL, src);
    sprintf(buffer, "p8_3D_scene-%03d.ppm", i);
    image_write(src, buffer);
  }

  // free stuff here
  module_delete(cube);
  image_free(src);

  system("convert -delay 10 p8_3D_scene*.ppm ../images/Z-Depth-Test-Scene.gif");
  system("rm -f p8_3D_scene*");

  return (0);
}
