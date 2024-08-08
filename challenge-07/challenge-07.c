#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"
#include "../include/aes.h"

int challenge7(char * filename , char * key)
{
	//uint8_t key[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E'};
	//char filename[] = "../files/set1-chal7.txt";

	int aes_input_size;
	uint8_t * aes_input = NULL;
	uint8_t * dec_out = NULL;
	uint8_t local_key[16];
	char * str_out;

	if(strlen(key) != 16){
		printf("Key is not 16 bytes\n. Exiting...\n");
		return -1;
	}

	strncpy( (char *)local_key, key, 16);

	aes_input = b64file2bytes(filename , &aes_input_size);
	dec_out = (uint8_t*)malloc(aes_input_size * sizeof(uint8_t));

	ecb_decrypt( aes_input , dec_out, aes_input_size , local_key);

	str_out = bytes2string(dec_out , aes_input_size);

	printf("%s" , str_out);

	free(str_out);
	free(dec_out);
	free(aes_input);

	return 1;
}