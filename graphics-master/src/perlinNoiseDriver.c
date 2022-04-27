/*
  Creates a Perlin - like Noise set

  CS5310 - Roshan Bellary
	09-23-2021 (Fall 2021)
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"

int main(int argc, char *argv[]) 
{
  // inputs are a rectangle on the complex plane and the number of rows in the output image
  if(argc < 2) {
    printf("Usage: Perlin Like Noise <rows (int)> <cols (int)>\n");
    exit(-1);
  }

  int rows, cols;

  float noiseVal;

  rows = atoi(argv[1]);
  cols = atoi(argv[2]);

  FPixel p = {0, 0, 0};

  Image *src;

	src = image_create(rows, cols);

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      noiseVal = (float)returnRandom(0,1);
      printf("Noise: %f\n", noiseVal);
      p.rgb[0] = noiseVal;
      p.rgb[1] = noiseVal;
      p.rgb[2] = noiseVal;

      image_setf(src, i, j, p);
    }
  }
  
  image_write( src, "perlinOutput.ppm");

  image_free( src );

  return(0);
}
