/*
  This is an example of reading and writing an greenImage using the ppmIO.c
  routines.  To manipulate the greenImage, change the pixel color values.

  Roshan Bellary
*/

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[]) {
  Pixel *finalImage;
  Pixel *greenImage;
  Pixel *bgImage;
  Pixel *maskImage;

  long imagesize;
  long i;

  if(argc < 6) {
    printf("Usage: mergeTwo <green screen file> <bg file> <mask> <output file> <dx> <dy>\n");
    exit(-1);
  }

  /* read in the greenImage */
  int greenImageRows, greenImageCols, greenImageColors;
  greenImage = readPPM(&greenImageRows, &greenImageCols, &greenImageColors, argv[1]);
  if(!greenImage) {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  /* read in the bgImage */
  int bgRows, bgCols, bgColors;
  bgImage = readPPM(&bgRows, &bgCols, &bgColors, argv[2]);
  if(!bgImage) {
    fprintf(stderr, "Unable to read %s\n", argv[2]);
    exit(-1);
  }

  /* read in the maskImage */
  int maskRows, maskCols, maskColors;
  maskImage = readPPM(&maskRows, &maskCols, &maskColors, argv[3]);
  if(!maskImage) {
    fprintf(stderr, "Unable to read %s\n", argv[3]);
    exit(-1);
  }

  /* read in the greenImage */
  int rows, cols, colors;
  finalImage = readPPM(&rows, &cols, &colors, argv[4]);
  if(!finalImage) {
    fprintf(stderr, "Unable to read %s\n", argv[4]);
    exit(-1);
  }

  /* read the dx */
  int dx = argv[5];

  /* read the dy */
  int dy = argv[6];

  /* calculate the finalImage size */
  imagesize = (long)rows * (long)cols;

  // Reset image <- not needed.
  for(i=0;i<imagesize;i++) 
  {
    finalImage[i].r = 0;
    finalImage[i].g = 0;
    finalImage[i].b = 0;
  }

  /* mess with the finalImage here  */
  for(i=0;i<imagesize;i++) 
  {
    //int alpha = g/255;
    // I = alpha * image1 + (1 - alpha) * image2
    //finalImage[i].r = (maskImage[i].r * greenImage[i].r) + (1 - maskImage[i].r) * bgImage[i].r;
    //finalImage[i].g = (maskImage[i].g * greenImage[i].g) + (1 - maskImage[i].g) * bgImage[i].g;
    //finalImage[i].b = (maskImage[i].b * greenImage[i].b) + (1 - maskImage[i].b) * bgImage[i].b;
  
    if (maskImage[i].r == 0 || maskImage[i].g == 0 || maskImage[i].b == 0)
    {
      // Not Green screen. Take Colors from greenScreenImage.
      finalImage[i].r = greenImage[i].r;
      finalImage[i].g = greenImage[i].g;
      finalImage[i].b = greenImage[i].b;
    }
    else 
    {
      // Green screen. Take Colors from bgImage.
      finalImage[i].r = bgImage[i].r;
      finalImage[i].g = bgImage[i].g;
      finalImage[i].b = bgImage[i].b;
    }
  }

  /* write out the resulting finalImage */
  writePPM(finalImage, rows, cols, colors /* s/b 255 */, argv[4]);

  /* free the finalImage memory */
#if USECPP
  delete[] finalImage;
#else
  free(finalImage);
#endif

  return(0);
}
