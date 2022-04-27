/*
    CS5310 - Roshan Bellary
	09-28-2021 (Fall 2021)

	Simple test program drawing dotted lines in all four quadrants.
*/

#include <stdio.h>

#include "graphics.h"

int main(int argc, char *argv[]) {

	printf("0\n");

	Image *src;
	Line line;
  	Color White;
  	Color Red;
  	Color Green;

	printf("1\n");

	color_set( &White, 1.0, 1.0, 1.0 );
	color_set( &Red, 0.9, 0.1, 0.1 );
	color_set( &Green, 0.1, 0.9, 0.1 );

	printf("2\n");

	src = image_create( 100, 100 );

	printf("3\n");

	// horizontal line in the 1st quadrant
	line_set2D( &line, 55, 50, 90, 50 );

	printf("4\n");

	line_draw_dotted( &line, src, White );

	printf("5\n");

	// vertical line in the 2nd quadrant
	line_set2D( &line, 50, 45, 50, 10 );
	line_draw_dotted( &line, src, White );

	// horizontal line in the 3rd quadrant
	line_set2D( &line, 45, 50, 10, 50 );
	line_draw_dotted( &line, src, White );

	// vertical line in the 4th quadrant
	line_set2D( &line, 50, 55, 50, 90 );
	line_draw_dotted( &line, src, White );

	// line in the 1st octant
	line_set2D( &line, 60, 45, 90, 35);
	line_draw_dotted( &line, src, Red );

	// 45 degree line in 1st quadrant
	line_set2D( &line, 60, 40, 90, 10);
	line_draw_dotted( &line, src, Green );

	// line in the 2nd octant
	line_set2D( &line, 55, 40, 65, 10);
	line_draw_dotted( &line, src, Red );

	// line in the 3rd octant
	line_set2D( &line, 45, 40, 35, 10);
	line_draw_dotted( &line, src, Red );

	// 45 degree line in 2nd quadrant
	line_set2D( &line, 40, 40, 10, 10);
	line_draw_dotted( &line, src, Green );

	// line in the 4th octant
	line_set2D( &line, 40, 45, 10, 35);
	line_draw_dotted( &line, src, Red );

	// line in the 5th octant
	line_set2D( &line, 40, 55, 10, 65);
	line_draw_dotted( &line, src, Red );

	// 45 degree line in 3rd quadrant
	line_set2D( &line, 40, 60, 10, 90);
	line_draw_dotted( &line, src, Green );

	// line in the 6th octant
	line_set2D( &line, 45, 60, 35, 90);
	line_draw_dotted( &line, src, Red );

	// line in the 7th octant
	line_set2D( &line, 55, 60, 65, 90);
	line_draw_dotted( &line, src, Red );

	// 45 degree line in 4th quadrant
	line_set2D( &line, 60, 60, 90, 90);
	line_draw_dotted( &line, src, Green );

	// line in the 6th octant
	line_set2D( &line, 60, 55, 90, 65);
	line_draw_dotted( &line, src, Red );

	image_write(src, "test3d.ppm");
	image_free(src);

	return(0);
}
