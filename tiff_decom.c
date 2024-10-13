#include <stdio.h>
#include <tiffio.h>
#include "tiff_decom.h"

int planar_convert(char *in_file , char *out_file) {
    TIFF *input = TIFFOpen(in_file, "r");
    if (!input) {
        fprintf(stderr, "Error: Could not open input TIFF file %s\n", in_file);
        return 1;
    }

    // Create output TIFF
    TIFF *output = TIFFOpen(out_file, "w");
    if (!output) {
        fprintf(stderr, "Error: Could not create output TIFF file %s\n", out_file);
        TIFFClose(input);
        return 1;
    }

    // Read TIFF info
    uint32 width, height;
    uint16 samplesPerPixel, bitsPerSample, planarConfig, photo;
    TIFFGetField(input, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(input, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(input, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
    TIFFGetField(input, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
    TIFFGetField(input, TIFFTAG_PLANARCONFIG, &planarConfig);
    TIFFGetField(input, TIFFTAG_PHOTOMETRIC, &photo);

    // Check if input TIFF is in Chunky format
    if (planarConfig != PLANARCONFIG_CONTIG || samplesPerPixel != 3) {
        fprintf(stderr, "Error: Input file is already planarr format.\n");
        TIFFClose(input);
        TIFFClose(output);
        return 1;
    }

    // Allocate memory to hold the entire image in Chunky format
    tsize_t scanlineSize = TIFFScanlineSize(input);
    unsigned char *buffer = (unsigned char *) _TIFFmalloc(scanlineSize * height);
    if (!buffer) {
        fprintf(stderr, "Error: Unable to allocate memory for input TIFF data.\n");
        TIFFClose(input);
        TIFFClose(output);
        return 1;
    }

    // Read the image into buffer (for RGB Chunky format)
    for (uint32 row = 0; row < height; row++) {
        if (TIFFReadScanline(input, buffer + row * scanlineSize, row, 0) < 0) {
            fprintf(stderr, "Error reading scanline %u.\n", row);
            _TIFFfree(buffer);
            TIFFClose(input);
            TIFFClose(output);
            return 1;
        }
    }

    // Allocate memory for planar data
    unsigned char *red = (unsigned char *) _TIFFmalloc(width * height);
    unsigned char *green = (unsigned char *) _TIFFmalloc(width * height);
    unsigned char *blue = (unsigned char *) _TIFFmalloc(width * height);
    if (!red || !green || !blue) {
        fprintf(stderr, "Error: Unable to allocate memory for planar data.\n");
        _TIFFfree(buffer);
        TIFFClose(input);
        TIFFClose(output);
        return 1;
    }

    // Convert from Chunky to Planar
    for (uint32 row = 0; row < height; row++) {
        for (uint32 col = 0; col < width; col++) {
            uint32 index = (row * width + col) * 3;
            red[row * width + col] = buffer[index];      // Red channel
            green[row * width + col] = buffer[index + 1]; // Green channel
            blue[row * width + col] = buffer[index + 2];  // Blue channel
        }
    }

    // Set up output TIFF with planar format
    TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel);
    TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, bitsPerSample);
    TIFFSetField(output, TIFFTAG_PHOTOMETRIC, photo);
    TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE); // Planar format

    // Write the planar data
    for (uint32 row = 0; row < height; row++) {
        TIFFWriteScanline(output, red + row * width, row, 0);    // Red plane
        TIFFWriteScanline(output, green + row * width, row, 1);  // Green plane
        TIFFWriteScanline(output, blue + row * width, row, 2);   // Blue plane
    }

    // Clean up
    _TIFFfree(buffer);
    _TIFFfree(red);
    _TIFFfree(green);
    _TIFFfree(blue);
    TIFFClose(input);
    TIFFClose(output);

    printf("Conversion from Chunky to Planar format completed.\n");
    return 0;
}

void save_patch(uint8_t* red, uint8_t* green, uint8_t* blue, int patch_num) {
    char filename[64];
    sprintf(filename, "patch_%d.ppm", patch_num);
    FILE* f = fopen(filename, "wb");
    if (!f) {
        perror("Failed to open output file");
        return;
    }
    fprintf(f, "P6\n%d %d\n255\n", patch_width, patch_height);
    for (int i = 0; i < patch_width * patch_height; i++) {
        fputc(red[i], f);
        fputc(green[i], f);
        fputc(blue[i], f);
    }
    fclose(f);
}

uint8 * convertPlanarToArray(char *filename , int patch_num , uint16_t channel , uint8 *array){
    TIFF *tif = TIFFOpen(filename, "r");
    if (tif == NULL) {
        fprintf(stderr, "Error opening TIFF file.\n");
        return;
    }

    uint32 width, height;
    uint16 samplesPerPixel, bitsPerSample;
    
    // 讀取基本信息
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);

    // 計算一維陣列的大小
    if (array == NULL) {
        fprintf(stderr, "Error allocating memory for array.\n");
        TIFFClose(tif);
        return;
    }

    // 逐行讀取每個色板的數據
    for (uint32 row = 0; row < height; row++) {
        uint8 *buf = NULL;
        size_t bufSize = 0;

        // 讀取每個色板的數據
        TIFFReadScanline(tif, buf, row, channel);
        bufSize = TIFFScanlineSize(tif);

        // 將資料放入一維陣列
        for (size_t col = 0; col < width; col++) {
            array[(row * width + col) * samplesPerPixel + channel] = buf[col];
        }
        free(buf);
    }
    TIFFClose(tif);
    return array;
}

int tiff_decompose(char* filename , int patch_num , uint8_t ***patch) {
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
    
    if (samples_per_pixel != 3 || bits_per_sample != 8 || planar_config != PLANARCONFIG_SEPARATE) {
        fprintf(stderr, "This program only supports 8-bit RGB TIFF files with planar configuration.\n");
        TIFFClose(tiff);
        return 1;
    }

    int patch_width_num  = 0;
    int patch_height_num = 0;
    int tiff_width_C     = 0;
    int tiff_height_C    = 0;

    patch_width_num = (width % patch_width == 0) ? (width / patch_width) : (width / patch_width) + 1;

    patch_height_num = (height % patch_height == 0) ? (height / patch_height) : (height / patch_height) + 1; 

    if (patch_width_num == 1 && patch_height_num == 1 && patch_num == 1)
    {
        tiff_width_C = 1;
        tiff_height_C = 1;
    }
    else{
        tiff_width_C  = patch_num % (patch_width_num);
        tiff_height_C = 
            (patch_num / patch_width_num == 0) ? 
            ( (patch_num / patch_width_num)) : ( (patch_num / patch_width_num) +1 );
    }
            
    // Allocate memory for a single patch of size 512x512 for three channels (R, G, B)

    uint8_t *tempbuffer = malloc(width * sizeof(uint8_t));

    int read_line  = (tiff_height_C - 1) * patch_height;
    int read_witdh = (tiff_width_C - 1)  * patch_width;


    printf("patch_width_num : %d\n",patch_width_num);
    printf("patch_height_num : %d\n",patch_height_num);
    printf("tiff_width_C : %d\n",tiff_width_C);
    printf("tiff_height_C : %d\n",tiff_height_C);
    printf("read_line : %d\n",read_line);
    printf("read_witdh : %d\n",read_witdh);

    for (int i = 0; i < patch_height ; i++) {
        printf("read_line : %d",read_line);
        for(int ch = 0;ch < 3;ch++){
            if (read_line >= height){
                printf("read_line : %d",read_line);
                for(int j = 0;j < patch_width;j++){
                    patch[i][j][ch] = 0;
                }
            }
            else{
                printf("read_line : %d",read_line);
                TIFFReadScanline(tiff, tempbuffer, read_line ,ch);
                for(int j = 0;j < patch_width;j++){
                    if( (read_witdh+j) >= width)
                        patch[i][j][ch] = 0;
                    else
                        patch[i][j][ch] = tempbuffer[read_witdh+j];
                }
                memset(tempbuffer,0,patch_width);
            }
            // saveChannel(tempbuffer,patch,read_line,read_witdh,j);
        }
        read_line++;
    }
    TIFFClose(tiff);
    
    return 0;
}


int tiff_decompose_modify(char* filename){
    TIFF* tiff = TIFFOpen(filename, "r");
    if (!tiff) {
        fprintf(stderr, "Could not open TIFF file: %s\n", filename);
        return 1;
    }
    uint32_t width, height;
    uint16_t samples_per_pixel, bits_per_sample, planar_config;

    TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tiff, TIFFTAG_PLANARCONFIG, &planar_config);
     
    int tiff_byte   = width * height * samples_per_pixel * bits_per_sample;
    int package_num = 0;

    //determine package numbers
    if(tiff_byte % patch_byte == 0)
        package_num = tiff_byte / patch_byte;
    else
        package_num = (tiff_byte / patch_byte)+1 ;  
  
    printf("package_num : %d\n",package_num);

    if (samples_per_pixel != 3 || bits_per_sample != 8 || planar_config != PLANARCONFIG_SEPARATE) {
        fprintf(stderr, "This program only supports 8-bit RGB TIFF files with planar configuration.\n");
        TIFFClose(tiff);
        return 1;
    }

    uint8_t* red = (uint8_t*)malloc(patch_pixel);
    uint8_t* green = (uint8_t*)malloc(patch_pixel);
    uint8_t* blue = (uint8_t*)malloc(patch_pixel);


    if (!red || !green || !blue) {
        fprintf(stderr, "Memory allocation failed\n");
        TIFFClose(tiff);
        return 1;
    }

    memset(red, 0, patch_pixel);
    memset(green, 0, patch_pixel);
    memset(blue, 0, patch_pixel);
    
    int patch_num = 0;
    for (uint32_t y = 0; y < height; y += patch_height){

    }

    for (uint32_t y = 0; y < height; y += patch_height) {
        for (uint32_t x = 0; x < width; x += patch_width) {
            // Read Red channel
            for (uint32_t py = 0; py < patch_height; py++) {
                TIFFReadScanline(tiff, red + py * patch_width, y + py, 0);
            }

            // Read Green channel
            for (uint32_t py = 0; py < patch_height; py++) {
                TIFFReadScanline(tiff, green + py * patch_width, y + py, 1);
            }

            // Read Blue channel
            for (uint32_t py = 0; py < patch_height; py++) {
                TIFFReadScanline(tiff, blue + py * patch_width, y + py, 2);
            }
        }
    }

    free(red);
    free(green);
    free(blue);
    TIFFClose(tiff);

    return 0;
} 


// -------------------------------------------------------------------
// debug
// -------------------------------------------------------------------

// void pr_patch(uint8_t*** patch ){
//     printf("Original Channels Information:\n");
//     for (uint32_t y = 0; y < patch_height; y++) {
//         for (uint32_t x = 0; x < patch_width; x++) {
//             printf("Pixel (%d, %d) - R: %d, G: %d, B: %d\n",
//                    x, y,patch[y][x][0],patch[y][x][1],patch[y][x][2]);
//         }
//     }
// }

void pr_patch_channel(uint8_t **patch_ch){
    
}