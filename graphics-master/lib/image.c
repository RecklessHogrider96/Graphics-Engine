/*
  CS5310 - Roshan Bellary
  10-20-2021 (Fall 2021)

  Image Base.
*/

#include "image.h"
#include "ppmIO.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* constructors & destructors */

// Allocates an Image structure and initializesthe top level fields to appropriate values.
// Allocates space for an image of the specified size, unless
// either rows or cols is 0.
// Returns a pointer to the allocated Image structure. Returns a NULL pointer if
// the operation fails.
Image *image_create(int rows, int cols)
{
	if ((rows == 0) || (cols == 0))
	{
		printf("WARNING: image_create >> rows or cols is Zero\n");
	}
	Image *image = malloc(sizeof(Image));
	if (NULL == image)
	{
		return NULL;
	}
	image_init(image);
	image_alloc(image, rows, cols);
	return image;
}


// De-allocates image fpixel and frees the Image structure.
void image_free(Image *src)
{
	if (NULL != src)
	{
		if (NULL != src->fpixel)
		{
			int i;
			for (i = 0; i < src->rows; i++)
			{
				free(src->fpixel[i]); // free all the fpixel cols
			}
			free(src->fpixel); // free the fpixel rows
			src->fpixel = NULL;
			free(src);
			src = NULL;
		}
		else
		{
			free(src);
			src = NULL;
		}
	}
	else
	{
		printf("WARNING: image_free >> src is NULL\n");
	}
}

// Given an uninitialized Image structure, sets the rows and cols
// fields to zero and the fpixel field to NULL.
void image_init(Image *src)
{
	if (NULL != src)
	{
		src->fpixel = NULL;
		src->rows = 0;
		src->cols = 0;
	}
	else
	{
		printf("ERROR: cannot initialize image >> image pointer is NULL\n");
	}
}

// Allocates space for the image fpixel given rows and columns and initializes the image fpixel to appropriate values,
// such as 0.0 for RGBA and 1.0 for Z.
// Returns 0 if the operation is successful. Returns a non-zero value if the operation fails.
// This function does free existing memory if rows and cols are both non-zero.
int image_alloc(Image *src, int rows, int cols)
{
	if (NULL != src)
	{
		if (rows == 0 || cols == 0)
		{
			printf("WARNING: image_alloc >> rows or cols is Zero\n");
		}
		else
		{ // if rows and cols are non-zero, deallocate the src first
			image_dealloc(src);
		}

		src->fpixel = malloc(sizeof(FPixel *) * rows); // creating "rows". an array of FPixel pointers
		if (NULL == src->fpixel)
		{
			return -1;
		}
		int k;
		for (k = 0; k < rows; k++)
		{
			src->fpixel[k] = malloc(sizeof(FPixel) * cols); // creating "cols". arrays of FPixels.
			if (NULL == src->fpixel[k])
			{
				return -1;
			}
		}
		src->rows = rows;
		src->cols = cols;

		image_reset(src);	   // resets all FPixels to black, a=1.0, z=1.0.
		image_filla(src, 0.0); // sets the alpha value of each FPixel to 0.0
		return 0;
	}
	else
	{
		printf("ERROR: image_alloc >> image pointer in NULL\n");
		return -1;
	}
}

// De-allocates image fpixel and resets the Image structure
// fields. The function does not free the Image structure.
void image_dealloc(Image *src)
{
	if (NULL != src)
	{
		if (NULL != src->fpixel)
		{
			int i;
			for (i = 0; i < src->rows; i++)
			{
				free(src->fpixel[i]); // free all the fpixel cols
			}
			free(src->fpixel); // free the fpixel rows
			src->fpixel = NULL;
			src->rows = 0;
			src->cols = 0;
		}
	}
	else
	{
		printf("WARNING: image_dealloc >> image pointer is NULL\n");
	}
}

/*I/O functions */

// Reads a PPM image from the given filename. 
// Initializes the alpha channel to 1.0 and the z channel to 1.0. 
// Returns a NULL pointer if the operation fails.
// *** IT will free the memory of readPPM() after copying the fpixel into the new Image structure
Image *image_read(char *filename)
{
	if (strlen(filename) == 0)
	{
		printf("ERROR: could not read image >> filename is empty\n");
		return NULL;
	}
	else
	{
		int rows;
		int cols;
		int color;
		Pixel *pixel = readPPM(&rows, &cols, &color, filename); //reading the .ppm file

		Image *image = image_create(rows, cols); // creating an image using our image structure.
		image_alloc(image, rows, cols);			 // allocate memory for the FPixels and initialize them
		int i;
		int j;
		int p;
		for (i = 0, p = 0; i < rows; i++)
		{ // transfering the rgb values from ppm file Pixel array to the new FPixels of Image structure
			for (j = 0; j < cols; j++, p++)
			{
				image->fpixel[i][j].rgb[0] = (float)(pixel[p].r / 255.0);
				image->fpixel[i][j].rgb[1] = (float)(pixel[p].g / 255.0);
				image->fpixel[i][j].rgb[2] = (float)(pixel[p].b / 255.0);
				image->fpixel[i][j].a = 1.0;
				image->fpixel[i][j].z = 1.0;
			}
		}
		// free the ppm array
		free(pixel);
		pixel = NULL;

		//final checkpoint and return
		if (NULL != image)
		{
			return image;
		}
		else
		{
			return NULL;
		}
	}
}

// Writes a PPM image to the given filename.
// Returns 0 on success.
// This function DOES NOT free the memory of the Image src.
int image_write(Image *src, char *filename)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		int imagesize = src->rows * src->cols;			  // number of pixels (image size)
		Pixel *image = malloc(sizeof(Pixel) * imagesize); //allocate memory for the PPM array that will be written later to a file
		if (NULL == image)
		{
			return -1;
		} // checking the malloc

		int i;
		int j;
		int k;
		for (i = 0, k = 0; i < src->rows; i++)
		{
			for (j = 0; j < src->cols; j++, k++)
			{
				image[k].r = (unsigned char)(src->fpixel[i][j].rgb[0] * 255);
				image[k].g = (unsigned char)(src->fpixel[i][j].rgb[1] * 255);
				image[k].b = (unsigned char)(src->fpixel[i][j].rgb[2] * 255);
			}
		}
		writePPM(image, src->rows, src->cols, 255, filename);
		free(image);
		return 0;
	}
	else
	{
		return -1;
	}
}

// Writes a PPM image to the given filename.
// The format should be included in the filename. e.g. "../images/test.jpg"
// Returns 0 on success.
// This function DOES NOT free the memory of the Image src.
int image_fwrite(Image *src, char *filename)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		int imagesize = src->rows * src->cols;			  // number of pixels (image size)
		Pixel *image = malloc(sizeof(Pixel) * imagesize); //allocate memory for the PPM array that will be written later to a file
		if (NULL == image)
		{
			return -1;
		} // checking the malloc

		int i;
		int j;
		int k;
		for (i = 0, k = 0; i < src->rows; i++)
		{
			for (j = 0; j < src->cols; j++, k++)
			{
				image[k].r = (unsigned char)(src->fpixel[i][j].rgb[0] * 255);
				image[k].g = (unsigned char)(src->fpixel[i][j].rgb[1] * 255);
				image[k].b = (unsigned char)(src->fpixel[i][j].rgb[2] * 255);
			}
		}
		// write the file using writePPM and then convert it using system call
		writePPM(image, src->rows, src->cols, 255, filename);
		free(image);
		if (NULL != filename)
		{
			char command[200] = "convert ";
			strcat(command, filename);
			strcat(command, " ");
			strcat(command, filename);
			int s = system(command);
			if (0 != s)
			{
				printf("ERROR: image_fwrite >> format conversion failed");
				return -1;
			}
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

/* Access functions */

// Returns the FPixel at (r, c).
FPixel image_getf(Image *src, int r, int c)
{
	return src->fpixel[r][c];
}

// Returns the value of band b at pixel (r, c).
float image_getc(Image *src, int r, int c, int b)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		switch (b)
		{
		case 0:
			return src->fpixel[r][c].rgb[0];
		case 1:
			return src->fpixel[r][c].rgb[1];
		case 2:
			return src->fpixel[r][c].rgb[2];
		default:
			printf("ERROR: invalid band request: b = %d.\n", b);
			return -1.0;
		}
	}
	else
	{
		printf("ERROR: Image src or src->fpixel is NULL");
		return -1.0;
	}
}

// Returns the alpha value at pixel (r, c).
float image_geta(Image *src, int r, int c)
{
	return src->fpixel[r][c].a;
}

// Returns the depth value at pixel (r, c).
float image_getz(Image *src, int r, int c)
{
	return src->fpixel[r][c].z;
}

// Sets the values of pixel (r,c) to the FPixel val.
void image_setf(Image *src, int r, int c, FPixel val)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		src->fpixel[r][c].rgb[0] = val.rgb[0];
		src->fpixel[r][c].rgb[1] = val.rgb[1];
		src->fpixel[r][c].rgb[2] = val.rgb[2];
		src->fpixel[r][c].a = val.a;
		src->fpixel[r][c].z = val.z;
	}
}

// Sets the value of pixel (r, c) band b to val.
void image_setc(Image *src, int r, int c, int b, float val)
{
	if ((NULL != src) || (NULL != src->fpixel))
	{
		switch (b)
		{
		case 0:
			src->fpixel[r][c].rgb[0] = val;
			break;
		case 1:
			src->fpixel[r][c].rgb[1] = val;
			break;
		case 2:
			src->fpixel[r][c].rgb[2] = val;
			break;
		}
	}
	else
	{
		printf("ERROR: can't set the color >> src or src->fpixel is NULL\n");
		exit(-1);
	}
}

// Sets the RGB value of pixel (r,c) to the RGB value of Color c.
void image_setColor(Image *src, int r, int c, Color clr)
{
	if ((NULL == src) && (NULL == src->fpixel))
	{
		printf("ERROR: image_setColor >> src or src->fpixel is NULL");
		exit(-1);
	}
	if (r < 0 || r > src->rows - 1 || c < 0 || c > src->cols - 1)
	{
		return;
	}
	src->fpixel[r][c].rgb[0] = clr.c[0];
	src->fpixel[r][c].rgb[1] = clr.c[1];
	src->fpixel[r][c].rgb[2] = clr.c[2];
}

// Sets the alpha value of pixel (r, c) to val.
void image_seta(Image *src, int r, int c, float val)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		src->fpixel[r][c].a = val;
	}
}

// Sets the depth value of pixel (r, c) to val.
void image_setz(Image *src, int r, int c, float val)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		src->fpixel[r][c].z = val;
	}
}

/* Utility functions */

// Resets every pixel to a default value (e.g. Black, alpha value of 1.0, z value of 1.0).
void image_reset(Image *src)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		int i;
		int j;
		for (i = 0; i < src->rows; i++)
		{
			for (j = 0; j < src->cols; j++)
			{
				src->fpixel[i][j].rgb[0] = 0.0;
				src->fpixel[i][j].rgb[1] = 0.0;
				src->fpixel[i][j].rgb[2] = 0.0;
				src->fpixel[i][j].a = 1.0;
				src->fpixel[i][j].z = 1.0;
			}
		}
	}
}

// Sets every FPixel to the given value.
void image_fill(Image *src, FPixel val)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		int i;
		int j;
		for (i = 0; i < src->rows; i++)
		{
			for (j = 0; j < src->cols; j++)
			{
				src->fpixel[i][j].rgb[0] = val.rgb[0];
				src->fpixel[i][j].rgb[1] = val.rgb[1];
				src->fpixel[i][j].rgb[2] = val.rgb[2];
				src->fpixel[i][j].a = val.a;
				src->fpixel[i][j].z = val.z;
			}
		}
	}
}

// Sets the (r,g,b) value of each pixel to the given color.
void image_fillrgb(Image *src, float r, float g, float b)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		int i;
		int j;
		for (i = 0; i < src->rows; i++)
		{
			for (j = 0; j < src->cols; j++)
			{
				src->fpixel[i][j].rgb[0] = r;
				src->fpixel[i][j].rgb[1] = g;
				src->fpixel[i][j].rgb[2] = b;
			}
		}
	}
}

// Sets the alpha value of each pixel to the given value.
void image_filla(Image *src, float a)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		int i;
		int j;
		for (i = 0; i < src->rows; i++)
		{
			for (j = 0; j < src->cols; j++)
			{
				src->fpixel[i][j].a = a;
			}
		}
	}
}

// Sets the z value of each pixel to the given value.
void image_fillz(Image *src, float z)
{
	if ((NULL != src) && (NULL != src->fpixel))
	{
		int i;
		int j;
		for (i = 0; i < src->rows; i++)
		{
			for (j = 0; j < src->cols; j++)
			{
				src->fpixel[i][j].z = z;
			}
		}
	}
}

// Return 1 if same colors, else return 0.
int color_equals(Color *a, Color *b)
{
	int res = 1;
	for (int i = 0; i < 3; i++)
	{
		if (a->c[i] != b->c[i])
		{
			res = 0;
			break;
		}
	}
	return res;
}

// Set color.
void color_set(Color *to, float r, float g, float b)
{
	to->c[0] = r;
	to->c[1] = g;
	to->c[2] = b;
}

void color_copy(Color *to, Color *from)
{
	*to = *from;
}

// Return Color.
int image_getColor(Image *src, int r, int c, Color *t)
{
	// Check bounds
	if ((r >= 0 && r < src->rows) && (c >= 0 && c < src->cols))
	{
		for (int i = 0; i < 3; i++)
		{
			t->c[i] = src->fpixel[r][c].rgb[i];
		}
	}
	else
	{
		return 0;
	}

	return 1;
}

FPixel generateFPixelBasedOnSeed(int k, int variation)
{
	FPixel p = {{0, 0, 0}, 1, 1};
	//printf("k: %d; Variation: %d \n", k, variation);
	switch (variation)
	{
	case 0:
		p.rgb[0] = (k * k) / 255;
		p.rgb[1] = (k) / 255;
		p.rgb[2] = (k * k / 2) / 255;
		break;

	case 1:
		p.rgb[0] = (k * k / 2) / 255;
		p.rgb[1] = (k) / 255;
		p.rgb[2] = (k * k) / 255;
		break;

	case 2:
		p.rgb[0] = (k * sin(k)) / 255;
		p.rgb[1] = (k) / 255;
		p.rgb[2] = (1 / k) / 255;
		break;

	default:
		p.rgb[0] = (k) / 255;
		p.rgb[1] = (k) / 255;
		p.rgb[2] = (k) / 255;
		break;
	}
	return p;
}

// Generates and return random number in range [lower, upper]
int returnRandom(int lower, int upper)
{
	int num = (rand() % (upper - lower + 1)) + lower;
	return num;
}
