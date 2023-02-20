#include "bmp_operations.h"
#include <stdio.h>
#include <stdlib.h>

void copyImage(byte** copy_pixels, byte* pixels, int32 width, int32 height, int32 bytesPerPixel){
    int i = 0;
    int unpaddedRowSize = width*bytesPerPixel;
    int totalSize = unpaddedRowSize*height;
    *copy_pixels = (byte*)heap_malloc(1, totalSize);
    if(*copy_pixels == 0)
    	printf("Nije alocirana memorija.\n");

    for(i = 0; i < totalSize; i++){
        (*copy_pixels)[i] = pixels[i];
    }
}

void ReadImage(const char *fileName, byte **pixels, int32 *width, int32 *height, int32 *bytesPerPixel) {
	FILE *imageFile = fopen(fileName, "r");
	if (imageFile == NULL) {
		printf("Nije otvorena slika\n");
		return;
	}
	char temp[4];
	uint32 dataOffset;
	fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
	fread(temp, 1, 1, imageFile);
	fread(temp + 1, 1, 1, imageFile);
	fread(temp + 2, 1, 1, imageFile);
	fread(temp + 3, 1, 1, imageFile);
	dataOffset = temp[3];
	dataOffset <<= 8;
	dataOffset |= temp[2];
	dataOffset <<= 8;
	dataOffset |= temp[1];
	dataOffset <<= 8;
	dataOffset |= temp[0];

	fseek(imageFile, WIDTH_OFFSET, SEEK_SET);

	fread(temp, 1, 1, imageFile);
	fread(temp + 1, 1, 1, imageFile);
	fread(temp + 2, 1, 1, imageFile);
	fread(temp + 3, 1, 1, imageFile);
	*width = temp[3];
	*width <<= 8;
	*width |= temp[2];
	*width <<= 8;
	*width |= temp[1];
	*width <<= 8;
	*width |= temp[0];

	fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);

	fread(temp, 1, 1, imageFile);
	fread(temp + 1, 1, 1, imageFile);
	fread(temp + 2, 1, 1, imageFile);
	fread(temp + 3, 1, 1, imageFile);
	*height = temp[3];
	*height <<= 8;
	*height |= temp[2];
	*height <<= 8;
	*height |= temp[1];
	*height <<= 8;
	*height |= temp[0];
	int16 bitsPerPixel;
	fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);

	fread(temp, 1, 1, imageFile);
	fread(temp + 1, 1, 1, imageFile);
	bitsPerPixel = temp[1];
	bitsPerPixel <<= 8;
	bitsPerPixel |= temp[0];
	*bytesPerPixel = ((uint32) bitsPerPixel) / 8;

	int paddedRowSize = (4 * ((*width) / 4.0f)) * (*bytesPerPixel);
	int unpaddedRowSize = (*width) * (*bytesPerPixel);
	int totalSize = unpaddedRowSize * (*height);

	*pixels = (byte *) heap_malloc(1, totalSize);
	int i = 0;
	byte *currentRowPointer = *pixels + ((*height - 1) * unpaddedRowSize);
	for (i = 0; i < *height; i++) {
		fseek(imageFile, dataOffset + (i * paddedRowSize), SEEK_SET);
		fread(currentRowPointer, 1, unpaddedRowSize, imageFile);
		currentRowPointer -= unpaddedRowSize;
	}

	fclose(imageFile);
	printf("Slika ucitana!\n");
}
void WriteImage(const char *fileName, byte *pixels, int32 width, int32 height, int32 bytesPerPixel) {
	FILE *outputFile = fopen(fileName, "w");
	if (outputFile == NULL) {
		printf("Neuspjesno otvaranje slike\n");
		return;
	}
	//*****HEADER************//
	const char *BM = "BM";
	fwrite(&BM[0], 1, 1, outputFile);
	fwrite(&BM[1], 1, 1, outputFile);
	int paddedRowSize = (4 *( width / 4.0f)) * bytesPerPixel;
	//int paddedRowSize = 300;
	uint32 fileSize = paddedRowSize * height + HEADER_SIZE + INFO_HEADER_SIZE;
	fwrite(&fileSize, 1, 1, outputFile);
	fileSize >>= 8;
	fwrite(&fileSize, 1, 1, outputFile);
	fileSize >>= 8;
	fwrite(&fileSize, 1, 1, outputFile);
	fileSize >>= 8;
	fwrite(&fileSize, 1, 1, outputFile);

	uint32 reserved = 0x0000;
	fwrite(&reserved, 1, 1, outputFile);
	fwrite(&reserved, 1, 1, outputFile);
	fwrite(&reserved, 1, 1, outputFile);
	fwrite(&reserved, 1, 1, outputFile);

	uint32 dataOffset = HEADER_SIZE + INFO_HEADER_SIZE;
	fwrite(&dataOffset, 1, 1, outputFile);
	dataOffset >>= 8;
	fwrite(&dataOffset, 1, 1, outputFile);
	dataOffset >>= 8;
	fwrite(&dataOffset, 1, 1, outputFile);
	dataOffset >>= 8;
	fwrite(&dataOffset, 1, 1, outputFile);

	//*******INFO*HEADER******//
	uint32 infoHeaderSize = INFO_HEADER_SIZE;
	fwrite(&infoHeaderSize, 1, 1, outputFile);
	infoHeaderSize >>= 8;
	fwrite(&infoHeaderSize, 1, 1, outputFile);
	infoHeaderSize >>= 8;
	fwrite(&infoHeaderSize, 1, 1, outputFile);
	infoHeaderSize >>= 8;
	fwrite(&infoHeaderSize, 1, 1, outputFile);

	uint32 width2 = width;
	fwrite(&width2, 1, 1, outputFile);
	width2 >>= 8;
	fwrite(&width2, 1, 1, outputFile);
	width2 >>= 8;
	fwrite(&width2, 1, 1, outputFile);
	width2 >>= 8;
	fwrite(&width2, 1, 1, outputFile);

	uint32 height2 = height;
	fwrite(&height2, 1, 1, outputFile);
	height2 >>= 8;
	fwrite(&height2, 1, 1, outputFile);
	height2 >>= 8;
	fwrite(&height2, 1, 1, outputFile);
	height2 >>= 8;
	fwrite(&height2, 1, 1, outputFile);

	int16 planes = 1; // always 1
	fwrite(&planes, 1, 1, outputFile);
	planes >>= 8;
	fwrite(&planes, 1, 1, outputFile);

	int16 bitsPerPixel = bytesPerPixel * 8;
	fwrite(&bitsPerPixel, 1, 1, outputFile);
	bitsPerPixel >>= 8;
	fwrite(&bitsPerPixel, 1, 1, outputFile);

	// write compression
	uint32 compression = NO_COMPRESION;
	fwrite(&compression, 1, 1, outputFile);
	compression >>= 8;
	fwrite(&compression, 1, 1, outputFile);
	compression >>= 8;
	fwrite(&compression, 1, 1, outputFile);
	compression >>= 8;
	fwrite(&compression, 1, 1, outputFile);

	// write image size (in bytes)
	uint32 imageSize;
	uint32 resolutionX;
	uint32 resolutionY;
	imageSize = width * height * bytesPerPixel;
	resolutionX = 0;
	resolutionY = 0;
	fwrite(&imageSize, 1, 1, outputFile);
	imageSize >>= 8;
	fwrite(&imageSize, 1, 1, outputFile);
	imageSize >>= 8;
	fwrite(&imageSize, 1, 1, outputFile);
	imageSize >>= 8;
	fwrite(&imageSize, 1, 1, outputFile);

	fwrite(&resolutionX, 1, 1, outputFile);
	resolutionX >>= 8;
	fwrite(&resolutionX, 1, 1, outputFile);
	resolutionX >>= 8;
	fwrite(&resolutionX, 1, 1, outputFile);
	resolutionX >>= 8;
	fwrite(&resolutionX, 1, 1, outputFile);

	fwrite(&resolutionY, 1, 1, outputFile);
	resolutionY >>= 8;
	fwrite(&resolutionY, 1, 1, outputFile);
	resolutionY >>= 8;
	fwrite(&resolutionY, 1, 1, outputFile);
	resolutionY >>= 8;
	fwrite(&resolutionY, 1, 1, outputFile);

	uint32 colorsUsed = MAX_NUMBER_OF_COLORS;
	fwrite(&colorsUsed, 1, 1, outputFile);
	colorsUsed >>= 8;
	fwrite(&colorsUsed, 1, 1, outputFile);
	colorsUsed >>= 8;
	fwrite(&colorsUsed, 1, 1, outputFile);
	colorsUsed >>= 8;
	fwrite(&colorsUsed, 1, 1, outputFile);

	uint32 importantColors = ALL_COLORS_REQUIRED;
	fwrite(&importantColors, 1, 1, outputFile);
	importantColors >>= 8;
	fwrite(&importantColors, 1, 1, outputFile);
	importantColors >>= 8;
	fwrite(&importantColors, 1, 1, outputFile);
	importantColors >>= 8;
	fwrite(&importantColors, 1, 1, outputFile);

	int unpaddedRowSize = width * bytesPerPixel;
	int pixelOffset;
	for (int i = 0; i < height; i++) {
		// printf("Upis reda\n");
		pixelOffset = ((height - i) - 1) * unpaddedRowSize;
		fwrite(&pixels[pixelOffset], 1, paddedRowSize, outputFile);
	}
	fclose(outputFile);
	printf("Slika sacuvana!\n");
}