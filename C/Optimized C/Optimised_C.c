/*****************************************************************************
 * Optimised_C.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "Optimised_C.h"
#include <stdio.h>
#include <def21489.h>
#include <sru21489.h>
#include <SYSREG.h>
#include <cycle_count.h>
#include <stdlib.h>
#include <time.h>
#include "bmp_operations.h"
#include "stack.h"

cycle_t start_count, final_count;
int* offsets;

const char* image = "slike/slika6.bmp";
const char* grayscale_image = "obradjene_slike/Grayscale.bmp";
const char* gaussian_blur_image = "obradjene_slike/Gaussian_blur.bmp";
const char* laplacian_picture_image = "obradjene_slike/Laplacian_picture.bmp";
const char* coded_picture_image = "obradjene_slike/Coded_picture.bmp";
const char* painted_picture_image = "obradjene_slike/Painted_picture.bmp";

void InitSRU(void)
{
	//** LED01**//
	SRU(HIGH,DPI_PBEN06_I);
	SRU(FLAG4_O,DPI_PB06_I);
	//** LED02**//
	SRU(HIGH,DPI_PBEN13_I);
	SRU(FLAG5_O,DPI_PB13_I);
	//** LED03**//
	SRU(HIGH,DPI_PBEN14_I);
	SRU(FLAG6_O,DPI_PB14_I);
	//** LED04**//
	SRU(HIGH,DAI_PBEN03_I);
	SRU(HIGH,DAI_PB03_I);
	//** LED05**//
	SRU(HIGH,DAI_PBEN04_I);
	SRU(HIGH,DAI_PB04_I);
	//** LED06**//
	SRU(HIGH,DAI_PBEN15_I);
	SRU(HIGH,DAI_PB15_I);
	//** LED07**//
	SRU(HIGH,DAI_PBEN16_I);
	SRU(HIGH,DAI_PB16_I);
	//** LED08**//
	SRU(HIGH,DAI_PBEN17_I);
	SRU(HIGH,DAI_PB17_I);
	//Setting flag pins as outputs
	sysreg_bit_set(sysreg_FLAGS, (FLG4O|FLG5O|FLG6O) );
	//Setting HIGH to flag pins
	sysreg_bit_set(sysreg_FLAGS, (FLG4|FLG5|FLG6) );
}

void returnOffsets(int32 w, int offset, int* offsets){
    offsets[0] = offset - w - 3;
    offsets[1] = offset - w;
    offsets[2] = offset - w + 3;
    offsets[3] = offset - 3;
    offsets[4] = offset;
    offsets[5] = offset + 3;
    offsets[6] = offset + w - 3;
    offsets[7] = offset + w;
    offsets[8] = offset + w + 3;
}

void dfs(byte* pixels, int32 w, int32 h, int offset, int code){
    int empty, off, pom1 = 3*w, pom2 = 3*w*(h-1);
    push(offset);
    for(empty = 0; !empty; isEmpty(&empty)){
        pop(&off);
        if (!(pixels[off] == 0x00) || off < pom1 || off%(pom1) == 0 || (off+3)%(pom1) == 0 || off > pom2)
            continue;
        pixels[off] = code;
        push(off+3);
        push(off+pom1);
        push(off-pom1);
        push(off-3);
    }
}

void findBlackSpots(byte* pixels, int32 w, int32 h, int* offset){
    *offset = -1;
    for(int i = 0; i < 3*w*h; i+=3){
        if(pixels[i] == 0x00){
            *offset = i;
            break;
        }
    }
}

void randomColor(unsigned char* paint){
    for(int i = 0; i < 3; i++)
        paint[i] = rand()%254+1;
}

/** 
 * If you want to use command program arguments, then place them in the following string. 
 */
char __argv_string[] = "";

int main(int argc, char *argv[])
{
	/**
	 * Initialize managed drivers and/or services that have been added to 
	 * the project.
	 * @return zero on success 
	 */
	adi_initComponents();
	InitSRU();
	//turn off LEDs
	sysreg_bit_clr(sysreg_FLAGS, FLG4);
	sysreg_bit_clr(sysreg_FLAGS, FLG5);
	sysreg_bit_clr(sysreg_FLAGS, FLG6);
	SRU(LOW,DAI_PB03_I);
	SRU(LOW,DAI_PB04_I);
	SRU(LOW,DAI_PB15_I);
	SRU(LOW,DAI_PB16_I);
	SRU(LOW,DAI_PB17_I);

	int32 w, h, bpp;
	byte* pixels, *pixels_copy,** paint;
	int gray, blur, laplacian, offset, code = 1;
	offsets = (int*)heap_malloc(mem_id, 10*sizeof(int));
	START_CYCLE_COUNT(start_count);
	ReadImage(image, &pixels, &w, &h, &bpp);
	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("Ucitavanje: ", final_count);
	sysreg_bit_set(sysreg_FLAGS, FLG4);
	START_CYCLE_COUNT(start_count);

	/*****************************Grayscale******************************/

	for(int i = 0; i < 3*w*h; i+=3){
		gray = 0.299*pixels[i] + 0.587*pixels[i+1] + 0.114*pixels[i+2];
		pixels[i] = gray;
		pixels[i+1] = gray;
		pixels[i+2] = gray;
	}
	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("Grayscale: ", final_count);
	//WriteImage(grayscale_image, pixels, w, h, bpp);
	printf("Grayscale finished!\n");
	sysreg_bit_set(sysreg_FLAGS, FLG5);

	/********************************************************************/
	/*************************Gaussian blur******************************/

	copyImage(&pixels_copy, pixels, w, h, bpp);

	START_CYCLE_COUNT(start_count);
	for(int i = 3*w; i < 3*w*(h-1); i+=3){
		if(i%(3*w) == 0 || (i+3)%(3*w) == 0){
			continue;
		}
		returnOffsets(3*w, i, offsets);
		blur = 0.25*pixels_copy[offsets[4]] + 0.0625*(pixels_copy[offsets[0]] + pixels_copy[offsets[2]] + pixels_copy[offsets[6]] + pixels_copy[offsets[8]]) + 0.125*(pixels_copy[offsets[1]] + pixels_copy[offsets[3]] + pixels_copy[offsets[5]] + pixels_copy[offsets[7]]);
		pixels[offsets[4]] = blur;
		pixels[offsets[4]+1] = blur;
		pixels[offsets[4]+2] = blur;
	}
	//WriteImage(gaussian_blur_image, pixels, w, h, bpp);
	heap_free(mem_id, pixels_copy);
	printf("Blur finished!\n");
	sysreg_bit_set(sysreg_FLAGS, FLG6);

	/********************************************************************/
	/***************************Laplacian********************************/

	copyImage(&pixels_copy, pixels, w, h, bpp);

	for(int i = 3*w; i < 3*w*(h-1); i+=3){
		if(i%(3*w) == 0 || (i+3)%(3*w) == 0){
			continue;
		}
		returnOffsets(3*w, i, offsets);
		laplacian = 8*pixels_copy[offsets[4]] - (pixels_copy[offsets[0]] + pixels_copy[offsets[2]] + pixels_copy[offsets[6]] + pixels_copy[offsets[8]] + pixels_copy[offsets[1]] + pixels_copy[offsets[3]] + pixels_copy[offsets[5]] + pixels_copy[offsets[7]]);
		pixels[offsets[4]] = laplacian;
		pixels[offsets[4]+1] = laplacian;
		pixels[offsets[4]+2] = laplacian;
	}

	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("Detekcija ivica: ", final_count);
	//WriteImage(laplacian_picture_image, pixels, w, h, bpp);
	heap_free(mem_id, pixels_copy);
	printf("Laplacian finished!\n");
	SRU(HIGH,DAI_PB03_I);


	/********************************************************************/
	/*******************************Coding*******************************/

	START_CYCLE_COUNT(start_count);
	for(;;){
		findBlackSpots(pixels, w, h, &offset);
		if(offset == -1){
			break;
		}
		dfs(pixels, w, h, offset, code);
		code++;
	}
	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("Kodovanje: ", final_count);
	//WriteImage(coded_picture_image, pixels, w, h, bpp);
	printf("Coding finished!\n");
	SRU(HIGH,DAI_PB04_I);


	/********************************************************************/
	/****************************Painting********************************/

	START_CYCLE_COUNT(start_count);
	paint = (byte**)heap_malloc(mem_id,code*sizeof(byte*));
	for(int i = 0; i < code; i++){
		paint[i] = (byte*)heap_malloc(mem_id,4*sizeof(byte));
		randomColor(paint[i]);
	}
	for(int i = 0; i < 3*w*h; i+=3){
		if(pixels[i] >= code && pixels[i+1] >= code && pixels[i+2] >= code)
			continue;
		pixels[i+2] = paint[pixels[i]][2];
		pixels[i+1] = paint[pixels[i]][1];
		pixels[i] = paint[pixels[i]][0];
	}
	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("Bojenje: ", final_count);
	WriteImage(painted_picture_image, pixels, w, h, bpp);
	printf("Painting finished!\n");
	SRU(HIGH,DAI_PB15_I);

	/********************************************************************/
	for(int i = 0; i < code; i++)
		heap_free(mem_id, paint[i]);
	heap_free(mem_id, paint);
	heap_free(mem_id, offsets);

	return 0;
}
