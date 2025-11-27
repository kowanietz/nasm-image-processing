#include "../include/bmp.h"
#include "../include/filters.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

extern void invert_image(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int components);

extern void invert_image_avx2(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int components);


extern void grayscale_image(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int components);


extern void brightness_image(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int components,
                             signed int brightness);


unsigned int filter_invert(const bool avx2, const char *path, const char *out_path) {
    unsigned char *pixels = NULL;
    unsigned int width, height;

    unsigned int err = loadbmp_decode_file(path, &pixels, &width, &height, LOADBMP_RGB);
    if (err) {
        printf("Erro loading BMP %u\n", err);
        return err;
    }

    if (avx2 == 0) {
        invert_image(pixels, width, height, LOADBMP_RGB);
    } else {
        invert_image_avx2(pixels, width, height, LOADBMP_RGB);
    }

    unsigned int write_err = loadbmp_encode_file(out_path, pixels, width, height, LOADBMP_RGB);
    if (write_err) {
        printf("Error saving BMP: %u\n", write_err);
        return err;
    }

    free(pixels);

    return 0;
}

unsigned int filter_grayscale(const char *path, const char *out_path) {
    unsigned char *pixels = NULL;
    unsigned int width, height;

    unsigned int err = loadbmp_decode_file(path, &pixels, &width, &height, LOADBMP_RGB);
    if (err) {
        printf("Erro loading BMP %u\n", err);
        return err;
    }

    grayscale_image(pixels, width, height, LOADBMP_RGB);

    unsigned int write_err = loadbmp_encode_file(out_path, pixels, width, height, LOADBMP_RGB);
    if (write_err) {
        printf("Error saving BMP: %u\n", write_err);
        return err;
    }

    free(pixels);

    return 0;
}

unsigned int filter_brightness(const char *path, const char *out_path, const signed int brightness) {
    unsigned char *pixels = NULL;
    unsigned int width, height;

    unsigned int err = loadbmp_decode_file(path, &pixels, &width, &height, LOADBMP_RGB);
    if (err) {
        printf("Erro loading BMP %u\n", err);
        return err;
    }

    brightness_image(pixels, width, height, LOADBMP_RGB, brightness);

    unsigned int write_err = loadbmp_encode_file(out_path, pixels, width, height, LOADBMP_RGB);
    if (write_err) {
        printf("Error saving BMP: %u\n", write_err);
        return err;
    }

    free(pixels);

    return 0;
}
