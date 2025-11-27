#pragma once

#include <stdbool.h>


unsigned int filter_invert(bool avx2, const char *path, const char *out_path);

unsigned int filter_grayscale(const char *path, const char *out_path);

unsigned int filter_brightness(const char *path, const char *out_path, signed int brightness);
