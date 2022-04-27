#ifndef IMAGE_H

#define IMAGE_H

#include <stdio.h>

// FPixel Data structure
typedef struct
{
  float rgb[3];
  float a;
  float z;
} FPixel;

// Color Data structure
typedef struct
{
  float c[3];
} Color;

// Image Data structure
typedef struct
{
  FPixel **fpixel;
  int rows;
  int cols;
  int colors;
  int maxval;
  char *filename;
} Image;

// Constructors and destructors
Image *image_create(int rows, int cols);
void image_free(Image *src);
void image_init(Image *src);
int image_alloc(Image *src, int rows, int cols);
void image_dealloc(Image *src);

// Access
FPixel image_getf(Image *src, int r, int c);
float image_getc(Image *src, int r, int c, int b);
float image_geta(Image *src, int r, int c);
float image_getz(Image *src, int r, int c);
void image_setf(Image *src, int r, int c, FPixel val);
void image_setc(Image *src, int r, int c, int b, float val);
void image_seta(Image *src, int r, int c, float val);
void image_setz(Image *src, int r, int c, float val);

// Utility
void image_reset(Image *src);
void image_fill(Image *src, FPixel val);
void image_fillrgb(Image *src, float r, float g, float b);
void image_filla(Image *src, float a);
void image_fillz(Image *src, float z);

// I/O functions
Image *image_read(char *filename);
int image_write(Image *src, char *filename);
int image_fwrite(Image *src, char *filename);

// Color methods
void color_copy(Color *to, Color *from);
void color_set(Color *to, float r, float g, float b);
void image_setColor(Image *src, int r, int c, Color val);
int image_getColor(Image *src, int r, int c, Color *t);
int color_equals(Color *a, Color *b);

FPixel generateFPixelBasedOnSeed(int k, int variation);

int returnRandom(int lower, int upper);

#endif
