# SIMD-accelerated image processing in x86 NASM

The side project of suffering and despair

## Examples

[My test images](https://github.com/kowanietz/nasm-image-processing/tree/master/images)

[Some results](https://github.com/kowanietz/nasm-image-processing/tree/master/images/out)

## Filters

### Image Inversion

- Effect: Produces negative image by inverting each color channel
- Scalar implementation:
  - Inverts every RGB value individually
- SIMD implementation:
  - Inverts 256-bit vectors (32 packed 8-bit samples) per instruction using AVX2 byte-wise operations

### Grayscale

- Effect: Converts RBG to a monochrome representation
- Implementation: Replaces each RBG pair with the intensity value derived from its RBG channels

### Brightness

- Effect: Globally increased or reduces image brightness
- Implementation Applies a per-channel offset to each component in the image

## Sources

BMP utils: [vallentin/LoadBMP](https://github.com/vallentin/LoadBMP)

## Usage

```bash
make && ./build/main
```
