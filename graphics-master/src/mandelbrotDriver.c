/*
  Creates a Mandelbrot set at a selected location based on Inputs.

  CS5310 - Roshan Bellary
	09-23-2021 (Fall 2021)
*/
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "fractals.h"

int main(int argc, char *argv[]) 
{
  // inputs are a rectangle on the complex plane and the number of rows in the output image
  if(argc < 6) {
    printf("Usage: MandelbrotDriver (rectangle on the complex plane) <x0> <y0> <x1> <y1> <rows (int)>\n");
    exit(-1);
  }

  int rows, cols;
  float x0, y0, x1, y1, dx, dy;

  x0 = atof(argv[1]);
  y0 = atof(argv[2]);
  x1 = atof(argv[3]);
  y1 = atof(argv[4]);
  rows = atoi(argv[5]);

  dx = x1 - x0;
  dy = y1 - y0;

  cols = dx * rows / dy;

  printf("Cols: %d\n", cols);

  Image *src;

	src = image_create(rows, cols);

	mandelbrot( src, x0, y0, dx);

  image_write( src, "mandelbrotDriverOutput.ppm");

  image_free( src );

  return(0);
}

// Parameters: Image, anchor on the complex plane (x0, y0) and width of the rectangle
void mandelbrot( Image *dst, float x0, float y0, float dx)
{
  float dy, sx, sy, x, y, zx, zy, zxn, x1, y1;
  int i, j, k, rows, cols;

  rows = dst->rows;
  cols = dst->cols;

  // calculate the number of columns cols = dx * rows / dy
  dy = (dx * (float)rows) / (float)cols;

  sx = dx / (float)cols;
  sy = dy / (float)rows;

  x1 = dx + x0;
  y1 = dy + y0;

  // for each pixel in the image (i, j)
  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < cols; j++)
    {
      // calculate (x, y) given (i, j)
      // this corresponds to cx and cy in the Mandelbrot equation
      x = (sx * j) + x0;
      y = -(sy * i) + y1;
      
      // set zx and zy to (0, 0)
      zx = 0;
      zy = 0;

      // for some number of iterations up to N (e.g. 100)
      for (k = 0; k < 1000; k++)
      {
        // iterate the Mandelbrot equation
        // zx zy
        zxn = (zx*zx) - (zy*zy) - x;
        zy = 2*zx*zy - y;
        zx = zxn;

        // if the length of z is greater than 2.0,
        if (zx*zx + zy*zy > 2*2)
        {
          // store the number of iterations and break
          break;
        }
      }

      // color pixel (i, j)
      image_setf(dst, i, j, generateFPixelBasedOnSeed(k, returnRandom(0,2)));

    }
  }
}