// THIS FILE AHS BEEN MODIFIED FROM ITS ORIGINAL VERSION
//
// Author: Christian Vallentin <vallentin.source@gmail.com>
// Website: http://vallentin.dev
// Repository: https://github.com/vallentin/LoadBMP
//
// Date Created: January 03, 2014
// Last Modified: August 13, 2016
//
// Version: 1.1.0

// Copyright (c) 2014-2016 Christian Vallentin <vallentin.source@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.

// Include loadbmp.h as following
// to create the implementation file.
//
// #define LOADBMP_IMPLEMENTATION
// #include "loadbmp.h"

#ifndef LOADBMP_H
#define LOADBMP_H

#ifdef __cplusplus
extern "C" {

#endif


// Errors
#define LOADBMP_NO_ERROR 0

#define LOADBMP_OUT_OF_MEMORY 1

#define LOADBMP_FILE_NOT_FOUND 2
#define LOADBMP_FILE_OPERATION 3

#define LOADBMP_INVALID_FILE_FORMAT 4

#define LOADBMP_INVALID_SIGNATURE 5
#define LOADBMP_INVALID_BITS_PER_PIXEL 6


// Components
#define LOADBMP_RGB  3
#define LOADBMP_RGBA 4


#ifdef LOADBMP_IMPLEMENTATION
#	define LOADBMP_API
#else
#	define LOADBMP_API extern
#endif


// LoadBMP uses raw buffers and support RGB and RGBA formats.
// The order is RGBRGBRGB... or RGBARGBARGBA..., from top left
// to bottom right, without any padding.


LOADBMP_API unsigned int loadbmp_decode_file(
    const char *filename, unsigned char **imageData, unsigned int *width, unsigned int *height,
    unsigned int components);

LOADBMP_API unsigned int loadbmp_encode_file(
    const char *filename, const unsigned char *imageData, unsigned int width, unsigned int height,
    unsigned int components);


#ifdef LOADBMP_IMPLEMENTATION

// Disable Microsoft Visual C++ compiler security warnings for fopen, strcpy, etc being unsafe
#if defined(_MSC_VER) && (_MSC_VER >= 1310)
#	pragma warning(disable: 4996)
#endif

#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* memset(), memcpy() */
#include <stdio.h> /* fopen(), fwrite(), fread(), fclose() */

unsigned int loadbmp_decode_file(
    const char *filename,
    unsigned char **imageData,
    unsigned int *width,
    unsigned int *height,
    unsigned int components
) {
    FILE *f = fopen(filename, "rb");
    if (!f) return LOADBMP_FILE_NOT_FOUND;

    unsigned char file_header[14];
    unsigned char info_header[40];

    if (fread(file_header, sizeof(file_header), 1, f) != 1 ||
        fread(info_header, sizeof(info_header), 1, f) != 1) {
        fclose(f);
        return LOADBMP_INVALID_FILE_FORMAT;
    }

    if (file_header[0] != 'B' || file_header[1] != 'M') {
        fclose(f);
        return LOADBMP_INVALID_SIGNATURE;
    }

    if (info_header[14] != 24 && info_header[14] != 32) {
        fclose(f);
        return LOADBMP_INVALID_BITS_PER_PIXEL;
    }

    unsigned int w = info_header[4] + (info_header[5]<<8) + (info_header[6]<<16) + (info_header[7]<<24);
    unsigned int h = info_header[8] + (info_header[9]<<8) + (info_header[10]<<16) + (info_header[11]<<24);
    unsigned int offset = file_header[10] + (file_header[11]<<8) + (file_header[12]<<16) + (file_header[13]<<24);

    unsigned int row_bytes = w * 3;
    unsigned int padding = (4 - (row_bytes % 4)) % 4;

    unsigned char *data = (unsigned char*) malloc(w * h * components);
    if (!data) { fclose(f); return LOADBMP_OUT_OF_MEMORY; }

    fseek(f, offset, SEEK_SET);

    for (int y = h - 1; y >= 0; y--) { // BMP bottom-up
        unsigned char *row_ptr = data + y * w * components;
        if (fread(row_ptr, 3, w, f) != w) {
            free(data); fclose(f); return LOADBMP_INVALID_FILE_FORMAT;
        }

        // Convert BGR -> RGB
        for (unsigned int x = 0; x < w; x++) {
            unsigned char *px = row_ptr + x * components;
            unsigned char tmp = px[0]; px[0] = px[2]; px[2] = tmp;
            if (components == LOADBMP_RGBA) px[3] = 255;
        }

        fseek(f, padding, SEEK_CUR);
    }

    fclose(f);
    *imageData = data;
    *width = w;
    *height = h;
    return LOADBMP_NO_ERROR;
}

unsigned int loadbmp_encode_file(
    const char *filename,
    const unsigned char *imageData,
    unsigned int width,
    unsigned int height,
    unsigned int components
) {
    FILE *f = fopen(filename, "wb");
    if (!f) return LOADBMP_FILE_OPERATION;

    unsigned int row_bytes = width * 3;
    unsigned int padding = (4 - (row_bytes % 4)) % 4;
    unsigned char bmp_pad[3] = {0, 0, 0};

    unsigned int filesize = 54 + (row_bytes + padding) * height;

    // BMP file header
    unsigned char file_header[14] = {'B','M',0,0,0,0,0,0,0,0,54,0,0,0};
    file_header[2] = (unsigned char)(filesize);
    file_header[3] = (unsigned char)(filesize >> 8);
    file_header[4] = (unsigned char)(filesize >> 16);
    file_header[5] = (unsigned char)(filesize >> 24);

    // BMP info header
    unsigned char info_header[40] = {40,0,0,0,0,0,0,0,0,0,0,0,1,0,24,0};
    info_header[4] = (unsigned char)(width);
    info_header[5] = (unsigned char)(width >> 8);
    info_header[6] = (unsigned char)(width >> 16);
    info_header[7] = (unsigned char)(width >> 24);
    info_header[8] = (unsigned char)(height);
    info_header[9] = (unsigned char)(height >> 8);
    info_header[10] = (unsigned char)(height >> 16);
    info_header[11] = (unsigned char)(height >> 24);

    if (fwrite(file_header, 14, 1, f) != 1 || fwrite(info_header, 40, 1, f) != 1) {
        fclose(f); return LOADBMP_FILE_OPERATION;
    }

    for (int y = height - 1; y >= 0; y--) {
        const unsigned char *row_ptr = imageData + y * width * components;
        for (unsigned int x = 0; x < width; x++) {
            unsigned char pixel[3];
            pixel[0] = row_ptr[x*components + 2]; // B
            pixel[1] = row_ptr[x*components + 1]; // G
            pixel[2] = row_ptr[x*components + 0]; // R
            if (fwrite(pixel, 1, 3, f) != 3) {
                fclose(f); return LOADBMP_FILE_OPERATION;
            }
        }
        if (padding > 0) fwrite(bmp_pad, 1, padding, f);
    }

    fclose(f);
    return LOADBMP_NO_ERROR;
}
#endif

#ifdef __cplusplus
}
#endif

#endif
