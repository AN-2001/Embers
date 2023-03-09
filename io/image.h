/******************************************************************************\
*  image.h                                                                     *
*                                                                              *
*  Handles everything about images.                                            *
*  Only PPM files are supported and only P6 type files.                        *
*                                                                              *
*              Written by Abed Na'ran                          February 2023   *
*                                                                              *
\******************************************************************************/
#ifndef IMAGE_H
#define IMAGE_H
#include "config.h"

#define R (0)
#define G (1)
#define B (2)
#define RGB(x, y, z) ((Colour){.r = x, .g = y, .b = z})

/* a pixel is an array of 3 chars so they stay contigous without padding.     */
typedef unsigned char Pixel[3];

/* We don't care about padding for a colour, and we want to return and give   */
/* it to functions.                                                           */
typedef struct Colour_t {
    unsigned char r, g, b;
} Colour;


typedef struct Image_t {
    Pixel *Pixels;
    unsigned Width, Height;
} Image;

/******************************************************************************\
* ImageCreate                                                                  *
*                                                                              *
*  Create a new image.                                                         *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -Width: The width.                                                          *
*  -Height: The height.                                                        *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Image*: The newly created image.                                           *
*                                                                              *
\******************************************************************************/
Image *ImageCreate(unsigned Width, unsigned Height);

/******************************************************************************\
* ImageSetPixel                                                                *
*                                                                              *
*  Set the pixel colour at (x, y).                                             *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -Img: The image.                                                            *
*  -x: The x value.                                                            *
*  -y: The y value.                                                            *
*  -c: The new colour.                                                         *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -void.                                                                      *
*                                                                              *
\******************************************************************************/
void ImageSetPixel(Image *Img, int x, int y, Colour c);

/******************************************************************************\
* ImageGetPixel                                                                *
*                                                                              *
*  Get the pixel colour at (x, y).                                             *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -Img: The image.                                                            *
*  -x: The x value.                                                            *
*  -y: The y value.                                                            *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Colour: The colour at the pixel.                                           *
*                                                                              *
\******************************************************************************/
Colour ImageGetPixel(Image *Img, int x, int y);

/******************************************************************************\
* ImageRead                                                                    *
*                                                                              *
*  Read an image from disk, only ppm is supported.                             *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -Filename: The name of the image file.                                      *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -Image*: The image.                                                         *
*                                                                              *
\******************************************************************************/
Image *ImageRead(const char *Filename);

/******************************************************************************\
* ImageWrite                                                                   *
*                                                                              *
*  Write the image out to disk, only ppm is supported.                         *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -Filename: The name of the image file.                                      *
*  -Img: The image.                                                            *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -void.                                                                      *
*                                                                              *
\******************************************************************************/
void ImageWrite(const char *Filename, const Image *Img);

/******************************************************************************\
* ImageFree                                                                    *
*                                                                              *
*  Frees the image's memory.                                                   *
*                                                                              *
* Parameters                                                                   *
*                                                                              *
*  -Img: The image.                                                            *
*                                                                              *
* Return                                                                       *
*                                                                              *
*  -void.                                                                      *
*                                                                              *
\******************************************************************************/
void ImageFree(Image *Img);

#endif /* IMAGE_H */
