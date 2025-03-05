#ifndef DECODE_H
#define DECODE_H

#include "types.h"


typedef struct _DecodeInfo
{
	/*Encoded image*/
	char *encoded_image_fname;//To store input file name
	FILE *encoded_image_fptr;//file pointer for input file

	/*Output file*/
	char *output_fname;//to store output file name
	char output_secret_fname[100];//to store the name at starting
	FILE *output_secret_fptr;//file pointer for output file
	int extn_size;//to store secret file extn size
	int secret_file_size;//to store secret file data size
	
}DecodeInfo;
/* Decoding function prototype */

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_imag_file(DecodeInfo *decInfo);

/* Skip bmp image header */
Status skip_bmp_header(FILE *encoded_image_fptr);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/*Decode the extn size */
Status decode_secret_file_extn_size( DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *DecInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *DecInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *DecInfo);

/* Decode a byte into LSB of image data array */
char decode_byte_from_lsb(char *image_buffer);

/*Decode the int to lsb */
int decode_int_from_lsb(char *buffer);

/*Get the magic string data from input file*/
Status decode_data_from_image(char *decoded_data,int size,FILE *encoded_image_fptr);

#endif
