#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "aux_functions.cpp"

const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

uint8_t * hex2bytes(char * hex, int size_hex, int * size_bytes) //Verify pad
{
	uint8_t * bytes = NULL;
	char * p = hex;	

	/*Get size of bytes array*/
	if(size_hex % 2 == 0){ /*Even*/
		(*size_bytes) = size_hex/2;
	}else{ /*Odd*/
		(*size_bytes) = (size_hex+1)/2; 
	}
	
	bytes = (uint8_t*)malloc((*size_bytes) * sizeof(uint8_t));	
	
	for(int j = 0; j < (*size_bytes); j++){
		sscanf(p, "%2hhx", &(bytes[j]));
		p += 2;
	}
	
	return bytes;
}

char * bytes2b64(uint8_t * bytes, int size_bytes, int * size_b64) //Verify pad
{
	char * b64 = NULL;
	int val;

	/*Get size of bytes array*/
	(*size_b64) = size_bytes; 
	if( (*size_b64) % 3 != 0){ //Make size_b64 divisible by 3
		(*size_b64) += 3 - ((*size_b64) % 3); 
	}
	(*size_b64) = (*size_b64) *4 / 3; //Multyply by 4/3 
	
	b64 = (char*)malloc((*size_b64 + 1) * sizeof(char));// +1 to add \0
	b64[(*size_b64)] = '\0';
	
	for(int i=0,j=0; i < size_bytes; i+=3 , j+=4){
		/*Push 3 bytes into val. Can be replaced with one-liners*/
		val = bytes[i];
		val = (i + 1 < size_bytes) ? val << 8 | bytes[i+1] : val << 8;
		val = (i + 2 < size_bytes) ? val << 8 | bytes[i+2] : val << 8;

		b64[j] = b64chars[(val >> 18) & 0x3F];
		b64[j+1] = b64chars[(val >> 12) & 0x3F];
		b64[j+2] = (i + 1 < size_bytes) ? b64chars[(val >> 6) & 0x3F] : '='; 
		b64[j+3] = (i + 2 < size_bytes) ? b64chars[val & 0x3F] : '=';
		//variable = condition ? value_if_true : value_if_false
	}

	return b64;
}

char * hex2b64(char * hex)
{
	int size_hex = strlen(hex);
	uint8_t * bytes;
	int size_bytes;	

	char * b64;
	int size_b64;

	bytes = hex2bytes(hex, size_hex , &size_bytes); 
	b64 = bytes2b64(bytes, size_bytes , &size_b64);
	
	free(bytes);
	
	return b64;
}

/*int main(int argc, char **argv)
{
	char hex[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	uint8_t * bytes;

	int size_hex = sizeof(hex) - 1;//Ignore \0
	int size_bytes;

	bytes = hex2bytes(hex, size_hex , &size_bytes); 
	//print_hex(bytes, size_bytes);

	char * b64;
	int size_b64;
	b64 = bytes2b64(bytes, size_bytes , &size_b64);
	//printf("%s\n" , b64);		

	free(bytes);
	free(b64);

	return 1;
}*/


