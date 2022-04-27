#ifndef DRAWSTATE_H

#define DRAWSTATE_H

#include <graphics.h>

typedef enum
{
    ShadeFrame,    // Draw only the borders of objs, including polygons
    ShadeConstant, // Draw objs using current foreground color, fill polygons
    ShadeDepth,    // Draw objs usign their depth val
    ShadeFlat,     // Draw obj using shading calculations
    ShadeGouraud,  // Draw objs using gouraud shading
    ShadePhong,    // Draw objects using phongs shading
} ShadeMethod;

typedef struct
{
    Color color;        // foreground color
    Color flatColor;    // flat-fill a polygon based on shading calculation
    Color body;         // body reflection color
    Color surface;      // surface reflection color
    float surfaceCoeff; // float that represents the shine of the surface
    ShadeMethod shade;
    int zBufferflag;    // whether to use z-buffer hidden surface removal
    Point viewer;       // a point representing the view location in 3D (identical to VRP in View3D)
    float scaleFactor;
} DrawState;

// Create a new DrawState structure and initalize the fields
DrawState *drawstate_create(void);

// Set the color field to c
void drawstate_setColor(DrawState *s, Color *c);

// Set the body field to c
void drawstate_setBody(DrawState *s, Color *c);

// Set the surface field to c
void drawstate_setSurface(DrawState *s, Color *c);

// Set the surface coeff field to f
void drawstate_setSurfaceCoeff(DrawState *s, float f);

// Copy the DrawState data
void drawstate_copy(DrawState *to, DrawState *from);

#endif