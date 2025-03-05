#include <stdio.h>
#include "decode.h"
#include "common.h"
#include <string.h>
#include "encode.h"
#include <stdlib.h>
/*Name : chaithanya
  Date :22-11-2024
  Project name : stegnography
  */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	//check the source file is .bmp or not
	printf("Checking arguments is started\n");
	if ( strstr(argv[2],".bmp") != NULL)
	{
		//storing input file name to char pointer
		decInfo -> encoded_image_fname = argv[2];
		//checking cml if the user is given output file name or not
		if (argv[3] != NULL)
		{
			char str[100];
			strcpy(str, argv[3]);
			char *token = strtok(str, ".");
			//stroing if name is given in cml
			strcpy(decInfo -> output_secret_fname, token);
		}
		else
		{
			//if user is not given output file name storing default name
			strcpy(decInfo -> output_secret_fname, "output_secret");
			return e_success;
		}
		

	}
	else
	{
		printf("Error :: Dificulty in chekcing source file ectenstion in .bmp.\n");
		printf("Difficulty in checking file extenstion\n");
		return e_failure;
	}
	return e_success;
}
Status open_image_file(DecodeInfo *decInfo)
{
	//opening input file 
	printf("Open files proccess started\n");
	decInfo -> encoded_image_fptr = fopen(decInfo -> encoded_image_fname, "r");
        //do error handling
	if (decInfo -> encoded_image_fptr == NULL)
	{
		//it will print the type error autometically
		perror("fopen\n");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo -> encoded_image_fname);
		//if file is equal to returning e_failure
		return e_failure;
	}
	return e_success;  

}
Status skip_bmp_header(FILE *encoded_image_fptr)
{
	//by using fseek pointing file pointer to 54 byte
	fseek(encoded_image_fptr, 54, SEEK_SET);
	return e_success;//returing e_success
}
Status decode_magic_string(DecodeInfo *decInfo)
{
	
	const int len = strlen(MAGIC_STRING);
	char magic_string[len + 1];
	//calling data from image function
	decode_data_from_image( magic_string, len, decInfo -> encoded_image_fptr);
	magic_string[2] = '\0';
	//printf("%s\n", magic_string);
	//checking decoded magic string and macro magic string both are equal or not
	if (strcmp(magic_string, MAGIC_STRING) == 0)
	{
		//both are equal return success;
		return e_success;
	}
	else
	{
		//if both are not equal return failure
		return e_failure;

	}

}
Status decode_data_from_image(char *decoded_data,int size,FILE *encoded_image_fptr)
{
	char buffer[8];
	for (int i = 0; i < size; i++)
	{
		//reading 8 from input file
		fread(buffer, 8, 1, encoded_image_fptr);
		//calling  decode_byte_from_lsb and storing char pointer
		decoded_data[i] = decode_byte_from_lsb(buffer);
	}
	return e_success;

}
Status decode_secret_file_extn_size( DecodeInfo *decInfo)
{
	char buffer[32];
	//reading 32 from input file
	fread(buffer, 32, 1, decInfo -> encoded_image_fptr);
	//storing extn size in structure memeber
	decInfo -> extn_size = decode_int_from_lsb(buffer);
	//printf("%d\n",decInfo -> extn_size);
	return e_success;
}
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
	char file_extn[decInfo -> extn_size + 1];
	char buffer[8];
	int i;
	for(i = 0; i < decInfo -> extn_size; i++)
	{
	//reading 8 from input file	
	fread(buffer, 8, 1, decInfo -> encoded_image_fptr);
	//calling function and storing in char array
	file_extn[i] = decode_byte_from_lsb(buffer);
	}
	file_extn[i] = '\0';

	//printf("%s\n", file_extn);
	//concatinating extn and file name
	strcat(decInfo -> output_secret_fname, file_extn);
	decInfo -> output_fname = decInfo -> output_secret_fname;
	printf("Secret message file::%s\n",decInfo -> output_fname);
	//opening output file in write mode
	decInfo -> output_secret_fptr = fopen(decInfo -> output_fname, "w");
	//do error handling
	if(decInfo -> output_secret_fptr == NULL)
	{
		//it will print the type error autometically
		perror("fopen\n");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo -> output_fname);
		return e_failure;
	}
	//if file opened return success
        printf("Secret file openned succesfully\n");
	return e_success;
	

}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
	char buffer[32];
	//reading 32 bytes from input file
	fread(buffer, 32, 1, decInfo -> encoded_image_fptr);
	decInfo -> secret_file_size = decode_int_from_lsb(buffer);
	return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
	char secret_file_data[decInfo -> secret_file_size + 1];
	char buffer[8];
	for (int i = 0; i < decInfo -> secret_file_size; i++)
	{
		//reading 8 bytes from input file
		fread(buffer, 8, 1, decInfo -> encoded_image_fptr);
		//calling decode_byte_from_lsb
	        secret_file_data[i] = decode_byte_from_lsb(buffer);
		//printf("%c ", secret_file_data[i]);
		//stroing  secret data in output file
		putc(secret_file_data[i], decInfo -> output_secret_fptr);
	}
        secret_file_data[decInfo -> secret_file_size] = '\0';
	return e_success;

}
char decode_byte_from_lsb(char *image_buffer)
{
	char ch = 0; 
	int get;
        for(int i = 0; i < 8; i++)
        {
		get = (image_buffer[i] & 1) << (7 - i);//get the lsb bit and shift to msb
		ch = ch | get;//replace using or operator
	       
	}
        return ch;

}
int decode_int_from_lsb(char *buffer)
{
	int res = 0;
	int get;
        for (int i = 0; i < 32; i++) 
        { 
		 get = (buffer[i] & 1) << (31 - i);//get the lsb bit and shift to msb
		 res = res | get;//replace using or operator
        }
	//printf("size-->%d\n",res);
        return res;

}
Status do_decoding(DecodeInfo *decInfo)
{
	if (open_image_file(decInfo) == e_success)
	{
		printf("Files opend Successfully.\n");
		if (skip_bmp_header(decInfo -> encoded_image_fptr) == e_success)
		{
			printf("Decoding magic string\n");
		       if (decode_magic_string(decInfo) == e_success)
		       {
				printf("Magic String Decoded Successfully.\n");
				printf("Decoding Secret file extenstion size.\n");
		       }
		       else
		       {
			       printf("Error::Magic String cannot be decode.\n");
			       printf("Error::In Decoding\n");
		       }
		       if (decode_secret_file_extn_size(decInfo) == e_success)
		       {
			       printf("Secret file extenstion Size Decoded Successfully.\n");
			       printf("Decoding Secret file extenstion\n");
		       }
		       if (decode_secret_file_extn(decInfo) == e_success)
		       {
			       printf("Secret file extenstion Decoded Successfully.\n");
			       printf("Decoding Secret file size..\n");
		       }
		       if (decode_secret_file_size(decInfo) == e_success)
		       {
			       printf("Secret file size Decoded successfully.\n");
			       printf("Decoding secret data..\n");
		       }
		       if (decode_secret_file_data(decInfo) == e_success)
		       {
			       printf("Secret data Decoded Successfully.\n");
			       return e_success;
			       
		       }
		       else
		       {
			        printf("Decoded Unsuccessfully\n");
				return e_failure;
		       }

		}
		else
		{
			 printf("Decoded Unsuccessfully\n");
			 return e_failure;
		}

	}
	else
	{
		 printf("Decoded Unsuccessfully\n");
                 return e_failure;
	}

}

