#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
	//Checking if command line aruguments are greather than 1 or not
	if(argc > 1)
	{
		//calling check_operation function
		 //if it is true Encoding will start
		if (check_operation_type(argv) == e_encode)
		{
			//Declaring structure variable
			EncodeInfo encInfo;
			//function call
			if (argc > 3)
			{
				printf("Encoding Started.\n");
				printf("Arguments verified successfully\n");
			if(read_and_validate_encode_args(argv, &encInfo) == e_success)
			{
				//function call
				printf("Arguments verified successfully\n");
				do_encoding(&encInfo);
				printf("Encoding is completed\n");
				return 0;
			}
			}
			else
			{
				printf("ERROR : unable To perform Operation.Arrguments Should be > 3\n");
				printf("For Encoding Arguments sould be : \n");
				printf("./a.out -e <sourcfile.bmp> <secretfile.txt> <outputfile.bmp>(outputfile is optional\n");
				printf("For decoding Arguments should be:\n");
				printf("./a.out -d <sourcefile.bmp> <outputfile.bmp>\n");
			}
		}
		
		//calling check_operation function
		//if it is true decoding will start
		else if(check_operation_type(argv) == e_decode)
		{
			//Declaring structure variable
			DecodeInfo decInfo;
			if (argc > 2)
			{
				printf("Decoding started.\n");
				
			if (read_and_validate_decode_args(argv, &decInfo) == e_success)
			{
				
				//function call
				 printf("Arguments verified successfully\n");
			        do_decoding(&decInfo);
				printf("Decoding is Completed.\n");
				return 0;

			}
			}
			else
			{
				 printf("ERROR : unable To perform Operation.Arrguments Should be > 3\n");
				 printf("For Encoding Arguments sould be : \n");
				 printf("./a.out -e <sourcfile.bmp> <secretfile.txt> <outputfile.bmp>(outputfile is optional\n");
				 printf("For decoding Arguments should be:\n");
				 printf("./a.out -d <sourcefile.bmp> <outputfile.bmp>\n");

			}
		}
		else 
		{
			printf("Error: Unable to proceed operation provide valid type(-e or -d)\n");
			return 0;
		}
	}
	else
	{
		printf("Please pass the proper arguments\n");
	}
    

    return 0;
}
