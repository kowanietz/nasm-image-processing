#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/bmp.h"

extern void invert_image(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int components);

extern void invert_image_avx2(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int components);


extern void grayscale_image(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int components);


int main() {
    unsigned char *pixels = NULL;
    unsigned int width, height;

    unsigned int err = loadbmp_decode_file("images/pepper.bmp", &pixels, &width, &height, LOADBMP_RGB);

    if (err) printf("Erro loading BMP %u\n", err);

    printf("Image Dimensions: %d x %d\n", width, height);

    grayscale_image(pixels, width, height, 3); // R, G, B -> 3 (RGBA -> 4)

    unsigned int write_err = loadbmp_encode_file("images/out/grayscale/pepper.bmp", pixels, width, height, LOADBMP_RGB);

    if (write_err) printf("Error saving BMP: %u\n", write_err);

    free(pixels);
    return 0;
}
