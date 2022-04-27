/*
  CS5310 - Roshan Bellary
  11-10-2021 (Fall 2021)

  3D View - Cylinder
*/

#include <stdlib.h>

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
    Module *cylinder;
    Module *cylinder2;

    int rows = 600;
    int cols = 600;
    Image *src;

    color_set(&white, 0.0, 1.0, 1.0);
    color_set(&red, 0.6, 0.7, 1.0);
    color_set(&black, 0.0, 0.0, 0.0);
    color_set(&dark, 0.2, 0.2, 1.0);
    color_set(&blue, 0.0, 0, 1.0);
    src = image_create(rows, cols);

    View3D view;
    Matrix VTM, GTM;
    DrawState *ds;

    cylinder = module_create();
    module_scale(cylinder, 0.2, 0.2, 0.2);
    module_translate(cylinder, 0, 2, 0);
    module_cylinder(cylinder, 50);

    cylinder2 = module_create();
    module_scale(cylinder2, 0.8, 0.8, 0.8);
    module_translate(cylinder2, 0, 0, 0);
    // Shaded
    module_cylinder(cylinder2, 100);

    // Add cylinders to Scene
    sceneRoot = module_create();
    module_module(sceneRoot, cylinder);
    module_module(sceneRoot, cylinder2);

    // Setup view
    point_set3D(&(view.vrp), 0.0, 2.0, -2.0);
    vector_set(&(view.vpn), 0.0, -1.0, 2.0);
    vector_set(&(view.vup), 0.0, 1.0, 0.0);
    view.d = 1;
    view.du = 1.0;
    view.dv = 1.0 * rows / cols;
    view.screeny = rows;
    view.screenx = cols;
    view.f = 0.0;
    view.b = 3.0;

    matrix_setView3D(&VTM, &view);
    matrix_identity(&GTM);

    ds = drawstate_create();
    ds->color = blue;
    ds->shade = ShadeFrame;

    module_draw(sceneRoot, &VTM, &GTM, ds, NULL, src);

    image_write(src, "p7_cylinder.ppm");

    module_delete(cylinder);
    module_delete(cylinder2);
    module_delete(sceneRoot);

    free(ds);
    image_free(src);

    return (0);
}