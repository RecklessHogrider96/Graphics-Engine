/*
  Bruce A. Maxwell
  Fall 2014

	Skeleton scanline fill algorithm
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "graphics.h"
#include "list.h"
#include "polygon.h"
#include "drawstate.h"

/****************************************
Start Scanline Fill
*****************************************/
typedef struct tEdge
{
	float x0, y0, z0; /* start point for the edge */
	float x1, y1, z1; /* end point for the edge */
	int yStart, yEnd; /* start row and end row */
	float xIntersect, dxPerScan;
	float zIntersect, dzPerScan; /* where the edge intersects the current scanline and how it changes */
	/* we'll add more here later */
	struct tEdge *next;
} Edge;

/*
    This is a comparison function that returns a value < 0 if a < b, a
    value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
    Edge structure.  It is used to sort the overall edge list.
 */
static int compYStart(const void *a, const void *b)
{
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	return (ea->yStart - eb->yStart);
}

/*
    This is a comparison function that returns a value < 0 if a < b, a
    value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
    Edge structure.  It is used to sort the active edge list.
 */
static int compXIntersect(const void *a, const void *b)
{
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	if (ea->xIntersect < eb->xIntersect)
		return (-1);
	else if (ea->xIntersect > eb->xIntersect)
		return (1);

	return (0);
}

/*
    Allocates, creates, fills out, and returns an Edge structure given
    the inputs.

    Current inputs are just the start and end location in image space.
    Eventually, the points will be 3D and we'll add color and texture
    coordinates.
 */
static Edge *makeEdgeRec(Point start, Point end, DrawState *ds, Image *src)
{
	Edge *edge;

	edge = (Edge *)malloc(sizeof(Edge));

	edge->x0 = start.val[0];
	edge->y0 = start.val[1];
	edge->z0 = start.val[2];

	edge->x1 = end.val[0];
	edge->y1 = end.val[1];
	edge->z1 = end.val[2];

	edge->yStart = (int)(edge->y0 + 0.5);
	edge->yEnd = (int)(edge->y1 + 0.5) - 1;
	edge->dxPerScan = (edge->x1 - edge->x0) / (edge->y1 - edge->y0);

	if (ds->shade == ShadeDepth)
	{
		edge->dzPerScan = (1 / edge->z1 - 1 / edge->z0) / (edge->y1 - edge->y0);
	}
	else
	{
		edge->dzPerScan = 0;
	}

	//Correctly initializing xIntersect
	edge->xIntersect = edge->x0 + abs((edge->y0 - edge->yStart)) * edge->dxPerScan;

	edge->zIntersect = 1 / edge->z0 + abs((edge->y0 - edge->yStart)) * edge->dzPerScan;

	//Clipping if the edge starts off the image or goes off image
	if (edge->y0 < 0)
	{
		//if edge starts below row 0
		edge->xIntersect += -edge->y0 * edge->dxPerScan;
		edge->zIntersect += -edge->y0 * edge->dzPerScan;
		edge->y0 = 0;
		edge->yStart = 0;
	}

	if (edge->yEnd > src->rows - 1)
	{
		//if the edge starts inside the image but continues outside
		edge->yEnd = src->rows - 1;
	}

	if (edge->xIntersect < edge->x1 && edge->dxPerScan < 0)
	{
		edge->xIntersect = edge->x1;
	}

	if (edge->xIntersect > edge->x1 && edge->dxPerScan > 0)
	{
		edge->xIntersect = edge->x1;
	}

	edge->next = NULL;

	return (edge);
}

/*
    Returns a list of all the edges in the polygon in sorted order by
    smallest row.
*/
static LinkedList *setupEdgeList(Polygon *p, DrawState *ds, Image *src)
{
	LinkedList *edges = NULL;
	Point v1, v2;
	int i;

	// create a linked list
	edges = ll_new();

	// walk around the polygon, starting with the last point
	v1 = p->vertex[p->nVertex - 1];

	for (i = 0; i < p->nVertex; i++)
	{

		// the current point (i) is the end of the segment
		v2 = p->vertex[i];

		// if it is not a horizontal line
		if ((int)(v1.val[1] + 0.5) != (int)(v2.val[1] + 0.5))
		{
			Edge *edge;

			// if the first coordinate is smaller (top edge)
			if (v1.val[1] < v2.val[1])
				edge = makeEdgeRec(v1, v2, ds, src);
			else
				edge = makeEdgeRec(v2, v1, ds, src);

			// insert the edge into the list of edges if it's not null
			if (edge)

				ll_insert(edges, edge, compYStart);
		}
		v1 = v2;
	}

	// check for empty edges (like nothing in the viewport)
	if (ll_empty(edges))
	{
		ll_delete(edges, NULL);
		edges = NULL;
	}

	return (edges);
}

/*
    Draw one scanline of a polygon given the scanline, the active edges,
    a DrawState, the image, and some Lights (for Phong shading only).
 */
static void fillScan(int scan, LinkedList *active, DrawState *ds, Image *src)
{
	Edge *p1, *p2;
	float curZ;
	float dzPerColumn;
	int i;

	// loop over the list
	p1 = ll_head(active);
	while (p1)
	{
		// the edges have to come in pairs, draw from one to the next
		p2 = ll_next(active);
		if (!p2)
		{
			printf("bad bad bad (your edges are not coming in pairs)\n");
			break;
		}

		// if the xIntersect values are the same, don't draw anything.
		// Just go to the next pair.
		if (p2->xIntersect == p1->xIntersect)
		{
			p1 = ll_next(active);
			printf("xIntersect values are the same.\n");
			continue;
		}

		if (p1->xIntersect < 0)
		{
			// if starts to draw before the left of the image
			p1->xIntersect = 0;
		}
		if (p2->xIntersect > src->cols)
		{
			// if ends drawing beyound the right side of the image
			p2->xIntersect = src->cols;
		}

		int colStart = (int)(p1->xIntersect);
		int colEnd = (int)(p2->xIntersect + 1);
		int row = scan;

		curZ = p1->zIntersect;
		dzPerColumn = (p2->zIntersect - p1->zIntersect) / (colEnd - colStart);

		for (i = colStart; i < colEnd; i++)
		{
			if ((curZ - src->fpixel[row][i].z) > 0.01)
			{
				if (ds->shade == ShadeConstant)
				{
					image_setColor(src, row, i, ds->color);
				}
				else if (ds->shade == ShadeDepth)
				{
					Color c;
					float z = 1 / curZ;
					color_set(&c,
							  ds->scaleFactor * (1 - z) * ds->color.c[0],
							  ds->scaleFactor * (1 - z) * ds->color.c[1],
							  ds->scaleFactor * (1 - z) * ds->color.c[2]);
					image_setColor(src, row, i, c);
				}

				src->fpixel[row][i].z = curZ;
			}
			curZ += dzPerColumn;
		}

		// move ahead to the next pair of edges
		p1 = ll_next(active);
	}

	return;
}

/* 
     Process the edge list, assumes the edges list has at least one entry
*/
static int processEdgeList(LinkedList *edges, DrawState *ds, Image *src)
{
	LinkedList *active = NULL;
	LinkedList *tmplist = NULL;
	LinkedList *transfer = NULL;
	Edge *current;
	Edge *tedge;
	int scan = 0;

	active = ll_new();
	tmplist = ll_new();

	current = ll_head(edges);
	// start at the first scanline and go until the active list is empty
	for (scan = current->yStart; scan < src->rows; scan++)
	{

		// grab all edges starting on this row
		while (current != NULL && current->yStart == scan)
		{
			ll_insert(active, current, compXIntersect);
			current = ll_next(edges);
		}

		if (ll_empty(active))
		{
			printf("Edges are empty.\n");
			break;
		}

		fillScan(scan, active, ds, src);

		// remove any ending edges and update the rest
		for (tedge = ll_pop(active); tedge != NULL; tedge = ll_pop(active))
		{

			// keep anything that's not ending
			if (tedge->yEnd > scan)
			{
				float a = 1.0;

				// update the edge information with the dPerScan values
				tedge->xIntersect += tedge->dxPerScan;
				tedge->zIntersect += tedge->dzPerScan;

				// adjust in the case of partial overlap
				if (tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1)
				{
					a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
					tedge->xIntersect = tedge->x1;
				}
				else if (tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1)
				{
					a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
					tedge->xIntersect = tedge->x1;
				}

				ll_insert(tmplist, tedge, compXIntersect);
			}
		}
		//
		transfer = active;
		active = tmplist;
		tmplist = transfer;
	}

	// get rid of the lists, but not the edge records
	ll_delete(active, NULL);
	ll_delete(tmplist, NULL);

	return (0);
}

/*
    Draws a filled polygon of the specified color into the image src.
 */
void polygon_drawFill(Polygon *p, Image *src, DrawState *ds)
{
	LinkedList *edges = NULL;

	// set up the edge list
	edges = setupEdgeList(p, ds, src);
	if (!edges)
	{
		printf("Edges are null.\n");
		return;
	}

	// process the edge list (should be able to take an arbitrary edge list)
	processEdgeList(edges, ds, src);

	// clean up
	ll_delete(edges, (void (*)(const void *))free);

	return;
}
/****************************************
End Scanline Fill
*****************************************/
