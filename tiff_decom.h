#ifndef __tiff_decom_H__
#define __tiff_decom_H__

#include "config.h"
int planar_convert(char *in_file , char *out_file);
void save_patch(uint8_t* red, uint8_t* green, uint8_t* blue, int patch_num);
uint8 * convertPlanarToArray(char *filename , int patch_num , uint16_t channel , uint8 *array);
void saveChannel(uint8_t *buffer , uint8_t ***patch,int line ,int width,int channel);
int tiff_decompose(char* filename , int patch_num , uint8_t ***patch);

#endif //__tiff_decom_H__
