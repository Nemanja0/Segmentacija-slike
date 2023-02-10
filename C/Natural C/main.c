#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "bmp_operations.h"
#include "stack.h"

clock_t start, end;
double time_spend;

const char* image = "slike/slika.bmp";
const char* grayscale_image = "obradjene_slike/Grayscale.bmp";
const char* gaussian_blur_image = "obradjene_slike/Gaussian_blur.bmp";
const char* laplacian_picture_image = "obradjene_slike/Laplacian_picture.bmp";
const char* coded_picture_image = "obradjene_slike/Coded_picture.bmp";
const char* painted_picture_image = "obradjene_slike/Painted_picture.bmp";

void returnOffsets(int32 w, int offset, int* offsets){
    offsets[0] = offset - 3*w - 3;
    offsets[1] = offset - 3*w;
    offsets[2] = offset - 3*w + 3;
    offsets[3] = offset - 3;
    offsets[4] = offset;
    offsets[5] = offset + 3;
    offsets[6] = offset + 3*w - 3;
    offsets[7] = offset + 3*w;
    offsets[8] = offset + 3*w + 3;
}

void dfs(byte* pixels, int32 w, int32 h, int offset, int code){
    int* offsets = (int*)malloc(10*sizeof(int));
    int off;
    push(offset);
    while(!isEmpty()){
        int off = pop();
        if (off < 3*w || off%(3*w) == 0 || (off+3)%(3*w) == 0 || off > 3*w*(h-1))
            continue;
        returnOffsets(w, off, offsets);
        if (!(pixels[offsets[4]] == 0x00 || pixels[offsets[4]+1] == 0x00 || pixels[offsets[4]+2] == 0x00))
            continue;
        pixels[offsets[4]] = code;
        pixels[offsets[4]+1] = code;
        pixels[offsets[4]+2] = code;
        push(offsets[1]);
        push(offsets[3]);
        push(offsets[7]);
        push(offsets[5]);
    }
    free(offsets);
}

void findBlackSpots(byte* pixels, int32 w, int32 h, int* offset){
    int* offsets = (int*)malloc(10*sizeof(int));
    *offset = -1;
    for(int i = 0; i < 3*w*h; i+=3){
        returnOffsets(w, i, offsets);
        if(pixels[offsets[4]] == 0x00 && pixels[offsets[4]+1] == 0x00 && pixels[offsets[4]+2] == 0x00){
            *offset = i;
            break;
        }
    }
    free(offsets);
}

void randomColor(byte* paint){
    for(int i = 0; i < 3; i++)
        paint[i] = rand()%254+1;
}

int main()
{
    int32 w, h, bpp;
    byte* pixels, *pixels_copy,** paint;
    int gray, blur, laplacian, offset;
    int* offsets = (int*)malloc(10*sizeof(int));
    srand(time(0));

    ReadImage(image, &pixels, &w, &h, &bpp);

    /*****************************Grayscale******************************/
    start = clock();

    for(int i = 0; i < 3*w*h; i+=3){
        gray = 0.299*pixels[i] + 0.587*pixels[i+1] + 0.114*pixels[i+2];
        pixels[i] = gray;
        pixels[i+1] = gray;
        pixels[i+2] = gray;
    }
    end = clock();
    time_spend = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Graysale time: %lf\n", time_spend);

    WriteImage(grayscale_image, pixels, w, h, bpp);
    printf("Grayscale finished!\n");

    /********************************************************************/
    /*************************Gaussian blur******************************/
    start = clock();

    copyImage(&pixels_copy, pixels, w, h, bpp);

    for(int i = 3*w; i < 3*w*(h-1); i+=3){
        if(i%(3*w) == 0 || (i+3)%(3*w) == 0){
            continue;
        }
        returnOffsets(w, i, offsets);
        blur = 0.25*pixels_copy[offsets[4]] + 0.0625*(pixels_copy[offsets[0]] + pixels_copy[offsets[2]] + pixels_copy[offsets[6]] + pixels_copy[offsets[8]]) + 0.125*(pixels_copy[offsets[1]] + pixels_copy[offsets[3]] + pixels_copy[offsets[5]] + pixels_copy[offsets[7]]);
        pixels[offsets[4]] = blur;
        pixels[offsets[4]+1] = blur;
        pixels[offsets[4]+2] = blur;
    }

    WriteImage(gaussian_blur_image, pixels, w, h, bpp);
    free(pixels_copy);
    printf("Blur finished!\n");

    /********************************************************************/
    /***************************Laplacian********************************/

    copyImage(&pixels_copy, pixels, w, h, bpp);

    for(int i = 3*w; i < 3*w*(h-1); i+=3){
        if(i%(3*w) == 0 || (i+3)%(3*w) == 0){
            continue;
        }
        returnOffsets(w, i, offsets);
        laplacian = 8*pixels_copy[offsets[4]] - (pixels_copy[offsets[0]] + pixels_copy[offsets[2]] + pixels_copy[offsets[6]] + pixels_copy[offsets[8]] + pixels_copy[offsets[1]] + pixels_copy[offsets[3]] + pixels_copy[offsets[5]] + pixels_copy[offsets[7]]);
        pixels[offsets[4]] = laplacian;
        pixels[offsets[4]+1] = laplacian;
        pixels[offsets[4]+2] = laplacian;
    }
    end = clock();
    time_spend = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Laplasian time: %lf\n", time_spend);

    WriteImage(laplacian_picture_image, pixels, w, h, bpp);
    free(pixels_copy);
    printf("Laplacian finished!\n");

    /********************************************************************/
    /*******************************Coding*******************************/
    start = clock();

    int code = 1;
    while(1){
        findBlackSpots(pixels, w, h, &offset);
        if(offset == -1){
            break;
        }
        dfs(pixels, w, h, offset, code);
        code++;
    }
    end = clock();
    time_spend = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Coding time: %lf\n", time_spend);

    WriteImage(coded_picture_image, pixels, w, h, bpp);
    printf("Coding finished!\n");

    /********************************************************************/
    /****************************Painting********************************/
    start = clock();

    paint = (byte**)malloc(code*sizeof(byte*));
    for(int i = 0; i < code; i++){
        paint[i] = (byte*)malloc(4*sizeof(byte));
        randomColor(paint[i]);
    }
    for(int i = 0; i < 3*w*h; i+=3){
        if(pixels[i] >= code && pixels[i+1] >= code && pixels[i+2] >= code)
            continue;
        pixels[i] = paint[pixels[i]][0];
        pixels[i+1] = paint[pixels[i+1]][1];
        pixels[i+2] = paint[pixels[i+2]][2];
    }
    end = clock();
    time_spend = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Painting time: %lf\n", time_spend);

    WriteImage(painted_picture_image, pixels, w, h, bpp);
    printf("Painting finished!\n");

    /********************************************************************/

    free(paint);
    free(offsets);
    return 0;
}
