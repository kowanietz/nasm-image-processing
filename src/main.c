#include "../include/filters.h"


int main() {
    filter_invert(false, "images/barbara.bmp", "barbara_inverted.bmp");
    filter_invert(true, "images/barbara.bmp", "barbara_inverted_avx2.bmp");
    filter_grayscale("images/barbara.bmp", "barbara_grayscale.bmp");
    filter_brightness("images/barbara.bmp", "barbara_brightness_plus50.bmp", 50);
    filter_brightness("images/barbara.bmp", "barbara_brightness_minus50.bmp", -50);

    return 0;
}
