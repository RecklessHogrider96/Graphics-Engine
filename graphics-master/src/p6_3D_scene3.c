/*
  CS5310 - Roshan Bellary
  10-20-2021 (Fall 2021)

  3D Scene 3.
*/

#include <stdlib.h>
#include <math.h>

#include "vectors.h"
#include "matrix.h"
#include "view3D.h"
#include "module.h"

/*
  Adds to the module a unit cylinder, aligned along the Y-axis

  Takes in the number of subdivisions to use when creating the cylinder
 */
void cylinder(Module *mod, int sides);
void cylinder(Module *mod, int sides)
{
  Polygon p;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init(&p);
  point_set3D(&xtop, 0, 1.0, 0.0);
  point_set3D(&xbot, 0, 0.0, 0.0);

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for (i = 0; i < sides; i++)
  {
    Point pt[4];

    x1 = cos(i * M_PI * 2.0 / sides);
    z1 = sin(i * M_PI * 2.0 / sides);
    x2 = cos(((i + 1) % sides) * M_PI * 2.0 / sides);
    z2 = sin(((i + 1) % sides) * M_PI * 2.0 / sides);

    point_copy(&pt[0], &xtop);
    point_set3D(&pt[1], x1, 1.0, z1);
    point_set3D(&pt[2], x2, 1.0, z2);

    polygon_set(&p, 3, pt);
    module_polygon(mod, &p);

    point_copy(&pt[0], &xbot);
    point_set3D(&pt[1], x1, 0.0, z1);
    point_set3D(&pt[2], x2, 0.0, z2);

    polygon_set(&p, 3, pt);
    module_polygon(mod, &p);

    point_set3D(&pt[0], x1, 0.0, z1);
    point_set3D(&pt[1], x2, 0.0, z2);
    point_set3D(&pt[2], x2, 1.0, z2);
    point_set3D(&pt[3], x1, 1.0, z1);

    polygon_set(&p, 4, pt);
    module_polygon(mod, &p);
  }

  polygon_clear(&p);
}

// adds a cube centered on the origin of width 2x2x2 to the module
void cube(Module *mod);
void cube(Module *mod)
{
  Point pt[4];
  Polygon p;

  polygon_init(&p);
  point_set3D(&pt[0], -1, -1, -1);
  point_set3D(&pt[1], -1, -1, 1);
  point_set3D(&pt[2], -1, 1, 1);
  point_set3D(&pt[3], -1, 1, -1);
  polygon_set(&p, 4, pt);
  module_polygon(mod, &p);

  point_set3D(&pt[0], 1, -1, -1);
  point_set3D(&pt[1], 1, -1, 1);
  point_set3D(&pt[2], 1, 1, 1);
  point_set3D(&pt[3], 1, 1, -1);
  polygon_set(&p, 4, pt);
  module_polygon(mod, &p);

  point_set3D(&pt[0], -1, -1, -1);
  point_set3D(&pt[1], -1, -1, 1);
  point_set3D(&pt[2], 1, -1, 1);
  point_set3D(&pt[3], 1, -1, -1);
  polygon_set(&p, 4, pt);
  module_polygon(mod, &p);

  point_set3D(&pt[0], -1, 1, -1);
  point_set3D(&pt[1], -1, 1, 1);
  point_set3D(&pt[2], 1, 1, 1);
  point_set3D(&pt[3], 1, 1, -1);
  polygon_set(&p, 4, pt);
  module_polygon(mod, &p);

  point_set3D(&pt[0], -1, -1, -1);
  point_set3D(&pt[1], -1, 1, -1);
  point_set3D(&pt[2], 1, 1, -1);
  point_set3D(&pt[3], 1, -1, -1);
  polygon_set(&p, 4, pt);
  module_polygon(mod, &p);

  point_set3D(&pt[0], -1, -1, 1);
  point_set3D(&pt[1], -1, 1, 1);
  point_set3D(&pt[2], 1, 1, 1);
  point_set3D(&pt[3], 1, -1, 1);
  polygon_set(&p, 4, pt);
  module_polygon(mod, &p);
}

int main(int argc, char *argv[])
{
  Image *src;
  Module *scene1;
  Module *scene2;
  Module *scene3;
  Module *twing;
  Module *twings;
  Module *tieFighter;
  Module *flameTieFighter;
  Module *greenTieFighter;
  Polygon p;
  Point pt[4];
  View3D view;
  Matrix vtm, gtm1, gtm2, gtm3;
  DrawState *ds;
  Color Flame = {{1.0, 0.7, 0.2}};
  Color Red = {{1.0, 0.2, 0.1}};
  Color Green = {{0.3, 1.0, 0.1}};
  float bodyWidth = 2.0;
  char filename[100];

  int frames = 50;

  // set up the view
  point_set3D(&(view.vrp), 5, 30, 120);
  vector_set(&(view.vpn), 0, -30, -100);
  vector_set(&(view.vup), 0, 1, 0);
  view.d = 1.8;
  view.du = 1.6;
  view.dv = 0.9;
  view.f = 1;
  view.b = 50;
  view.screenx = 640;
  view.screeny = 360;

  matrix_setView3D(&vtm, &view);
  matrix_identity(&gtm1);
  matrix_identity(&gtm2);
  matrix_identity(&gtm3);

  // Tie wing
  twing = module_create();
  polygon_init(&p);
  point_set3D(&pt[0], 0.0, 0.0, 0.0);
  point_set3D(&pt[1], 0.0, 0.0, 5.0);
  point_set3D(&pt[2], 15.0, 0.0, 3.0);
  point_set3D(&pt[3], 15.0, 0.0, 0.0);
  polygon_set(&p, 4, pt);
  module_polygon(twing, &p);
  module_translate(twing, 0, 0.5, 0);
  module_polygon(twing, &p);

  module_identity(twing);
  module_translate(twing, 3, 1.6, -1);

  module_identity(twing);
  point_set3D(&pt[0], 15.0, 0.0, 3.0);
  point_set3D(&pt[1], 15.0, 0.0, 0.0);
  point_set3D(&pt[2], 15.0, 0.5, 0.0);
  point_set3D(&pt[3], 15.0, 0.5, 3.0);
  polygon_set(&p, 4, pt);
  module_polygon(twing, &p);

  point_set3D(&pt[0], 15.0, 0.0, 0.0);
  point_set3D(&pt[1], 0.0, 0.0, 0.0);
  point_set3D(&pt[2], 0.0, 0.5, 0.0);
  point_set3D(&pt[3], 15.0, 0.5, 0.0);
  polygon_set(&p, 4, pt);
  module_polygon(twing, &p);

  point_set3D(&pt[0], 15.0, 0.0, 3.0);
  point_set3D(&pt[1], 15.0, 0.5, 3.0);
  point_set3D(&pt[2], 0.0, 0.5, 5.0);
  point_set3D(&pt[3], 0.0, 0.0, 5.0);
  polygon_set(&p, 4, pt);
  module_polygon(twing, &p);

  module_translate(twing, 15, 0.25, 0);

  // 4 Tie wings
  twings = module_create();
  module_rotateZ(twings, cos(0.3), sin(0.3));
  module_translate(twings, bodyWidth, 0, 0);
  module_module(twings, twing);

  module_identity(twings);
  module_scale(twings, 1, -1, 1);
  module_rotateZ(twings, cos(-0.3), sin(-0.3));
  module_translate(twings, bodyWidth, 0, 0);
  module_module(twings, twing);

  module_identity(twings);
  module_scale(twings, -1, 1, 1);
  module_rotateZ(twings, cos(-0.3), sin(-0.3));
  module_translate(twings, -bodyWidth, 0, 0);
  module_module(twings, twing);

  module_identity(twings);
  module_scale(twings, -1, -1, 1);
  module_rotateZ(twings, cos(0.3), sin(0.3));
  module_translate(twings, -bodyWidth, 0, 0);
  module_module(twings, twing);

  // Tie Fighter
  tieFighter = module_create();

  module_module(tieFighter, twings);

  module_scale(tieFighter, bodyWidth, bodyWidth, 8);
  module_translate(tieFighter, 0, 0, 3);
  cylinder(tieFighter, 10);
  module_scale(tieFighter, .8, .8, 1);
  module_color(tieFighter, &Red);
  cylinder(tieFighter, 10);

  // Flame Tie Fighter
  flameTieFighter = module_create();

  module_module(flameTieFighter, twings);

  module_scale(flameTieFighter, bodyWidth, bodyWidth, 8);
  module_translate(flameTieFighter, 0, 0, 3);
  cylinder(flameTieFighter, 10);
  module_scale(flameTieFighter, .8, .8, 1);
  module_color(flameTieFighter, &Flame);
  cylinder(flameTieFighter, 10);

  // Green Tie Fighter
  greenTieFighter = module_create();

  module_module(greenTieFighter, twings);

  module_scale(greenTieFighter, bodyWidth, bodyWidth, 8);
  module_translate(greenTieFighter, 0, 0, 3);
  cylinder(greenTieFighter, 10);
  module_scale(greenTieFighter, .8, .8, 1);
  module_color(greenTieFighter, &Green);
  cylinder(greenTieFighter, 10);

  // scene with three ships (body module)
  scene1 = module_create();
  module_scale(scene1, 0.7, 0.7, 0.7);
  module_rotateZ(scene1, cos(0.5), sin(0.5));
  module_rotateY(scene1, cos(-0.1), sin(-0.1));
  module_rotateX(scene1, cos(0.2), sin(0.2));
  module_translate(scene1, -15, -10, -10);
  module_module(scene1, tieFighter);

  module_identity(scene1);
  module_scale(scene1, 0.7, 0.7, 0.7);
  module_rotateZ(scene1, cos(0.3), sin(0.3));
  module_rotateY(scene1, cos(-0.05), sin(-0.05));
  module_rotateX(scene1, cos(0.2), sin(0.2));
  module_translate(scene1, 5, -1, -50);
  module_module(scene1, tieFighter);
  
  module_identity(scene1);
  module_scale(scene1, 0.7, 0.7, 0.7);
  module_rotateZ(scene1, cos(0.1), sin(0.1));
  module_rotateX(scene1, cos(0.2), sin(0.2));
  module_translate(scene1, -35, -3, -60);
  module_module(scene1, tieFighter);

  // Formation 2
  scene2 = module_create();
  module_scale(scene2, 0.5, 0.5, 0.5);
  module_rotateZ(scene2, cos(0.2), sin(0.2));
  module_rotateX(scene2, cos(0.5), sin(0.5));
  module_translate(scene2, -45, 5, -60);
  module_module(scene2, flameTieFighter);

  module_identity(scene2);
  module_scale(scene2, 0.5, 0.5, 0.5);
  module_rotateZ(scene2, cos(0.1), sin(0.1));
  module_rotateX(scene2, cos(0.2), sin(0.2));
  module_translate(scene2, -25, 5, -60);
  module_module(scene2, flameTieFighter);

  module_identity(scene2);
  module_scale(scene2, 0.7, 0.7, 0.7);
  module_rotateZ(scene2, cos(0.4), sin(0.4));
  module_rotateX(scene2, cos(0.5), sin(0.5));
  module_translate(scene2, -5, 5, -60);
  module_module(scene2, flameTieFighter);

  // Formation 3
  scene3 = module_create();
  module_scale(scene3, 0.5, 0.5, 0.5);
  module_rotateZ(scene3, cos(0.2), sin(0.2));
  module_rotateX(scene3, cos(0.5), sin(0.5));
  module_translate(scene3, 5, -15, -10);
  module_module(scene3, greenTieFighter);

  module_identity(scene3);
  module_scale(scene3, 0.5, 0.5, 0.5);
  module_rotateZ(scene3, cos(0.1), sin(0.1));
  module_rotateX(scene3, cos(0.2), sin(0.2));
  module_translate(scene3, 25, 5, -10);
  module_module(scene3, greenTieFighter);

  module_identity(scene3);
  module_scale(scene3, 0.7, 0.7, 0.7);
  module_rotateZ(scene3, cos(0.4), sin(0.4));
  module_rotateX(scene3, cos(0.5), sin(0.5));
  module_translate(scene3, 45, 5, -10);
  module_module(scene3, greenTieFighter);

  // create the image and drawstate
  src = image_create(360, 640);
  ds = drawstate_create();
  ds->shade = ShadeFrame;

  for (int i = 0; i < frames; i++)
  {
    image_reset(src);

    matrix_setView3D(&vtm, &view);
    matrix_identity(&gtm1);
    matrix_identity(&gtm2);
    matrix_identity(&gtm3);

    matrix_rotateY(&gtm1, cos(i * 2 * M_PI / 36.0), sin(i * 2 * M_PI / 36.0));

    matrix_rotateY(&gtm2, sin(i * 2 * M_PI / 36.0), cos(i * 1 * M_PI / 36.0));

    matrix_rotateY(&gtm3, cos(i * 1 * M_PI / 36.0), sin(i * 2 * M_PI / 36.0));

    // Different GTM applied to each formation.
    module_draw(scene1, &vtm, &gtm1, ds, NULL, src);
    module_draw(scene2, &vtm, &gtm2, ds, NULL, src);
    module_draw(scene3, &vtm, &gtm3, ds, NULL, src);

    sprintf(filename, "p6_3D_scene3_%04d.ppm", i);
    image_write(src, filename);
  }

  // free the polygon data
  polygon_clear(&p);

  // free the modules
  module_delete(scene1);
  module_delete(scene2);
  module_delete(scene3);
  module_delete(twing);
  module_delete(twings);
  module_delete(tieFighter);

  // free the drawstate
  free(ds);

  // free the image
  image_free(src);

  system("convert -delay 10 p6_3D_scene3_*.ppm ../images/3DScene3.gif");
  system("rm -f p6_3D_scene3_*");

  return (0);
}