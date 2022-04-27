/*
	Creates a Mandelbrot set at a selected location and Julia sets as well. Just for testing.

  CS5310 - Roshan Bellary
	09-23-2021 (Fall 2021)
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "fractals.h"

int main(int argc, char *argv[]) 
{
  Image *src;

	src = image_create(750, 1000);

	mandelbrot( src, -1.5, -1.5, 4);

  image_write( src, "mandelbrot-full-white.ppm");

	mandelbrot( src, 1.755, -0.02, 0.02);

  image_write( src, "mandelbrot-white.ppm");

	julia( src, -1.8, -1.8*0.75, 3.6);

  image_write( src, "juliaVariation2.ppm");

  image_free( src );

  return(0);
}

// Parameters: Image, anchor on the complex plane (x0, y0) and width of the rectangle
void mandelbrot( Image *dst, float x0, float y0, float dx)
{
  FPixel white = {1, 1, 1};
  FPixel black = {0, 0, 0};

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
      if (k > 50)
      {
        image_setf(dst, i, j, white);
      }
      else
      {
        image_setf(dst, i, j, black);
      }

    }
  }
}

// Parameters: Image, anchor on the complex plane (x0, y0) and width of the rectangle
void julia(Image *dst, float x0, float y0, float dx)
{
  FPixel white = {1, 1, 1};
  FPixel black = {0, 0, 0};

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
      // Constant for Julia set.
      x = -0.8696;
      y = 0.26;

      // Plane location.
      zx = (sx * j) + x0;
      zy = -(sy * i) + y1;

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

      image_setf(dst, i, j, generateFPixelBasedOnSeed(k, returnRandom(0,2)));

      // // color pixel (i, j)
      // if (k == 1000)
      // {
      //   image_setf(dst, i, j, white);
      // }
      // else
      // {
      //   image_setf(dst, i, j, black);
      // }
    }
  }
}