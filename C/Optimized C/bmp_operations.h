#ifndef BMP_OPERATIONS_H_
#define BMP_OPERATIONS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define DATA_OFFSET_OFFSET      0x000A
#define WIDTH_OFFSET            0x0012
#define HEIGHT_OFFSET           0x0016
#define BITS_PER_PIXEL_OFFSET   0x001C
#define HEADER_SIZE             14
#define INFO_HEADER_SIZE        40
#define NO_COMPRESION           0
#define MAX_NUMBER_OF_COLORS    0
#define GRAY_COLOR              256
#define ALL_COLORS_REQUIRED     0

typedef unsigned int int32;
typedef unsigned int uint32;
typedef short int16;
typedef unsigned char byte;

typedef struct{
    int x, y;
} Cords;

void ReadImage(const char *fileName, byte **pixels, int32 *width, int32 *height, int32 *bytesPerPixel);
void WriteImage(const char *fileName, byte *pixels, int32 width, int32 height, int32 bytesPerPixel);
void copyImage(byte** copy_pixels, byte* pixels, int32 width, int32 height, int32 bytesPerPixel);

#endif
