/* 
  CS5310 - Roshan Bellary
  11-10-2021 (Fall 2021)

  Bezier Code.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "graphics.h"
#include "bezier.h"

// Sets the zbuffer flag to 1 and the curve poitns to the X-axis between 0 and 1.
void bezierCurve_init(BezierCurve *b)
{
    int i;
    Point p[4];

    //set up the points to be on the x axis between 0 and 1.
    for (i = 0; i < 4; i++)
    {
        point_set3D(&(p[i]), (float)i / 4, 0.0, 0.0);
        ;
        b->control[i] = p[i];
    }

    //set the z-buffer to 1
    b->zBuffer = 1.0;
}

// Sets the zbuffer flag to 1 and the surface to the X-Z plane between (0,0) and (1,1).
void bezierSurface_init(BezierSurface *b)
{
    int i, j;
    Point p[16];

    //set up the points to be on the x, z plane between (0,0) and (1,1).
    for (i = 0; i < 16; i++)
    {
        point_set3D(&(p[i]), (float)i / 16, 0.0, (float)i / 16);
        b->control[i] = p[i];
    }

    //set the z-buffer to 1
    b->zBuffer = 1.0;
}

// Sets the contorl points of the BezierCurve to the four points in the vlist array.
void bezierCurve_set(BezierCurve *b, Point *vlist)
{
    b->control[0] = vlist[0];
    b->control[1] = vlist[1];
    b->control[2] = vlist[2];
    b->control[3] = vlist[3];
}

// Sets the control points of the BezierSurface to the 16 poitns in the vlist array.
void bezierSurface_set(BezierSurface *b, Point *vlist)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        b->control[i] = vlist[i];
    }
}

// Sets the z-buffer flag to the given value.
void bezierCurve_zBuffer(BezierCurve *p, int flag)
{
    p->zBuffer = flag;
}

// Sets the z_bffer flag to the given value.
void bezierSurface_zBuffer(BezierCurve *p, int flag)
{
    p->zBuffer = flag;
}

// Draws the Bezier curve, given in screen coordinates, into the image using the given color.
// The function should be adaptive so that it uses an appropriate number of line segments to draw the curve.
// For example if the bounding box of the control points is less than 10 pixels in the largest dimension,
// then it is reasonable to draw the lines between teh control poitns as an approximation to the curve.
void bezierCurve_draw(BezierCurve *b, Image *src, Color c)
{
    BezierCurve bezCurveA;
    BezierCurve bezCurveB;

    Line l;
    Point q[4];
    Point r[4];
    float minBound, maxBound, box;
    float dst;
    int i, j, k;
    minBound = INFINITY;
    maxBound = -INFINITY;

    for (k = 0; k < 4; k++)
    {
        dst = 0;
        for (j = 1; j < 4; j++)
        {
            dst = (b->control[k].val[0] - b->control[j - 1].val[0]) + (b->control[k].val[1] - b->control[j - 1].val[1]);
            minBound = minBound > dst ? dst : minBound;
            maxBound = maxBound < dst ? dst : maxBound;
        }
    }
    box = minBound * maxBound;
    box = box < 0 ? box * -1 : box;

    // Do the necessary subdivisions to find points on the line.
    q[0] = b->control[0];
    r[3] = b->control[3];

    // Get the average of the x, y, and z values for the other points
    for (i = 0; i < 4; i++)
    {
        q[1].val[i] = (b->control[0].val[i] + b->control[1].val[i]) * 0.5;
        q[2].val[i] = 0.5 * q[1].val[i] + 0.25 * (b->control[1].val[i] + b->control[2].val[i]);

        r[2].val[i] = (b->control[2].val[i] + b->control[3].val[i]) * 0.5;
        r[1].val[i] = 0.5 * r[2].val[i] + 0.25 * (b->control[1].val[i] + b->control[2].val[i]);
        
        q[3].val[i] = (q[2].val[i] + r[1].val[i]) * 0.5;
        r[0].val[i] = q[3].val[i];
    }

    if (box < 100)
    {
        line_set(&l, b->control[0], b->control[1]);
        line_draw(&l, src, c);

        line_set(&l, b->control[1], b->control[2]);
        line_draw(&l, src, c);

        line_set(&l, b->control[2], b->control[3]);
        line_draw(&l, src, c);
    }
    else
    {
        // Set the temporary bez Curves
        bezierCurve_set(&bezCurveA, q);
        bezierCurve_set(&bezCurveB, r);

        // Make recursive calls to continue to decrease the size of the line segments
        bezierCurve_draw(&bezCurveA, src, c);
        bezierCurve_draw(&bezCurveB, src, c);
    }
}

// De Casteljau Algorithm.
void deCasteljauAlgo(Point *a, Point *b)
{
    Point q[8];
    int i;

    //do the necessary subdivisions to find points on the line.
    q[0] = a[0];
    q[7] = a[3];

    //get the average of the x, y, and z values for the other points
    for (i = 0; i < 4; i++)
    {
        q[1].val[i] = (a[0].val[i] + a[1].val[i]) * 0.5;
        q[2].val[i] = 0.5 * q[1].val[i] + 0.25 * (a[1].val[i] + a[2].val[i]);

        q[6].val[i] = (a[2].val[i] + a[3].val[i]) * 0.5;
        q[5].val[i] = 0.5 * q[6].val[i] + 0.25 * (a[1].val[i] + a[2].val[i]);

        //middle point
        q[3].val[i] = (q[2].val[i] + q[5].val[i]) * 0.5;
    }
    q[4] = q[3];
    for (i = 0; i < 8; i++)
    {
        b[i] = q[i];
    }
}
