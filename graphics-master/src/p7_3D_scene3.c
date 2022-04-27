/*
  CS5310 - Roshan Bellary
  11-10-2021 (Fall 2021)

  3D Scene View
*/

#include <stdlib.h>
#include <math.h>

#include "module.h"
#include "view3D.h"

int main(int argc, char *argv[])
{
  Color blue, green, purple, red, white, grey, greyer;

  Module *sceneRoot;
  Module *cube;
  Module *bsmodule;
  Point p[32];

  int rows = 600;
  int cols = 600;
  int frames = 50;
  Image *src;
  char filename[100];
  int divisions = 4;

  color_set(&white, 1.0, 1.0, 1.0);
  color_set(&blue, .1, .2, .8);
  color_set(&green, .2, 0.7, 0.3);
  color_set(&purple, 0.6, 0.1, 0.7);
  color_set(&red, 0.75, 0.3, 0.3);
  color_set(&grey, 0.41, 0.41, 0.41);
  color_set(&greyer, 0.51, 0.51, 0.51);

  View3D view;
  Matrix VTM, GTM;
  BezierSurface bc;
  DrawState ds;

  // grab the command line argument, if one exists
  if (argc > 1)
  {
    int tmp = atoi(argv[1]);
    if (tmp >= 0 && tmp < 10)
      divisions = tmp;
  }
  printf("Creating Bezier curves with %d subdivisions\n", divisions);

  // Add cones to Scene
  sceneRoot = module_create();

  // Body surface Top
  point_set3D(&p[0], -1.0, 1.0, -1.0); // first row, constant x, even spacing in z
  point_set3D(&p[1], -1.0, 1.0, -0.33);
  point_set3D(&p[2], -1.0, 1.0, 0.66);
  point_set3D(&p[3], -1.0, 1.0, 1.0);

  point_set3D(&p[4], -0.33, 1.0, -1.0); // second row
  point_set3D(&p[5], -0.33, 1.0, -0.33);
  point_set3D(&p[6], -0.33, 1.0, 0.66);
  point_set3D(&p[7], -0.33, 1.0, 1.0);

  point_set3D(&p[8], 0.66, 1.0, -1.0); // third row
  point_set3D(&p[9], 0.66, 1.0, -0.33);
  point_set3D(&p[10], 0.66, 1.0, 0.66);
  point_set3D(&p[11], 0.66, 1.0, 1.0);

  point_set3D(&p[12], 1.0, 1.0, -1.0); // fourth row
  point_set3D(&p[13], 1.0, 1.0, -0.33);
  point_set3D(&p[14], 1.0, 1.0, 0.66);
  point_set3D(&p[15], 1.0, 1.0, 1.0);

  bezierSurface_set(&bc, p);

  bsmodule = module_create();
  module_translate(bsmodule, 0, 0, 0);
  module_color(bsmodule, &white);
  module_bezierSurface(bsmodule, &bc, divisions, 0);
  module_module(sceneRoot, bsmodule);

  // Body surface Bottom
  point_set3D(&p[0], -1.0, -1.0, -1.0); // first row, constant x, even spacing in z
  point_set3D(&p[1], -1.0, -1.0, -0.33);
  point_set3D(&p[2], -1.0, -1.0, 0.66);
  point_set3D(&p[3], -1.0, -1.0, 1.0);

  point_set3D(&p[4], -0.33, -1.0, -1.0); // second row
  point_set3D(&p[5], -0.33, -1.0, -0.33);
  point_set3D(&p[6], -0.33, -1.0, 0.66);
  point_set3D(&p[7], -0.33, -1.0, 1.0);

  point_set3D(&p[8], 0.66, -1.0, -1.0); // third row
  point_set3D(&p[9], 0.66, -1.0, -0.33);
  point_set3D(&p[10], 0.66, -1.0, 0.66);
  point_set3D(&p[11], 0.66, -1.0, 1.0);

  point_set3D(&p[12], 1.0, -1.0, -1.0); // fourth row
  point_set3D(&p[13], 1.0, -1.0, -0.33);
  point_set3D(&p[14], 1.0, -1.0, 0.66);
  point_set3D(&p[15], 1.0, -1.0, 1.0);
  bezierSurface_set(&bc, p);

  bsmodule = module_create();
  module_translate(bsmodule, 0, 0, 0);
  module_color(bsmodule, &white);
  module_bezierSurface(bsmodule, &bc, divisions, 0);
  module_module(sceneRoot, bsmodule);

  // Body surface Side
  point_set3D(&p[0], -1.0, 0.0, -1.0); // first row, constant x, even spacing in z
  point_set3D(&p[1], -1.0, 0.0, -0.33);
  point_set3D(&p[2], -1.0, 0.0, 0.66);
  point_set3D(&p[3], -1.0, 0.0, 1.0);

  point_set3D(&p[4], -0.33, 0.0, -1.0); // second row
  point_set3D(&p[5], -0.33, 0.0, -0.33);
  point_set3D(&p[6], -0.33, 0.0, 0.66);
  point_set3D(&p[7], -0.33, 0.0, 1.0);

  point_set3D(&p[8], 0.66, 0.0, -1.0); // third row
  point_set3D(&p[9], 0.66, 0.0, -0.33);
  point_set3D(&p[10], 0.66, 0.0, 0.66);
  point_set3D(&p[11], 0.66, 0.0, 1.0);

  point_set3D(&p[12], 1.0, 0.0, -1.0); // fourth row
  point_set3D(&p[13], 1.0, 0.0, -0.33);
  point_set3D(&p[14], 1.0, 0.0, 0.66);
  point_set3D(&p[15], 1.0, 0.0, 1.0);
  bezierSurface_set(&bc, p);

  // Left face
  bsmodule = module_create();
  module_translate(bsmodule, 0, 0, 0);
  module_rotateZ(bsmodule, -cos(1.0 * M_PI / 2.0), -sin(1.0 * M_PI / 2.0));
  module_translate(bsmodule, -1, 0, 0);
  module_color(bsmodule, &white);
  module_bezierSurface(bsmodule, &bc, divisions, 0);
  module_module(sceneRoot, bsmodule);

  // Right face
  bsmodule = module_create();
  module_translate(bsmodule, 0, 0, 0);
  module_rotateZ(bsmodule, -cos(1.0 * M_PI / 2.0), -sin(1.0 * M_PI / 2.0));
  module_translate(bsmodule, 1, 0, 0);
  module_color(bsmodule, &white);
  module_bezierSurface(bsmodule, &bc, divisions, 0);
  module_module(sceneRoot, bsmodule);

  // Back face
  bsmodule = module_create();
  module_translate(bsmodule, 0, 0, 0);
  module_rotateX(bsmodule, -cos(1.0 * M_PI / 2.0), -sin(1.0 * M_PI / 2.0));
  module_translate(bsmodule, 0, 0, -1);
  module_color(bsmodule, &white);
  module_bezierSurface(bsmodule, &bc, divisions, 0);
  module_module(sceneRoot, bsmodule);

  // Front face
  bsmodule = module_create();
  module_translate(bsmodule, 0, 0, 0);
  module_rotateX(bsmodule, -cos(1.0 * M_PI / 2.0), -sin(1.0 * M_PI / 2.0));
  module_translate(bsmodule, 0, 0, 1);
  module_color(bsmodule, &white);
  module_bezierSurface(bsmodule, &bc, divisions, 0);
  module_module(sceneRoot, bsmodule);

  // create a Heart
  // put the curve into a module
  // Tetrahedron
  cube = module_create();
  module_translate(cube, -0.5, 0, -0.5);
  module_color(cube, &red);
  module_tetrahedron(cube);
  module_module(sceneRoot, cube);

  // Cone
  //module_scale(sceneRoot, 0.4, 1.0, 0.4);
  // module_translate(sceneRoot, 0, 0, 0);
  module_color(sceneRoot, &blue);
  module_translate(sceneRoot, 0, 2, 0);
  module_cone(sceneRoot, 100);

  module_translate(sceneRoot, 0.5, -4, -0.5);

  point_set3D(&p[0], 0.2, -0.2, 0.2); // first row, constant x, even spacing in z
  point_set3D(&p[1], 0.0, -0.2, 0.33);
  point_set3D(&p[2], 0.0, -0.2, 0.66);
  point_set3D(&p[3], 0.2, -0.2, 0.8);
  point_set3D(&p[4], 0.33, -0.2, 0.0); // second row
  point_set3D(&p[5], 0.13, -2.6, 0.33);
  point_set3D(&p[6], 0.13, -2.6, 0.66);
  point_set3D(&p[7], 0.33, -0.2, 1.0);
  point_set3D(&p[8], 0.66, -0.2, 0.0); // third row
  point_set3D(&p[9], 0.86, -2.6, 0.33);
  point_set3D(&p[10], 0.86, -2.6, 0.66);
  point_set3D(&p[11], 0.66, -0.2, 1.0);
  point_set3D(&p[12], 0.8, -0.2, 0.2); // fourth row
  point_set3D(&p[13], 1.0, -0.2, 0.33);
  point_set3D(&p[14], 1.0, -0.2, 0.66);
  point_set3D(&p[15], 0.8, -0.2, 0.8);
  bezierSurface_set(&bc, p);

  // put the curve into a module
  // module_rotateX(sceneRoot, cos(1.0 * M_PI / 1.0), sin(1.0 * M_PI / 1.0));
  module_translate(sceneRoot, 0, 1, 0);

  bsmodule = module_create();
  module_translate(bsmodule, -0.5, 0, 0);
  module_color(bsmodule, &white);
  module_bezierSurface(bsmodule, &bc, divisions, 0);
  module_module(sceneRoot, bsmodule);

  // put the curve into a module
  bsmodule = module_create();
  module_translate(bsmodule, -1.5, 0, 0);
  module_color(bsmodule, &white);
  module_bezierSurface(bsmodule, &bc, divisions, 0);
  module_module(sceneRoot, bsmodule);

  // put the curve into a module LH
  bsmodule = module_create();
  module_rotateZ(bsmodule, cos(1.0 * M_PI / 2.0), sin(1.0 * M_PI / 2.0));
  module_translate(bsmodule, 0.3, 1, 0);
  module_color(bsmodule, &white);
  module_bezierSurface(bsmodule, &bc, divisions, 0);
  module_module(sceneRoot, bsmodule);

  // put the curve into a module RH
  bsmodule = module_create();
  module_rotateZ(bsmodule, -cos(1.0 * M_PI / 2.0), -sin(1.0 * M_PI / 2.0));
  module_translate(bsmodule, -1.3, 2, 0);
  module_color(bsmodule, &white);
  module_bezierSurface(bsmodule, &bc, divisions, 0);
  module_module(sceneRoot, bsmodule);

  // set up the view
  point_set3D(&(view.vrp), 0.0, 3.2, -7.0);
  vector_set(&(view.vpn), 0.0, -0.8, 2.5);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 1.5;
  view.du = 1.0;
  view.dv = 1.0 * rows / cols;
  view.screeny = rows;
  view.screenx = cols;
  view.f = 0.0;
  view.b = 3.0;

  matrix_setView3D(&VTM, &view);

  // set up the drawstate
  drawstate_setColor(&ds, &white);

  // initialize the image
  src = image_create(rows, cols);

  for (frames = 0; frames < 30; frames++)
  {
    image_reset(src);

    matrix_setView3D(&VTM, &view);
    matrix_identity(&GTM);

    matrix_rotateY(&GTM, cos(frames * 2 * M_PI / 36.0), sin(frames * 2 * M_PI / 36.0));
    module_draw(sceneRoot, &VTM, &GTM, &ds, NULL, src);

    sprintf(filename, "p7_3D_scene3%04d.ppm", frames);
    image_write(src, filename);
  }

  module_delete(sceneRoot);

  image_free(src);

  system("convert -delay 10 p7_3D_scene3*.ppm ../images/3DScene3.gif");
  system("rm -f p7_3D_scene3*");

  return (0);
}