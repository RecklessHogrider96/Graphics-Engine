/*
  CS5310 - Roshan Bellary
  10-26-2021 (Fall 2021)

  Module Functions.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "module.h"
#include "bezier.h"

// Allocate and return an initialized but empty Element.
Element *element_create()
{
    Element *e = malloc(sizeof(Element));
    e->next = NULL;

    return e;
}

// Allocate an element and store a duplicate of the data pointed to by obj in the Element.
// Modules do not get duplicated. The function needs to handle each type of object separately in case statement.
Element *element_init(ObjectType type, void *obj)
{
    Element *e = malloc(sizeof(Element));
    e->type = type;
    e->next = NULL;

    switch (type)
    {
    case ObjNone:
        break;
    case ObjLine:
        line_copy(&e->obj.line, (Line *)obj);
        break;
    case ObjPoint:
        point_copy(&e->obj.point, (Point *)obj);
        break;
    case ObjPolyline:
        polyline_init(&e->obj.polyline);
        polyline_copy(&e->obj.polyline, (Polyline *)obj);
        break;
    case ObjPolygon:
        polygon_init(&e->obj.polygon);
        polygon_copy(&e->obj.polygon, (Polygon *)obj);
        break;
    case ObjIdentity:
        break;
    case ObjMatrix:
        matrix_copy(&e->obj.matrix, (Matrix *)obj);
        break;
    case ObjColor:
    case ObjBodyColor:
    case ObjSurfaceColor:
        color_copy(&e->obj.color, (Color *)obj);
        break;
    case ObjSurfaceCoeff:
        e->obj.coeff = *(float *)obj;
        break;
    case ObjLight:
        break;
    case ObjModule:
        e->obj.module = (Module *)obj;
        break;
    default:
        e->type = ObjNone;
        break;
    }
    return e;
}

// Free the element adn the object it containts, as appropriate
void element_delete(Element *e)
{
    switch (e->type)
    {
    case ObjNone:
        break;
    case ObjLine:
        break;
    case ObjPoint:
        break;
    case ObjPolyline:
        polyline_clear(&(e->obj.polyline));
        break;
    case ObjPolygon:
        polygon_clear(&(e->obj.polygon));
        break;
    case ObjIdentity:
        break;
    case ObjMatrix:
        break;
    case ObjColor:
    case ObjBodyColor:
    case ObjSurfaceColor:
        break;
    case ObjSurfaceCoeff:
        break;
    case ObjLight:
        break;
    case ObjModule:
        break;
    }

    if (e)
        free(e);
}

// Allocate an empty module
Module *module_create(void)
{
    Module *m = malloc(sizeof(Module));

    m->head = NULL;
    m->tail = NULL;

    return m;
}

// Clear the module's list of Elements, freeing memory as appropriate
void module_clear(Module *md)
{
    if (md->head == NULL)
        return;

    Element *cur = md->head;

    while (cur->next != NULL)
    {
        Element *temp = (Element *)cur->next;
        element_delete(cur);
        cur = temp;
    }

    md->head = NULL;
    md->tail = NULL;
}

// Free all the memory associated with a module, including the memory pointed to by md
void module_delete(Module *md)
{
    module_clear(md);
    free(md);
}

// Generic insert of an element into the module at the tail of the list
void module_insert(Module *md, Element *e)
{
    if (md->head == NULL)
    {
        md->head = e;
        md->tail = e;
    }
    else
    {
        md->tail->next = (void *)e;
        md->tail = e;
    }
}

// Adds a pointer to the module sub to the tail of the module's list
void module_module(Module *md, Module *sub)
{
    Element *new = element_init(ObjModule, (void *)sub);

    // Insert to tail module
    module_insert(md, new);
}

// Adds p to the tail of the module's list
void module_point(Module *md, Point *p)
{
    Element *new = element_init(ObjPoint, (void *)p);

    // Insert to tail module
    module_insert(md, new);
}

// Adds p to the tail of the module's list
void module_line(Module *md, Line *l)
{
    Element *new = element_init(ObjLine, (void *)l);

    // Insert to tail module
    module_insert(md, new);
}

// Adds p to the tail of the module's list
void module_polyline(Module *md, Polyline *p)
{
    Element *new = element_init(ObjPolyline, (void *)p);

    // Insert to tail module
    module_insert(md, new);
}

// Adds p to the tail of the module's list
void module_polygon(Module *md, Polygon *p)
{
    Element *new = element_init(ObjPolygon, (void *)p);

    // Insert to tail module
    module_insert(md, new);
}

// Object that sets the current transformation to the identity, placed at the tail of the module's list
void module_identity(Module *md)
{
    Element *new = element_init(ObjIdentity, NULL);

    // Insert to tail module
    module_insert(md, new);
}

// Matrix operand to add a translation matrix to the tail of the module's list
void module_translate2D(Module *md, double tx, double ty)
{
    Matrix m;
    matrix_identity(&m);
    matrix_translate2D(&m, tx, ty);

    Element *new = element_init(ObjMatrix, (void *)&m);

    // Insert to tail module
    module_insert(md, new);
}

// Matrix operand to add a scale matrix to the tail of the module's list
void module_scale2D(Module *md, double sx, double sy)
{
    Matrix m;
    matrix_identity(&m);
    matrix_scale2D(&m, sx, sy);

    Element *new = element_init(ObjMatrix, (void *)&m);

    // Insert to tail module
    module_insert(md, new);
}

// Matrix operand to add a rotation about the Z axis to the tail of the module's list
void module_rotateZ(Module *md, double cth, double sth)
{
    Matrix m;
    matrix_identity(&m);
    matrix_rotateZ(&m, cth, sth);

    Element *new = element_init(ObjMatrix, (void *)&m);

    // Insert to tail module
    module_insert(md, new);
}

// Matrix operand to add a 2D shear matrix to the tail of the module's list
void module_shear2D(Module *md, double shx, double shy)
{
    Matrix m;
    matrix_identity(&m);
    matrix_scale2D(&m, shx, shy);

    Element *new = element_init(ObjMatrix, (void *)&m);

    // Insert to tail module
    module_insert(md, new);
}

// Draw the module into the image using the given view transformation matrix [VTM],
// Lighting, and DrawState by traversing the list of Elements
// (TODO: Temporarily -> Lighting will be an empty structure)
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src)
{
    Matrix LTM;
    matrix_identity(&LTM);

    // Start -> head module
    Element *iteratingElement = md->head;

    // Iterate through module
    while (iteratingElement != NULL)
    {
        switch (iteratingElement->type)
        {

        case ObjNone:
            break;

        case ObjLine:
        {
            Line temp;

            temp = iteratingElement->obj.line;
            matrix_xformLine(&LTM, &temp);
            matrix_xformLine(GTM, &temp);
            matrix_xformLine(VTM, &temp);
            line_normalize(&temp);
            line_draw(&temp, src, ds->color);
        }
        break;

        case ObjPoint:
        {
            Point temp;

            matrix_xformPoint(&LTM, &iteratingElement->obj.point, &temp);
            matrix_xformPoint(GTM, &temp, &temp);
            matrix_xformPoint(VTM, &temp, &temp);
            point_normalize(&temp);
            point_draw(&temp, src, ds->color);
        }
        break;

        case ObjPolyline:
        {
            Polyline *temp = polyline_create();

            polyline_copy(temp, &iteratingElement->obj.polyline);
            *temp = iteratingElement->obj.polyline;
            matrix_xformPolyline(&LTM, temp);
            matrix_xformPolyline(GTM, temp);
            matrix_xformPolyline(VTM, temp);
            polyline_normalize(temp);
            polyline_draw(temp, src, ds->color);
            polyline_free(temp);
        }
        break;

        case ObjPolygon:
        {
            Polygon *temp = polygon_create();

            polygon_copy(temp, &iteratingElement->obj.polygon);
            matrix_xformPolygon(&LTM, temp);
            matrix_xformPolygon(GTM, temp);
            matrix_xformPolygon(VTM, temp);
            polygon_normalize(temp);

            if (ds->shade == ShadeFrame)
            {
                polygon_draw(temp, src, ds->color);
            }
            else
            {
                polygon_drawFill(temp, src, ds);
            }

            polygon_clear(temp);
        }
        break;

        case ObjIdentity:
            matrix_identity(&LTM);
            break;

        case ObjMatrix:
            // This is Left Multiply right?. I hope so or else "Bad things will happen." :(
            // Below is -> LTM = obj.matrix * LTM
            matrix_multiply(&iteratingElement->obj.matrix, &LTM, &LTM);
            break;

        case ObjColor:
            ds->color = iteratingElement->obj.color;
            break;

        case ObjBodyColor:
            ds->body = iteratingElement->obj.color;
            break;

        case ObjSurfaceColor:
            ds->surface = iteratingElement->obj.color;
            break;

        case ObjSurfaceCoeff:
            ds->surfaceCoeff = iteratingElement->obj.coeff;
            break;

        case ObjLight:
            break;

        case ObjModule:
        {
            Matrix tGTM;

            DrawState *temp = drawstate_create();
            drawstate_copy(temp, ds);

            // Calculate latest GTM and pass it to the next depth / row.
            // DO NOT CHANGE CURRENT LEVEL GTM.
            // The GTM argument handed to the function should be the product of the GTM and the local LTM.
            // Below is -> tGTM = GTM * LTM
            matrix_multiply(GTM, &LTM, &tGTM);

            // Recursing the tree.
            // Depth + 1
            // Dear god, hope this is right or else "Bad things will happen."
            module_draw((Module *)(iteratingElement->obj.module), VTM, &tGTM, temp, lighting, src);
        }
        break;
        }

        // Update iterating variable (element)
        // Set next iteratingElement to iteratingElement
        iteratingElement = iteratingElement->next;
    }
}

// 3D functions

// Matrix operand to add a 3D translation to the module
void module_translate(Module *md, double tx, double ty, double tz)
{
    Matrix m;
    matrix_identity(&m);
    matrix_translate(&m, tx, ty, tz);

    Element *new = element_init(ObjMatrix, (void *)&m);

    // Insert to tail module
    module_insert(md, new);
}

// Matrix operand to add a 3D scale to the Module
void module_scale(Module *md, double sx, double sy, double sz)
{
    Matrix m;
    matrix_identity(&m);
    matrix_scale(&m, sx, sy, sz);

    Element *new = element_init(ObjMatrix, (void *)&m);

    // Insert to tail module
    module_insert(md, new);
}

// Matrix operand to add a rotation about the X-axis to the Module
void module_rotateX(Module *md, double cth, double sth)
{
    Matrix m;
    matrix_identity(&m);
    matrix_rotateX(&m, cth, sth);

    Element *new = element_init(ObjMatrix, (void *)&m);

    // Insert to tail module
    module_insert(md, new);
}

// Matrix operand to add a rotation about the Y-axis to the Module
void module_rotateY(Module *md, double cth, double sth)
{
    Matrix m;
    matrix_identity(&m);
    matrix_rotateY(&m, cth, sth);

    Element *new = element_init(ObjMatrix, (void *)&m);

    // Insert to tail module
    module_insert(md, new);
}

// Matrix operand to add a rotation that orients to teh orthonormal axes u, v,w
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w)
{
    Matrix m;
    matrix_identity(&m);
    matrix_rotateXYZ(&m, u, v, w);

    Element *new = element_init(ObjMatrix, (void *)&m);

    // Insert to tail module
    module_insert(md, new);
}

// Adds a unit cube, axis-aligned and centered on zero to the Module. If solid is zero, add only lines.
// If solid is non-zero use polygons. Make sure each polygon has surface normals defined for it.
void module_cube(Module *md, int solid)
{
    if (!solid)
    {
        Line edges;
        Point vertex[8];

        //set the points for the verticies of the cube
        point_set(&vertex[0], -0.5, -0.5, -0.5, 1);
        point_set(&vertex[1], 0.5, -0.5, -0.5, 1);
        point_set(&vertex[2], 0.5, 0.5, -0.5, 1);
        point_set(&vertex[3], -0.5, 0.5, -0.5, 1);

        point_set(&vertex[4], -0.5, -0.5, 0.5, 1);
        point_set(&vertex[5], 0.5, -0.5, 0.5, 1);
        point_set(&vertex[6], 0.5, 0.5, 0.5, 1);
        point_set(&vertex[7], -0.5, 0.5, 0.5, 1);

        line_set(&edges, vertex[0], vertex[1]); //1
        module_line(md, &edges);

        line_set(&edges, vertex[0], vertex[3]); //2
        module_line(md, &edges);

        line_set(&edges, vertex[0], vertex[4]); //3
        module_line(md, &edges);

        line_set(&edges, vertex[1], vertex[2]); //4
        module_line(md, &edges);

        line_set(&edges, vertex[1], vertex[5]); //5
        module_line(md, &edges);

        line_set(&edges, vertex[2], vertex[3]); //6
        module_line(md, &edges);

        line_set(&edges, vertex[2], vertex[6]); //7
        module_line(md, &edges);

        line_set(&edges, vertex[3], vertex[7]); //8
        module_line(md, &edges);

        line_set(&edges, vertex[4], vertex[5]); //9
        module_line(md, &edges);

        line_set(&edges, vertex[4], vertex[7]); //10
        module_line(md, &edges);

        line_set(&edges, vertex[5], vertex[6]); //11
        module_line(md, &edges);

        line_set(&edges, vertex[6], vertex[7]); //12
        module_line(md, &edges);
    }
    else
    {
        Point pt[4];
        Polygon p;

        polygon_init(&p);
        point_set3D(&pt[0], -0.5, -0.5, -0.5);
        point_set3D(&pt[1], -0.5, -0.5, 0.5);
        point_set3D(&pt[2], -0.5, 0.5, 0.5);
        point_set3D(&pt[3], -0.5, 0.5, -0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        point_set3D(&pt[0], 0.5, -0.5, -0.5);
        point_set3D(&pt[1], 0.5, -0.5, 0.5);
        point_set3D(&pt[2], 0.5, 0.5, 0.5);
        point_set3D(&pt[3], 0.5, 0.5, -0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        point_set3D(&pt[0], -0.5, -0.5, -0.5);
        point_set3D(&pt[1], -0.5, -0.5, 0.5);
        point_set3D(&pt[2], 0.5, -0.5, 0.5);
        point_set3D(&pt[3], 0.5, -0.5, -0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        point_set3D(&pt[0], -0.5, 0.5, -0.5);
        point_set3D(&pt[1], -0.5, 0.5, 0.5);
        point_set3D(&pt[2], 0.5, 0.5, 0.5);
        point_set3D(&pt[3], 0.5, 0.5, -0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        point_set3D(&pt[0], -0.5, -0.5, -0.5);
        point_set3D(&pt[1], -0.5, 0.5, -0.5);
        point_set3D(&pt[2], 0.5, 0.5, -0.5);
        point_set3D(&pt[3], 0.5, -0.5, -0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        point_set3D(&pt[0], -0.5, -0.5, 0.5);
        point_set3D(&pt[1], -0.5, 0.5, 0.5);
        point_set3D(&pt[2], 0.5, 0.5, 0.5);
        point_set3D(&pt[3], 0.5, -0.5, 0.5);
        polygon_set(&p, 4, pt);
        module_polygon(md, &p);

        polygon_clear(&p);
    }
}

// Shading / Color

// Adds the foreground color value to the tail of the module's list
void module_color(Module *md, Color *c)
{
    Element *new = element_init(ObjColor, (void *)c);

    // Insert to tail module
    module_insert(md, new);
}

// Adds the body color value to the tail of the module’s list
void module_bodyColor(Module *md, Color *c)
{
    Element *new = element_init(ObjBodyColor, (void *)c);

    // Insert to tail module
    module_insert(md, new);
}

// Adds the surface color value to the tail of the module’s list
void module_surfaceColor(Module *md, Color *c)
{
    Element *new = element_init(ObjSurfaceColor, (void *)c);

    // Insert to tail module
    module_insert(md, new);
}

// Adds the specular coefficient to the tail of the module’s list
void module_surfaceCoeff(Module *md, float *coeff)
{
    Element *new = element_init(ObjSurfaceCoeff, coeff);

    // Insert to tail module
    module_insert(md, new);
}

// Use the de Casteljau algorithm to subdivide the Bezier curve divisions times, then add the lines connecting the control points to the module.
// For example, if divisions is 1, the four original Bezier curve control points will be used to generate eight control points and two new Bezier curves.
// Then the algorithm will add six lines to the module, three for each of the smaller Bezier curves.
void module_bezierCurve(Module *m, BezierCurve *b, int divisions)
{
    Line l[3];
    Point temp[8];

    //Base case
    if (divisions == 0)
    {
        line_set(&l[0], b->control[0], b->control[1]);
        module_line(m, &l[0]);

        line_set(&l[1], b->control[1], b->control[2]);
        module_line(m, &l[1]);

        line_set(&l[2], b->control[2], b->control[3]);
        module_line(m, &l[2]);
        //point_print(&(b->control), stdout);
    }
    else
    {
        BezierCurve bezCurveA;
        BezierCurve bezCurveB;

        //Initalize new bezier curves
        bezierCurve_init(&bezCurveA);
        bezierCurve_init(&bezCurveB);

        //Do DeCast Algorithm on them
        deCasteljauAlgo(&(b->control[0]), &(temp[0]));

        //Set up the new bezier curves
        bezierCurve_set(&bezCurveA, &temp[0]);
        bezierCurve_set(&bezCurveB, &temp[4]);

        //Make recursive call
        module_bezierCurve(m, &bezCurveA, divisions - 1);
        module_bezierCurve(m, &bezCurveB, divisions - 1);
    }
}

// Use the de Casteljau algorithm to subdivide the Bezier surface divisions times, then draw either the
// lines connecting the control points, if solid is 0, or draw triangles using the four corner control points.
// For example, if divisions is 1, the 16 original Bezier curve control points will be used to generate 64
// control points and four new Bezier surfaces, 1 level of subdivision, and then the algorithm will draw lines or triangles to connect adjacent control points.
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid)
{
    int i, j;
    Line l[24];
    Polygon p;
    Point vertex[3];
    Point A[16];
    Point A2[16];
    Point B[32];
    Point C[64];
    Point Afinal[16], Bfinal[16], Cfinal[16], Dfinal[16];
    int count;

    //line
    if (divisions == 0)
    {
        if (solid == 0)
        {
            // Draw Vertical lines
            for (i = 4; i < 16; i++)
            {
                line_set(&l[i - 4], b->control[i - 4], b->control[i]);
                module_line(m, &l[i - 4]);
            }

            //Draw horizontal lines
            for (i = 1; i < 16; i++)
            {
                if (i % 4 == 0)
                    continue;
                line_set(&l[i - 1], b->control[i - 1], b->control[i]);
                module_line(m, &l[i - 1]);
            }
        }
        else
        {
            //Draw polygon stuff
            for (i = 0; i < 3; i++)
            {
                polygon_init(&p);

                // draw the first column of triangles!
                point_copy(&vertex[0], &(b->control[0 + i]));
                point_copy(&vertex[1], &(b->control[4 + i]));
                point_copy(&vertex[2], &(b->control[5 + i]));
                polygon_set(&p, 3, &vertex[0]);
                module_polygon(m, &p);

                point_copy(&vertex[0], &(b->control[4 + i]));
                point_copy(&vertex[1], &(b->control[8 + i]));
                point_copy(&vertex[2], &(b->control[9 + i]));
                polygon_set(&p, 3, &vertex[0]);
                module_polygon(m, &p);

                point_copy(&vertex[0], &(b->control[8 + i]));
                point_copy(&vertex[1], &(b->control[12 + i]));
                point_copy(&vertex[2], &(b->control[13 + i]));
                polygon_set(&p, 3, &vertex[0]);
                module_polygon(m, &p);

                point_copy(&vertex[0], &(b->control[0 + i]));
                point_copy(&vertex[1], &(b->control[5 + i]));
                point_copy(&vertex[2], &(b->control[1 + i]));
                polygon_set(&p, 3, &vertex[0]);
                module_polygon(m, &p);

                point_copy(&vertex[0], &(b->control[4 + i]));
                point_copy(&vertex[1], &(b->control[9 + i]));
                point_copy(&vertex[2], &(b->control[5 + i]));
                polygon_set(&p, 3, &vertex[0]);
                module_polygon(m, &p);

                point_copy(&vertex[0], &(b->control[8 + i]));
                point_copy(&vertex[1], &(b->control[13 + i]));
                point_copy(&vertex[2], &(b->control[9 + i]));
                polygon_set(&p, 3, &vertex[0]);
                module_polygon(m, &p);

                polygon_clear(&p);
            }
        }
    }
    else
    {
        //Recursive stuff
        BezierSurface bezSurfaceA;
        BezierSurface bezSurfaceB;
        BezierSurface bezSurfaceC;
        BezierSurface bezSurfaceD;

        //initalize the new surfaces one for each quadrant
        bezierSurface_init(&bezSurfaceA);
        bezierSurface_init(&bezSurfaceB);
        bezierSurface_init(&bezSurfaceC);
        bezierSurface_init(&bezSurfaceD);

        //Set up a temp list of vertices that are organized by column
        count = 0;
        for (j = 0; j < 4; j++)
        {
            for (i = 0; i < 4; i++)
            {
                A[count++] = b->control[i * 4 + j];
            }
        }

        //Run deCast algorithm on the column points getting 32 points
        for (i = 0; i < 4; i++)
        {
            deCasteljauAlgo(&(A[i * 4]), &(B[i * 8]));
        }

        //Set up a new temp list of vertices that are organized by row
        count = 0;
        for (j = 0; j < 8; j++)
        {
            for (i = 0; i < 4; i++)
            {
                A2[count++] = B[i * 8 + j];
            }
        }

        //Run deCast algorithm on the row points ending with a total of 64 points
        for (i = 0; i < 8; i++)
        {
            deCasteljauAlgo(&(A2[i * 4]), &(C[i * 8]));
        }

        //Split up the points into the appropriate quadrants
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                Afinal[i * 4 + j] = C[i * 8 + j];
                Bfinal[i * 4 + j] = C[i * 8 + (j + 4)];
                Cfinal[i * 4 + j] = C[(i + 4) * 8 + j];
                Dfinal[i * 4 + j] = C[(i + 4) * 8 + (j + 4)];
            }
        }

        //set the new bezier surfaces
        bezierSurface_set(&bezSurfaceA, &Afinal[0]);
        bezierSurface_set(&bezSurfaceB, &Bfinal[0]);
        bezierSurface_set(&bezSurfaceC, &Cfinal[0]);
        bezierSurface_set(&bezSurfaceD, &Dfinal[0]);

        //Make the 4 recursive calls
        module_bezierSurface(m, &bezSurfaceA, divisions - 1, solid);
        module_bezierSurface(m, &bezSurfaceB, divisions - 1, solid);
        module_bezierSurface(m, &bezSurfaceC, divisions - 1, solid);
        module_bezierSurface(m, &bezSurfaceD, divisions - 1, solid);
    }
}

// Adds a unit cylinder.
// By Bruce Maxwell.
void module_cylinder(Module *m, int divisions)
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
    for (i = 0; i < divisions; i++)
    {
        Point pt[4];

        x1 = cos(i * M_PI * 2.0 / divisions);
        z1 = sin(i * M_PI * 2.0 / divisions);
        x2 = cos(((i + 1) % divisions) * M_PI * 2.0 / divisions);
        z2 = sin(((i + 1) % divisions) * M_PI * 2.0 / divisions);

        point_copy(&pt[0], &xtop);
        point_set3D(&pt[1], x1, 1.0, z1);
        point_set3D(&pt[2], x2, 1.0, z2);

        polygon_set(&p, 3, pt);
        module_polygon(m, &p);
        point_copy(&pt[0], &xbot);
        point_set3D(&pt[1], x1, 0.0, z1);
        point_set3D(&pt[2], x2, 0.0, z2);

        polygon_set(&p, 3, pt);
        module_polygon(m, &p);

        point_set3D(&pt[0], x1, 0.0, z1);
        point_set3D(&pt[1], x2, 0.0, z2);
        point_set3D(&pt[2], x2, 1.0, z2);
        point_set3D(&pt[3], x1, 1.0, z1);

        polygon_set(&p, 4, pt);
        module_polygon(m, &p);
    }
    polygon_clear(&p);
}

// Adds a unit circle.
void module_cirle(Module *m, int divisions)
{
    Polygon p;
    Point xCenter;
    double x1, x2, z1, z2;
    int i;

    polygon_init(&p);
    point_set3D(&xCenter, 0, 0.0, 0.0);

    // make a fan for the top and bottom sides
    // and quadrilaterals for the sides
    for (i = 0; i < divisions; i++)
    {
        Point pt[3];

        x1 = cos(i * M_PI * 2.0 / divisions);
        z1 = sin(i * M_PI * 2.0 / divisions);
        x2 = cos(((i + 1) % divisions) * M_PI * 2.0 / divisions);
        z2 = sin(((i + 1) % divisions) * M_PI * 2.0 / divisions);

        point_copy(&pt[0], &xCenter);
        point_set3D(&pt[1], x1, 0.0, z1);
        point_set3D(&pt[2], x2, 0.0, z2);

        polygon_set(&p, 3, pt);
        module_polygon(m, &p);
    }
    polygon_clear(&p);
}

// Adds a unit cone.
void module_cone(Module *m, int divisions)
{
    if (divisions >= 3)
    {
        int i;
        double x, z;
        Point top;
        Polygon *base;
        Point p[divisions];
        Line l[divisions];
        point_set3D(&top, 0, 1, 0);

        for (i = 0; i < divisions; i++)
        {
            x = cos(i * M_PI * 2.0 / divisions);
            z = sin(i * M_PI * 2.0 / divisions);
            point_set3D(&p[i], x, 0, z);
            line_set(&l[i], p[i], top);
            module_line(m, &l[i]);
        }

        base = polygon_createp(divisions, &p[0]);
        module_polygon(m, base);
        polygon_clear(base);
    }
}

// Adds a unit tetrahedron.
void module_tetrahedron(Module *m)
{
    Point tet[6];
    Point temp[3];
    Polygon pol[8];
    int i;
    point_set3D(&tet[0], 0, 0, 0);
    point_set3D(&tet[1], 1, 0, 0);
    point_set3D(&tet[2], 1, 0, 1);
    point_set3D(&tet[3], 0, 0, 1);
    point_set3D(&tet[4], .5, 1, .5);
    point_set3D(&tet[5], .5, -1, .5);

    for (i = 0; i < 8; i++)
    {
        polygon_init(&pol[i]);
    }

    point_copy(&temp[0], &tet[0]);
    point_copy(&temp[1], &tet[1]);
    point_copy(&temp[2], &tet[4]);

    polygon_set(&pol[0], 3, temp);

    point_copy(&temp[0], &tet[1]);
    point_copy(&temp[1], &tet[2]);
    point_copy(&temp[2], &tet[4]);

    polygon_set(&pol[1], 3, temp);

    point_copy(&temp[0], &tet[2]);
    point_copy(&temp[1], &tet[3]);
    point_copy(&temp[2], &tet[4]);

    polygon_set(&pol[2], 3, temp);

    point_copy(&temp[0], &tet[0]);
    point_copy(&temp[1], &tet[3]);
    point_copy(&temp[2], &tet[4]);

    polygon_set(&pol[3], 3, temp);

    point_copy(&temp[0], &tet[0]);
    point_copy(&temp[1], &tet[1]);
    point_copy(&temp[2], &tet[5]);

    polygon_set(&pol[4], 3, temp);

    point_copy(&temp[0], &tet[1]);
    point_copy(&temp[1], &tet[2]);
    point_copy(&temp[2], &tet[5]);

    polygon_set(&pol[5], 3, temp);

    point_copy(&temp[0], &tet[2]);
    point_copy(&temp[1], &tet[3]);
    point_copy(&temp[2], &tet[5]);

    polygon_set(&pol[6], 3, temp);

    point_copy(&temp[0], &tet[0]);
    point_copy(&temp[1], &tet[3]);
    point_copy(&temp[2], &tet[5]);

    polygon_set(&pol[7], 3, temp);

    for (i = 0; i < 8; i++)
    {
        module_polygon(m, &pol[i]);
    }
}

// Adds a unit ellipsoid.
void module_ellipsoid(Module *m, int divisions)
{
    if (divisions >= 3)
    {
        int i;
        double x, z;
        Point top, bottom;
        Polygon *base;
        Point p[divisions];
        Line l[divisions];
        point_set3D(&top, 0, 0.7, 0);
        point_set3D(&bottom, 0, -0.7, 0);

        for (i = 0; i < divisions; i++)
        {
            x = cos(i * M_PI * 2.0 / divisions);
            z = sin(i * M_PI * 2.0 / divisions);
            point_set3D(&p[i], x, 0, z);
            line_set(&l[i], p[i], top);
            module_line(m, &l[i]);

            line_set(&l[i], p[i], bottom);
            module_line(m, &l[i]);
        }

        base = polygon_createp(divisions, &p[0]);
        module_polygon(m, base);
        polygon_clear(base);
    }
}

// Adds a unit drop of liquid.
void module_drop(Module *m, int divisions)
{
    if (divisions >= 3)
    {
        int i;
        double x, z;
        Point top, bottom;
        Polygon *base;
        Point p[divisions];
        Line l[divisions];
        point_set3D(&top, 0, 0.7, 0);
        point_set3D(&bottom, 0, -0.4, 0);

        for (i = 0; i < divisions; i++)
        {
            x = cos(i * M_PI * 2.0 / divisions);
            z = sin(i * M_PI * 2.0 / divisions);
            point_set3D(&p[i], x, 0, z);
            line_set(&l[i], p[i], top);
            module_line(m, &l[i]);

            line_set(&l[i], p[i], bottom);
            module_line(m, &l[i]);
        }

        base = polygon_createp(divisions, &p[0]);
        module_polygon(m, base);
        polygon_clear(base);
    }
}
