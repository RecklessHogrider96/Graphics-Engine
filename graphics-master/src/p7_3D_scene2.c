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
  Module *pole;
  Point p[16];
  Point tp[4];

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

  // Cylinder
  //module_scale(sceneRoot, 0.02, 0.01, 0.02);
  module_translate(sceneRoot, 0, 0, 0);
  module_color(sceneRoot, &greyer);
  module_cylinder(sceneRoot, 50);

  // Cylinder Pole
  pole = module_create();
  module_scale(pole, 0.1, 3, 0.1);
  module_translate(sceneRoot, 0, 1, 0);
  module_color(sceneRoot, &grey);
  module_cylinder(pole, 50);
  module_module(sceneRoot, pole);

  // create a flat plane
  point_set3D(&p[0], 0, 2.0, -0.2); // first row	 constant x	 even spacing in z
  point_set3D(&p[1], 0.33, 2.0, 0.2);
  point_set3D(&p[2], 0.66, 2.0, -0.2);
  point_set3D(&p[3], 1, 2.0, 0.2);
  point_set3D(&p[4], 0, 2.3, -0.2); // second row
  point_set3D(&p[5], 0.33, 2.3, 0.2);
  point_set3D(&p[6], 0.66, 2.3, -0.2);
  point_set3D(&p[7], 1, 2.3, 0.2);
  point_set3D(&p[8], 0, 2.7, -0.2); // third row
  point_set3D(&p[9], 0.33, 2.7, 0.2);
  point_set3D(&p[10], 0.66, 2.7, -0.2);
  point_set3D(&p[11], 1, 2.7, 0.2);
  point_set3D(&p[12], 0, 3.0, -0.2); // fourth row
  point_set3D(&p[13], 0.33, 3.0, 0.2);
  point_set3D(&p[14], 0.66, 3.0, -0.2);
  point_set3D(&p[15], 1, 3.0, 0.2);

  bezierSurface_set(&bc, p);

  // put the curve into a module
  module_color(sceneRoot, &blue);
  module_bezierSurface(sceneRoot, &bc, divisions, 0);

  // create a surface
  point_copy(&tp[0], &p[0]); // first row, constant x, even spacing in z
  point_copy(&tp[1], &p[4]);
  point_copy(&tp[2], &p[8]); // fourth row
  point_copy(&tp[3], &p[12]);

  bezierSurface_set(&bc, tp);

  // put the curve into a module
  module_color(sceneRoot, &green);
  //module_bezierSurface(sceneRoot, &bc, divisions, 0);

  // Tetrahedron
  //module_scale(sceneRoot, 0.2, 0.2, 0.2);
  module_translate(sceneRoot, 0, 1, 0);
  module_color(sceneRoot, &red);
  //module_tetrahedron(sceneRoot);

  // Cone
  //module_scale(sceneRoot, 0.4, 1.0, 0.4);
  module_translate(sceneRoot, 0, 0.5, 0);
  module_color(sceneRoot, &blue);
  //module_cone(sceneRoot, 100);

  // set the View parameters
  point_set3D(&(view.vrp), 0.0, 10.0, -10.0);
  vector_set(&(view.vpn), 0.0, -2.5, 3.5);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.0;
  view.dv = 1.0;
  view.f = 1.0;
  view.b = 50;
  view.screenx = cols;
  view.screeny = rows;

  matrix_setView3D(&VTM, &view);

  // set up the drawstate
  drawstate_setColor(&ds, &white);
  ds.shade = ShadeConstant;

  // initialize the image
  src = image_create(rows, cols);

  for (frames = 0; frames < 30; frames++)
  {
    image_reset(src);

    matrix_setView3D(&VTM, &view);
    matrix_identity(&GTM);

    matrix_rotateY(&GTM, cos(frames * 2 * M_PI / 36.0), sin(frames * 2 * M_PI / 36.0));
    module_draw(sceneRoot, &VTM, &GTM, &ds, NULL, src);

    sprintf(filename, "p7_3D_scene2%04d.ppm", frames);
    image_write(src, filename);
  }

  module_delete(sceneRoot);

  image_free(src);

  system("convert -delay 10 p7_3D_scene2*.ppm ../images/3DScene2.gif");
  system("rm -f p7_3D_scene2*");

  return (0);
}