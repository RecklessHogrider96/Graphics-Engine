/*
  CS5310 - Roshan Bellary
  10-20-2021 (Fall 2021)

  3D Scene 4.
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

  Module *m_body1;
  Module *m_body2;
  Module *m_body3;
  Module *m_lights1;
  Module *m_lights2;
  Module *m_lights3;
  Module *m_chassis;
  Module *m_cylinder;
  Module *bus1;
  Module *bus2;
  Module *bus3;

  View3D view;
  Matrix vtm, gtm1, gtm2, gtm3;
  DrawState *ds;
  Color DarkGrey = {{0.2, 0.2, 0.2}};
  Color Grey = {{0.3, 0.3, 0.3}};
  Color White = {{1.0, 1.0, 1.0}};
  Color Flame = {{1.0, 0.7, 0.2}};
  Color Red = {{1.0, 0.2, 0.1}};
  Color Green = {{0.3, 1.0, 0.1}};
  Color Blue = {{0.1, 0.1, 0.9}};
  Color Purple = {{0.5, 0.0, 0.5}};
  float bodyWidth = 15.0;
  float bodyHeight = 1.0;
  float bodyDepth = 8.0;
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

  printf("Final VTM: \n");
  matrix_print(&vtm, stdout);

  matrix_identity(&gtm1);
  matrix_identity(&gtm2);
  matrix_identity(&gtm3);

  // Tie wing

  // Wheels
  m_cylinder = module_create();

  module_identity(m_cylinder);
  module_scale(m_cylinder, 4, 4, 4);
  module_rotateX(m_cylinder, cos(1.5), sin(1.5));
  module_translate(m_cylinder, 0, 0, 0);
  module_color(m_cylinder, &Grey);
  module_cylinder(m_cylinder, 20);

  module_identity(m_cylinder);
  module_scale(m_cylinder, 4, 4, 4);
  module_rotateX(m_cylinder, cos(1.5), sin(1.5));
  module_translate(m_cylinder, 0, 0, 15);
  module_color(m_cylinder, &Grey);
  module_cylinder(m_cylinder, 20);

  module_identity(m_cylinder);
  module_scale(m_cylinder, 4, 4, 4);
  module_rotateX(m_cylinder, cos(1.5), sin(1.5));
  module_translate(m_cylinder, 15, 0, 15);
  module_color(m_cylinder, &Grey);
  module_cylinder(m_cylinder, 20);

  module_identity(m_cylinder);
  module_scale(m_cylinder, 4, 4, 4);
  module_rotateX(m_cylinder, cos(1.5), sin(1.5));
  module_translate(m_cylinder, 15, 0, 0);
  module_color(m_cylinder, &Grey);
  module_cylinder(m_cylinder, 20);

  // Chassis
  m_chassis = module_create();

  module_module(m_chassis, m_cylinder);

  module_scale(m_chassis, bodyWidth, bodyHeight, bodyDepth);
  module_translate(m_chassis, 7.5, 0, 8.5);
  module_color(m_chassis, &DarkGrey);
  module_cube(m_chassis, 1);

  // Body
  m_body1 = module_create();

  module_module(m_body1, m_chassis);

  module_scale(m_body1, bodyWidth, 6, bodyDepth);
  module_translate(m_body1, 7.5, 7, 8.5);
  module_color(m_body1, &Blue);
  module_cube(m_body1, 1);

  // Body
  m_body2 = module_create();

  module_module(m_body2, m_chassis);

  module_scale(m_body2, bodyWidth, 6, bodyDepth);
  module_translate(m_body2, 7.5, 7, 8.5);
  module_color(m_body2, &Green);
  module_cube(m_body2, 1);

  // Body
  m_body3 = module_create();

  module_module(m_body3, m_chassis);

  module_scale(m_body3, bodyWidth, 6, bodyDepth);
  module_translate(m_body3, 7.5, 7, 8.5);
  module_color(m_body3, &Purple);
  module_cube(m_body3, 1);

  // Tail and Head Lights

  m_lights1 = module_create();

  module_module(m_lights1, m_body1);

  module_identity(m_lights1);
  // Tail
  module_scale(m_lights1, 1, 1, 1);
  module_rotateZ(m_lights1, cos(1.5), sin(1.5));
  module_translate(m_lights1, -7, 0, 5);
  module_color(m_lights1, &Red);
  module_cylinder(m_lights1, 5);

  module_identity(m_lights1);
  // Tail 2
  module_scale(m_lights1, 1, 1, 1);
  module_rotateZ(m_lights1, cos(1.5), sin(1.5));
  module_translate(m_lights1, -7, 0, 13);
  module_color(m_lights1, &Red);
  module_cylinder(m_lights1, 5);

  module_identity(m_lights1);
  // Head
  module_scale(m_lights1, 1, 1, 1);
  module_rotateZ(m_lights1, cos(1.5), sin(1.5));
  module_translate(m_lights1, 23, 0, 5);
  module_color(m_lights1, &Flame);
  module_cylinder(m_lights1, 5);

  module_identity(m_lights1);
  // Head 2
  module_scale(m_lights1, 1, 1, 1);
  module_rotateZ(m_lights1, cos(1.5), sin(1.5));
  module_translate(m_lights1, 23, 0, 13);
  module_color(m_lights1, &Flame);
  module_cylinder(m_lights1, 5);

  // Bus
  bus1 = module_create();
  module_module(bus1, m_lights1);

  // Tail and Head Lights

  m_lights2 = module_create();

  module_module(m_lights2, m_body2);

  module_identity(m_lights2);
  // Tail
  module_scale(m_lights2, 1, 1, 1);
  module_rotateZ(m_lights2, cos(1.5), sin(1.5));
  module_translate(m_lights2, -7, 0, 5);
  module_color(m_lights2, &Red);
  module_cylinder(m_lights2, 5);

  module_identity(m_lights2);
  // Tail 2
  module_scale(m_lights2, 1, 1, 1);
  module_rotateZ(m_lights2, cos(1.5), sin(1.5));
  module_translate(m_lights2, -7, 0, 13);
  module_color(m_lights2, &Red);
  module_cylinder(m_lights2, 5);

  module_identity(m_lights2);
  // Head
  module_scale(m_lights2, 1, 1, 1);
  module_rotateZ(m_lights2, cos(1.5), sin(1.5));
  module_translate(m_lights2, 23, 0, 5);
  module_color(m_lights2, &Flame);
  module_cylinder(m_lights2, 5);

  module_identity(m_lights2);
  // Head 2
  module_scale(m_lights2, 1, 1, 1);
  module_rotateZ(m_lights2, cos(1.5), sin(1.5));
  module_translate(m_lights2, 23, 0, 13);
  module_color(m_lights2, &Flame);
  module_cylinder(m_lights2, 5);

  // Bus
  bus2 = module_create();
  module_module(bus2, m_lights2);

  // Tail and Head Lights

  m_lights3 = module_create();

  module_module(m_lights3, m_body3);

  module_identity(m_lights3);
  // Tail
  module_scale(m_lights3, 1, 1, 1);
  module_rotateZ(m_lights3, cos(1.5), sin(1.5));
  module_translate(m_lights3, -7, 0, 5);
  module_color(m_lights3, &Red);
  module_cylinder(m_lights3, 5);

  module_identity(m_lights3);
  // Tail 2
  module_scale(m_lights3, 1, 1, 1);
  module_rotateZ(m_lights3, cos(1.5), sin(1.5));
  module_translate(m_lights3, -7, 0, 13);
  module_color(m_lights3, &Red);
  module_cylinder(m_lights3, 5);

  module_identity(m_lights3);
  // Head
  module_scale(m_lights3, 1, 1, 1);
  module_rotateZ(m_lights3, cos(1.5), sin(1.5));
  module_translate(m_lights3, 23, 0, 5);
  module_color(m_lights3, &Flame);
  module_cylinder(m_lights3, 5);

  module_identity(m_lights3);
  // Head 2
  module_scale(m_lights3, 1, 1, 1);
  module_rotateZ(m_lights3, cos(1.5), sin(1.5));
  module_translate(m_lights3, 23, 0, 13);
  module_color(m_lights3, &Flame);
  module_cylinder(m_lights3, 5);

  // Bus
  bus3 = module_create();
  module_module(bus3, m_lights3);

  // Create the image and drawstate
  src = image_create(360, 640);

  ds = drawstate_create();
  ds->shade = ShadeFrame;

  matrix_setView3D(&vtm, &view);
  matrix_identity(&gtm1);
  matrix_identity(&gtm2);
  matrix_identity(&gtm3);

  for (int i = 0; i < frames; i++)
  {
    image_reset(src);

    // scene with three ships (body module)
    scene1 = module_create();
    module_scale(scene1, 1, 1, 1);
    module_translate(scene1, -15 + i, 0, -10);
    module_module(scene1, bus1);

    scene2 = module_create();
    module_scale(scene2, 1, 1, 1);
    module_rotateY(scene2, cos(1.5), sin(1.5));
    module_translate(scene2, 15 - i, 0, -10 + 1);
    module_module(scene2, bus2);

    scene3 = module_create();
    module_scale(scene3, 1, 1, 1);
    module_rotateY(scene3, -cos(1.5), -sin(1.5));
    module_translate(scene3, 55 - i, 0, -10 + i);
    module_module(scene3, bus3);

    matrix_identity(&gtm1);
    matrix_translate(&vtm, 0, i * 0.1, 0);
    matrix_rotateY(&gtm1, cos(30 * 0.01 * M_PI / 36.0), sin(30 * 0.01 * M_PI / 36.0));

    // Different GTM applied to each formation.
    module_draw(scene1, &vtm, &gtm1, ds, NULL, src);
    module_draw(scene2, &vtm, &gtm1, ds, NULL, src);
    module_draw(scene3, &vtm, &gtm1, ds, NULL, src);

    sprintf(filename, "p6_3D_scene4_%04d.ppm", i);
    image_write(src, filename);
  }

  ds->shade = ShadeDepth;
  for (int i = 0; i < frames; i++)
  {
    image_reset(src);

    // scene with three ships (body module)
    scene1 = module_create();
    module_scale(scene1, 1, 1, 1);
    module_translate(scene1, -15 + i, 0, -10);
    module_module(scene1, bus1);

    scene2 = module_create();
    module_scale(scene2, 1, 1, 1);
    module_rotateY(scene2, cos(1.5), sin(1.5));
    module_translate(scene2, 15 - i, 0, -10 + 1);
    module_module(scene2, bus2);

    scene3 = module_create();
    module_scale(scene3, 1, 1, 1);
    module_rotateY(scene3, -cos(1.5), -sin(1.5));
    module_translate(scene3, 55 - i, 0, -10 + i);
    module_module(scene3, bus3);

    matrix_identity(&gtm1);
    matrix_translate(&vtm, 0, i * 0.1, 0);
    matrix_rotateY(&gtm1, cos(30 * 0.01 * M_PI / 36.0), sin(30 * 0.01 * M_PI / 36.0));

    // Different GTM applied to each formation.
    module_draw(scene1, &vtm, &gtm1, ds, NULL, src);
    module_draw(scene2, &vtm, &gtm1, ds, NULL, src);
    module_draw(scene3, &vtm, &gtm1, ds, NULL, src);

    sprintf(filename, "p6_3D_scene4_%04d.ppm", i);
    image_write(src, filename);
  }

  // free the modules
  module_delete(scene1);
  module_delete(scene2);
  module_delete(scene3);

  module_delete(bus1);
  module_delete(bus2);
  module_delete(bus3);

  // free the drawstate
  free(ds);

  // free the image
  image_free(src);

  system("convert -delay 10 p6_3D_scene4_*.ppm ../images/3DScene4.gif");
  system("rm -f p6_3D_scene4_*");

  return (0);
}