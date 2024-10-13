#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>
#include "main.h"
#include "config.h"
int main(int argc, char* argv[]) {

    int err = 0;
    char* filename = argv[1];
    // planar_convert(filename,"456.tiff");
    uint8_t patch[patch_width][patch_height][3];
    memset(patch,0,patch_width * patch_height * 3);
    // combine_page();
    TIFF* tiff = TIFFOpen(filename, "r");
    if (!tiff) {
        fprintf(stderr, "Could not open TIFF file: %s\n", filename);
        return 1;
    }
    printf("1\n");
    uint32_t width, height;
    uint16_t samples_per_pixel, bits_per_sample, planar_config;

    TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tiff, TIFFTAG_PLANARCONFIG, &planar_config);

    printf("TIFFTAG_PLANARCONFIG : %d\n",planar_config);
}
