#ifndef MODULE_H

#define MODULE_H

#include "graphics.h"
#include "vectors.h"
#include "matrix.h"
#include "drawstate.h"
#include "lighting.h"
#include "bezier.h"

typedef enum
{
    ObjNone,
    ObjLine,
    ObjPoint,
    ObjPolyline,
    ObjPolygon,
    ObjIdentity,
    ObjMatrix,
    ObjColor,
    ObjBodyColor,
    ObjSurfaceColor,
    ObjSurfaceCoeff,
    ObjLight,
    ObjModule,
} ObjectType;

typedef union
{
    Point point;
    Line line;
    Polyline polyline;
    Polygon polygon;
    Matrix matrix;
    Color color;
    float coeff;
    void *module;
} Object;

typedef struct
{
    ObjectType type;
    Object obj;
    void *next;
} Element;

typedef struct
{
    // pointer to the head of the linked list
    Element *head;
    // keep around a pointer of the last object
    Element *tail;
} Module;

// Generic Functions and 2D Module

// Allocate and return an initalized but empty Element
Element *element_create(void);

// Allocate an element and store a duplicate of the data pointed to by obj in the Element.
// Modules do not get duplicated. The function needs to handle each type of object separately in case statement.
Element *element_init(ObjectType type, void *obj);

// Free the element adn the object it containts, as appropriate
void element_delete(Element *e);

// Allocate an empty module
Module *module_create(void);

// Clear the module's list of Elements, freeing memory as appropriate
void module_clear(Module *md);

// Free all the memory associated with a module, including the memory pointed to by md
void module_delete(Module *md);

// Generic insert of an element into the module at the tail of the list
void module_insert(Module *md, Element *e);

// Adds a pointer to the module sub to the tail of the module's list
void module_module(Module *md, Module *sub);

// Adds p to the tail of the module's list
void module_point(Module *md, Point *p);

// Adds p to the tail of the module's list
void module_line(Module *md, Line *p);

// Adds p to the tail of the module's list
void module_polyline(Module *md, Polyline *p);

// Adds p to the tail of the module's list
void module_polygon(Module *md, Polygon *p);

// Object that sets the current transformation to the identity, placed at the tail of the module's list
void module_identity(Module *md);

// Matrix operand to add a translation matrix to the tail of the module's list
void module_translate2D(Module *md, double tx, double ty);

// Matrix operand to add a scale matrix to the tail of the module's list
void module_scale2D(Module *md, double sx, double sy);

// Matrix operand to add a rotation about the Z axis to the tail of the module's list
void module_rotateZ(Module *md, double cth, double sth);

// Matrix operand to add a 2D shear matrix to the tail of the module's list
void module_shear2D(Module *md, double shx, double shy);

// Draw the module into the image using the given view transformation matrix [VTM],
// Lighting, and DrawState by traversing the list of Elements
// (TODO: Temporarily -> Lighting will be an empty structure)
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src);

// 3D functions

// Matrix operand to add a 3D translation to the module
void module_translate(Module *md, double tx, double ty, double tz);

// Matrix operand to add a 3D scale to the Module
void module_scale(Module *md, double sx, double sy, double sz);

// Matrix operand to add a rotation about the X-axis to the Module
void module_rotateX(Module *md, double cth, double sth);

// Matrix operand to add a rotation about the Y-axis to the Module
void module_rotateY(Module *md, double cth, double sth);

// Matrix operand to add a rotation that orients to teh orthonormal axes u, v,w
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);

// Adds a unit cube, axis-aligned and centered on zero to the Module. If solid is zero, add only lines. If solid is non-zero use polygons. Make sure each polygon has surface normals defined for it
void module_cube(Module *md, int solid);

// Shading / Color

// Adds the foreground color value to the tail of the module's list
void module_color(Module *md, Color *c);

// Adds the body color value to the tail of the module’s list
void module_bodyColor(Module *md, Color *c);

// Adds the surface color value to the tail of the module’s list
void module_surfaceColor(Module *md, Color *c);

// Adds the specular coefficient to the tail of the module’s list
void module_surfaceCoeff(Module *md, float *coeff);

// Use the de Casteljau algorithm to subdivide the Bezier curve divisions times, then add the lines connecting the control points to the module. 
// For example, if divisions is 1, the four original Bezier curve control points will be used to generate eight control points and two new Bezier curves. 
// Then the algorithm will add six lines to the module, three for each of the smaller Bezier curves.
void module_bezierCurve(Module *m, BezierCurve *b, int divisions);

// Use the de Casteljau algorithm to subdivide the Bezier surface divisions times, then draw either the
// lines connecting the control points, if solid is 0, or draw triangles using the four corner control points.
// For example, if divisions is 1, the 16 original Bezier curve control points will be used to generate 64
// control points and four new Bezier surfaces, 1 level of subdivision, and then the algorithm will draw lines or triangles to connect adjacent control points.
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid);

// Adds a unit cylinder.
// From Bruce Maxwell's test code.
void module_cylinder(Module *m, int divisions);

// Adds a unit circle.
void module_cirle(Module *m, int divisions);

// Adds a unit cone.
void module_cone(Module *m, int divisions);

// Adds a unit tetrahedron.
void module_tetrahedron(Module *m);

// Adds a unit sphere.
void module_ellipsoid(Module *m, int divisions);

// Adds a unit drop of liquid.
void module_drop(Module *m, int divisions);

#endif