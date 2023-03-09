#include "image.h"
#include "embers.h"
#include "errors.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include "ctype.h"
#define LINE_SIZE (70)
#define IN_BOUNDS(x, y, Img) ((unsigned)x >= 0 &&           \
                              (unsigned)y >= 0 &&           \
                              (unsigned)x < Img -> Width && \
                              (unsigned)y < Img -> Height)

static int ReadHeader(FILE *ImgFile, unsigned int Data[3]);
static void HandleLine(char *Line, unsigned int Data[3], int *Current);

Image *ImageCreate(unsigned Width, unsigned Height)
{
    if (!Width || !Height)
        return NULL;

    Image *Ret = (Image*)malloc(sizeof(*Ret));
    if (!Ret) {
        EMBERS_ERROR(EMBERS_OUT_OF_MEMORY);
        return NULL;
    }

    Ret -> Width = Width;
    Ret -> Height = Height;
    Ret -> Pixels = (Pixel*)malloc(sizeof(*Ret -> Pixels) * Width * Height);
    if (!Ret -> Pixels) {
        EMBERS_ERROR(EMBERS_OUT_OF_MEMORY);
        free(Ret);
        return NULL;
    }

    return Ret;
}

void ImageSetPixel(Image *Img, int x, int y, Colour c)
{
    if (!Img)
        return;

    if (!IN_BOUNDS(x, y, Img))
        return;

    int Index = x + y * Img -> Width; 

    Img -> Pixels[Index][R] = c.r;
    Img -> Pixels[Index][G] = c.g;
    Img -> Pixels[Index][B] = c.b;
}

Colour ImageGetPixel(Image *Img, int x, int y) 
{
    if (!Img)
        return RGB(0, 0, 0);

    if (!IN_BOUNDS(x, y, Img))
        return RGB(0, 0, 0);

    int Index = x + y * Img -> Width;

    return RGB(Img -> Pixels[Index][R],
               Img -> Pixels[Index][G],
               Img -> Pixels[Index][B]);

}

void ImageWrite(const char *Filename, const Image *Img)
{
    if (!Filename || !Img)
        return;

    FILE *ImageFile = fopen(Filename, "w");
    if (!ImageFile) {
        EMBERS_ERROR(EMBERS_CANT_OPEN_FILE);
        return;
    }

    fprintf(ImageFile,
            "P6 %u %u 255\n",
            Img -> Width,
            Img -> Height);

    fwrite(Img -> Pixels,
           sizeof(*Img -> Pixels),
           Img -> Width * Img -> Height,
           ImageFile);

    fclose(ImageFile);
}

Image *ImageRead(const char *Filename)
{
    Image *Returned;
    FILE *ImageFile;
    unsigned HeaderData[3];
    char Buff[EMBERS_BUFFER_SIZE];
    if (!Filename)
        return NULL;

    ImageFile = fopen(Filename, "r");
    if (!ImageFile) {
        EMBERS_ERROR(EMBERS_CANT_OPEN_FILE);
        return NULL;
    }

    if (!ReadHeader(ImageFile, HeaderData)) {
        EMBERS_ERROR(EMBERS_FAILURE);
        fclose(ImageFile);
        return NULL;
    }

    if (!HeaderData[0] || !HeaderData[1] || !HeaderData[2])  {
        EMBERS_ERROR(EMBERS_FAILURE);
        fclose(ImageFile);
        return NULL;
    }

    snprintf(Buff,
             EMBERS_BUFFER_SIZE,
             "Read [%s] width=%u height=%u max_Value=%u",
             Filename,
             HeaderData[0],
             HeaderData[1],
             HeaderData[2]);

    EMBERS_LOG_INFO(Buff);

    Returned = ImageCreate(HeaderData[0], HeaderData[1]);
    if (!Returned) {
        EMBERS_ERROR(EMBERS_OUT_OF_MEMORY);
        fclose(ImageFile);
        return NULL;
    }

    fread(Returned -> Pixels,
          sizeof(*Returned -> Pixels),
          HeaderData[0] * HeaderData[1],
          ImageFile);

    fclose(ImageFile);
    return Returned;

}

void ImageFree(Image *Img)
{
    free(Img -> Pixels);
    free(Img);
}

/* We only care about 4 things seperated by white-space.                  */
/* - The magic number.                                                    */
/* - The width.                                                           */
/* - The height.                                                          */
/* - The max value of a pixel.                                            */
/* The spec says that lines can't exceed LINE_SIZE(70 bytes).             */
int ReadHeader(FILE *ImgFile, unsigned int Data[3])
{
    size_t Size = LINE_SIZE;
    char *LineBuffer = (char*)malloc(sizeof(*LineBuffer) * Size);
    int LinesRead = 0;
    int Current = 0;
    if (!LineBuffer)
        return EMBERS_FALSE;
    
    Data[0] = Data[1] = Data[2] = 0;
    getline(&LineBuffer, &Size, ImgFile);

    /* Check for magic number.                                                */
    if (LineBuffer[0] != 'P' || LineBuffer[1] != '6') {
        free(LineBuffer);
        return EMBERS_FALSE;
    }
    
    HandleLine(LineBuffer + 2, Data, &Current);
    LinesRead = 1;
    while (Current < 3 && LinesRead < 5) {
        getline(&LineBuffer, &Size, ImgFile);
        if (!LineBuffer[0]) {
            free(LineBuffer);
            return EMBERS_FALSE;
        }

        HandleLine(LineBuffer, Data, &Current);
        LinesRead += LineBuffer[0] != '#';
    }

    free(LineBuffer);
    return EMBERS_TRUE;
}

void HandleLine(char *Line, unsigned int Data[3], int *Current)
{
    for (int i = 0;
            Line[i] &&
            Line[i] != '\n' &&
            Line[i] != '#' &&
            *Current < 3; i++) {

        if (!i || isspace(Line[i])) {
            Data[*Current] = atoi(Line + i);
            if (Data[*Current])
                (*Current)++;

        }

    }
}
