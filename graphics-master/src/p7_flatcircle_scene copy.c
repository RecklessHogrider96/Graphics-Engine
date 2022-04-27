/*
  CS5310 - Roshan Bellary
  11-10-2021 (Fall 2021)

  3D View - Circles
*/

#include <stdlib.h>
#include <math.h>

#include "module.h"
#include "view3D.h"

int main(int argc, char *argv[])
{
  Color white;
  Color red;
  Color black;
  Color dark;
  Color blue;

  Module *sceneRoot;
  Module *sphere;
  Module *sphere2;

  int rows = 600;
  int cols = 600;
  int frames = 50;
  Image *src;
  char filename[100];

  color_set(&white, 0.0, 1.0, 1.0);
  color_set(&red, 0.6, 0.7, 1.0);
  color_set(&black, 0.0, 0.0, 0.0);
  color_set(&dark, 0.2, 0.2, 1.0);
  color_set(&blue, 0.0, 0, 1.0);

  View3D view;
  Matrix VTM, GTM;
  DrawState *ds;

  sphere = module_create();
  module_scale(sphere, 0.2, 0.2, 0.2);
  module_translate(sphere, 0, 1, 0);
  // Frame only
  module_cirle(sphere, 10);

  sphere2 = module_create();
  module_scale(sphere2, 0.4, 0.4, 0.4);
  module_translate(sphere2, 0, 0, 0);
  // Shaded
  module_cirle(sphere2, 10);

  // Add cubes to Scene
  sceneRoot = module_create();
  module_module(sceneRoot, sphere);
  module_module(sceneRoot, sphere2);

  // set the View parameters
  point_set3D(&(view.vrp), 0.0, 2.0, -2.0);
  vector_set(&(view.vpn), 0.0, -2.0, 2.0);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.0;
  view.dv = 1.0;
  view.f = 0.0;
  view.b = 50;
  view.screenx = cols;
  view.screeny = rows;

  matrix_setView3D(&VTM, &view);

  // initialize the image
  src = image_create(rows, cols);

  for (frames = 0; frames < 30; frames++)
  {
    image_reset(src);

    matrix_setView3D(&VTM, &view);
    matrix_identity(&GTM);

    ds = drawstate_create();
    ds->color = blue;
    ds->shade = ShadeFrame;

    matrix_rotateY(&GTM, cos(frames * 2 * M_PI / 36.0), sin(frames * 2 * M_PI / 36.0));
    module_draw(sceneRoot, &VTM, &GTM, ds, NULL, src);

    sprintf(filename, "p7_flatcircle_scene_%04d.ppm", frames);
    image_write(src, filename);
  }

  module_delete(sphere);
  module_delete(sphere2);
  module_delete(sceneRoot);

  free(ds);
  image_free(src);

  system("convert -delay 10 p7_flatcircle_scene_*.ppm ../images/circle.gif");
  system("rm -f p7_flatcircle_scene_*");

  return (0);
}