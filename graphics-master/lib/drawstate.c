/*
  CS5310 - Roshan Bellary
  10-26-2021 (Fall 2021)

  Drawstate Functions.
*/

#include <stdlib.h>

#include "drawstate.h"

// Create a new DrawState structure and initalize the fields
DrawState *drawstate_create(void)
{
    DrawState *state = malloc(sizeof(DrawState));

    Color white;
    Color grey;
    color_set(&white, 1.0, 1.0, 1.0);
    color_set(&grey, .2, .2, .2);

    state->color = white;
    state->flatColor = white;
    state->body = white;
    state->surface = grey;
    state->surfaceCoeff = 0;
    state->zBufferflag = 1;
    state->scaleFactor = 1;

    return state;
}

// Set the color field to c
void drawstate_setColor(DrawState *s, Color *c)
{
    s->color = *c;
}

// Set the body field to c
void drawstate_setBody(DrawState *s, Color *c)
{
    s->body = *c;
}

// Set the surface field to c
void drawstate_setSurface(DrawState *s, Color *c)
{
    s->surface = *c;
}

// Set the surface coeff field to f
void drawstate_setSurfaceCoeff(DrawState *s, float f)
{
    s->surfaceCoeff = f;
}

// Set the scale factor field to f
void drawstate_setScaleFactor(DrawState *s, float f)
{
    s->scaleFactor = f;
}

// Copy the DrawState data
void drawstate_copy(DrawState *to, DrawState *from)
{
    *to = *from;
}