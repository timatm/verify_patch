#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>
#include "page_combine.h"


void combine_page(){
    uint8_t patch[patch_height][patch_width][3];
    // memset(patch,0,patch_byte * 3);

    uint8_t *buffer_ch0 = (unsigned char*)malloc(physical_page_size*2);
    uint8_t *buffer_ch1 = (unsigned char*)malloc(physical_page_size*2);
    uint8_t *buffer_ch2 = (unsigned char*)malloc(physical_page_size*2);
    uint8_t *buffer_ch3 = (unsigned char*)malloc(physical_page_size*2);
    uint8_t *buffer_ch4 = (unsigned char*)malloc(physical_page_size*2);
    uint8_t *buffer_ch5 = (unsigned char*)malloc(physical_page_size*2);
    uint8_t *buffer_ch6 = (unsigned char*)malloc(physical_page_size*2);
    uint8_t *buffer_ch7 = (unsigned char*)malloc(physical_page_size*2);

    buffer_ch0 = read_bin_file("ch0.0.0.bin");
    buffer_ch1 = read_bin_file("ch1.0.1.bin");
    buffer_ch2 = read_bin_file("ch2.0.2.bin");
    buffer_ch3 = read_bin_file("ch3.0.3.bin");
    buffer_ch4 = read_bin_file("ch4.0.4.bin");
    buffer_ch5 = read_bin_file("ch5.0.5.bin");
    buffer_ch6 = read_bin_file("ch6.0.6.bin");
    buffer_ch7 = read_bin_file("ch7.0.7.bin");

    int read_cnt = 0;
    for(int h = 0;h < patch_height; h = h + 4){
        for(int w = 0;w < patch_width;w = w + 4){
            for(int ch = 0;ch < 3;ch++)
            {
                //page1
                patch[h][w][ch] = buffer_ch0[read_cnt];
                patch[h][w+1][ch] = buffer_ch0[read_cnt+1];
                // page2
                patch[h+1][w][ch] = buffer_ch1[read_cnt];
                patch[h+1][w+1][ch] = buffer_ch1[read_cnt+1];
                // page3
                patch[h+2][w][ch] = buffer_ch2[read_cnt];
                patch[h+2][w+1][ch] = buffer_ch2[read_cnt+1];
                // page4
                patch[h+3][w][ch] = buffer_ch3[read_cnt];
                patch[h+3][w+1][ch] = buffer_ch3[read_cnt+1];
                // page5
                patch[h][w+2][ch] = buffer_ch4[read_cnt];
                patch[h][w+3][ch] = buffer_ch4[read_cnt+1];
                // page6
                patch[h+1][w+2][ch] = buffer_ch5[read_cnt];
                patch[h+1][w+3][ch] = buffer_ch5[read_cnt+1];
                // page7
                patch[h+2][w+2][ch] = buffer_ch6[read_cnt];
                patch[h+2][w+3][ch] = buffer_ch6[read_cnt+1];
                // page8
                patch[h+3][w+2][ch] = buffer_ch7[read_cnt];
                patch[h+3][w+3][ch] = buffer_ch7[read_cnt+1];
                read_cnt = read_cnt + 2;
            }
            
        }
    }
    pr_patch(patch);

}
void combine_blk(){
    // memset(patch,0,patch_byte * 3);
    patch_t patch;

    uint8_t *buffer_ch0 = (unsigned char*)malloc(physical_page_size);
    uint8_t *buffer_ch1 = (unsigned char*)malloc(physical_page_size);
    uint8_t *buffer_ch2 = (unsigned char*)malloc(physical_page_size);
    uint8_t *buffer_ch3 = (unsigned char*)malloc(physical_page_size);
    uint8_t *buffer_ch4 = (unsigned char*)malloc(physical_page_size);
    uint8_t *buffer_ch5 = (unsigned char*)malloc(physical_page_size);
    uint8_t *buffer_ch6 = (unsigned char*)malloc(physical_page_size);
    uint8_t *buffer_ch7 = (unsigned char*)malloc(physical_page_size);

    buffer_ch0 = read_bin_file("ch0.0.0.bin");
    buffer_ch1 = read_bin_file("ch1.0.1.bin");
    buffer_ch2 = read_bin_file("ch2.0.2.bin");
    buffer_ch3 = read_bin_file("ch3.0.3.bin");
    buffer_ch4 = read_bin_file("ch4.0.4.bin");
    buffer_ch5 = read_bin_file("ch5.0.5.bin");
    buffer_ch6 = read_bin_file("ch6.0.6.bin");
    buffer_ch7 = read_bin_file("ch7.0.7.bin");

    int read_cnt = 0;
    for(int h = 0;h < patch_height; h = h + 4){
        for(int w = 0;w < patch_width;w = w + 4){
            for(int ch = 0;ch < 3;ch++)
            {
                //page1
                // patch[h][w][ch] = buffer_ch0[read_cnt];
                // patch[h][w+1][ch] = buffer_ch0[read_cnt+1];
                // // page2
                // patch[h+1][w][ch] = buffer_ch1[read_cnt];
                // patch[h+1][w+1][ch] = buffer_ch1[read_cnt+1];
                // // page3
                // patch[h+2][w][ch] = buffer_ch2[read_cnt];
                // patch[h+2][w+1][ch] = buffer_ch2[read_cnt+1];
                // // page4
                // patch[h+3][w][ch] = buffer_ch3[read_cnt];
                // patch[h+3][w+1][ch] = buffer_ch3[read_cnt+1];
                // // page5
                // patch[h][w+2][ch] = buffer_ch4[read_cnt];
                // patch[h][w+3][ch] = buffer_ch4[read_cnt+1];
                // // page6
                // patch[h+1][w+2][ch] = buffer_ch5[read_cnt];
                // patch[h+1][w+3][ch] = buffer_ch5[read_cnt+1];
                // // page7
                // patch[h+2][w+2][ch] = buffer_ch6[read_cnt];
                // patch[h+2][w+3][ch] = buffer_ch6[read_cnt+1];
                // // page8
                // patch[h+3][w+2][ch] = buffer_ch7[read_cnt];
                // patch[h+3][w+3][ch] = buffer_ch7[read_cnt+1];
                // read_cnt = read_cnt + 2;
            }
            
        }
    }
    // pr_patch(patch);

}

uint8_t * read_bin_file(const char* filename) {
    FILE* file = fopen(filename, "rb");  // Open the file in binary mode
    if (!file) {
        perror("Failed to open file");
        return;
    }
    // Get the size of the file
    fseek(file, 0, SEEK_END);  // Move the file pointer to the end of the file
    long filesize = ftell(file);  // Get the current position, which is the file size
    fseek(file, 0, SEEK_SET);  // Move the pointer back to the beginning of the file

    // Allocate memory based on the file size
    uint8_t *buffer = (unsigned char*)malloc(filesize);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    // Read the file content
    size_t read_size = fread(buffer, 1, filesize, file);
    if (read_size != filesize) {
        perror("Failed to read complete file");
        free(buffer);
        fclose(file);
        return;
    }
    // pr_buffer(buffer);
    // Cleanup
    fclose(file);
    return buffer;
}

void pr_buffer(uint8_t *buffer){
    for (uint32_t i = 0; i < physical_page_size * 2 ; i++) {
        if(i % 16 == 0)
            printf("Byte %08x : ",i);
        printf("%02X", buffer[i]); // Print each byte in hexadecimal format
        if(i % 2 == 1)
            printf(" ");
        if(i % 16 == 15)
            printf("\n");
    }
}

void pr_patch(uint8_t patch[patch_height][patch_width][3]){ 
    for (int ch = 0;ch < 3;ch++){
        printf("================ CH[%d] INFO ================\n",ch);
        for(int h = 0;h < patch_height;h++){
            for(int w = 0;w < patch_width;w++){
                if(w % 16 == 0)
                printf("Byte %08x %08x : ",h,w);
                printf("%02X", patch[h][w][ch]); // Print each byte in hexadecimal format
                if(w % 2 == 1)
                    printf(" ");
                if(w % 16 == 15)
                    printf("\n");
            }
        }
    }

}



