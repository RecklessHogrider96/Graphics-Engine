/*
  CS5310 - Roshan Bellary
  10-20-2021 (Fall 2021)

  3D View - Cube
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
    Module *cube;
    Module *cube2;

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

    cube = module_create();
    module_scale(cube, 0.2, 0.2, 0.2);
    module_translate(cube, 0, 0, 0);
    // Frame only
    module_cube(cube, 0);

    cube2 = module_create();
    module_scale(cube2, 0.4, 0.4, 0.4);
    module_translate(cube2, 0, 2, 0);
    // Shaded
    module_cube(cube2, 1);

    // Add cubes to Scene
    sceneRoot = module_create();
    module_module(sceneRoot, cube);
    module_module(sceneRoot, cube2);

    // Setup view
    point_set3D(&(view.vrp), 0.0, 1.0, -3.0 );
    vector_set( &(view.vpn), 0.0, 0.0, 1.0 );
    vector_set( &(view.vup), 0.0, 1.0, 0.0 );
    view.d = 1;
    view.du = 1.0;
    view.dv = 1.0*rows/cols;
    view.screeny = rows;
    view.screenx = cols;
    view.f = 0.0;
    view.b = 3.0;

    matrix_setView3D( &VTM, &view );
    matrix_identity( &GTM );

    ds = drawstate_create();
    ds->color = blue;
    ds->shade = ShadeFrame; 
    
    module_draw(sceneRoot, &VTM, &GTM, ds, NULL, src);

    image_write( src, "p6_cube.ppm" );

    module_delete(cube);
    module_delete(cube2);
    module_delete(sceneRoot);

    free(ds);
    image_free(src);

    return (0);
}