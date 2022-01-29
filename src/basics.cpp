#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//Characters used in base64 encoding
const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//Transforms an hexadecimal string into an array of bytes. 
//Returns allocated bytes array and its size in size_bytes.
//Returns NULL on error.
uint8_t * hex2bytes(char * hex, int * size_bytes)
{
	uint8_t * bytes = NULL;
	char * p = hex;
	int size_hex = strlen(hex);	

	/*Get size of bytes array*/
	if(size_hex % 2 == 0){ /*Even*/
		(*size_bytes) = size_hex/2;
	}else{ /*Odd*/
		(*size_bytes) = (size_hex+1)/2; 
	}
	
	bytes = (uint8_t*)malloc((*size_bytes) * sizeof(uint8_t));
	if(bytes == NULL){
		printf("Error allocating memory for bytes array.\n");
		return NULL;
	}	
	
	for(int j = 0; j < (*size_bytes); j++){
		sscanf(p, "%2hhx", &(bytes[j])); /*Warning gave me the hh. Check it out*/
		p += 2;
	}
	
	return bytes;
}

//Transforms an array of bytes into an hexadecimal string. 
//Returns allocated hexadecimal string.
//Returns NULL on error.
char * bytes2hex(uint8_t * bytes, int size_bytes)
{
	char * hex = NULL;
	int size_hex = 2 * size_bytes;
	char * p;

	hex = (char*)malloc((size_hex + 1) * sizeof(char));// +1 to add \0
	if(hex == NULL){
		printf("Error allocating memory for hexadecimal string.\n");
		return NULL;
	}	
	hex[size_hex] = '\0';
	p = hex;

	for(int i = 0; i < size_bytes ; i++){
		sprintf(p, "%02x", bytes[i]);
		p += 2;
	}
	
	return hex;
}

//Transforms an array of bytes into a padded base64 encoded string. 
//Returns allocated base64 string.
//Returns NULL on error.
char * bytes2b64(uint8_t * bytes, int size_bytes)
{
	char * b64 = NULL;
	long int val; //Assure it holds 3+ bytes!
	int size_b64;
	
	//Get size of bytes array
	size_b64 = size_bytes; 
	if( size_b64 % 3 != 0){ //Make size_b64 divisible by 3
		size_b64 += 3 - (size_b64 % 3); 
	}
	size_b64 = size_b64 * 4 / 3; //Multyply by 4/3 because b64 encodes 3 bytes in 4 chars.
	
	b64 = (char*)malloc((size_b64 + 1) * sizeof(char));// +1 to add \0
	if(b64 == NULL){
		printf("Error allocating memory for base64 string.\n");
		return NULL;
	}	
	b64[size_b64] = '\0';
	
	for(int i=0,j=0; i < size_bytes; i+=3 , j+=4){
		/*Push (up to) 3 bytes into val (assignation and left-shift) */
		val = bytes[i];
		val = (i + 1 < size_bytes) ? val << 8 | bytes[i+1] : val << 8;
		val = (i + 2 < size_bytes) ? val << 8 | bytes[i+2] : val << 8;

		/*In b64, one char is represented by 6 bits. Right shift followed by & with 0b111111 to separate every 6-bit part */
		b64[j] = b64chars[(val >> 18) & 0x3F]; 
		b64[j+1] = b64chars[(val >> 12) & 0x3F];
		/*Assure if necessary. Otherwise, pad with =*/
		b64[j+2] = (i + 1 < size_bytes) ? b64chars[(val >> 6) & 0x3F] : '='; 
		b64[j+3] = (i + 2 < size_bytes) ? b64chars[val & 0x3F] : '=';
	}

	return b64;
}

//Transforms an hexadecimal string into a padded base64 encoded string. 
//Returns allocated base64 string.
//Returns NULL on error.
char * hex2b64(char * hex)
{
	uint8_t * bytes;
	int size_bytes;	
	char * b64;
	
	bytes = hex2bytes(hex, &size_bytes); 
	if(bytes == NULL)
		return NULL;

	b64 = bytes2b64(bytes, size_bytes);
	free(bytes);
	
	return b64;
}

//Prints decimal value of array of n bytes 
void print_dec(uint8_t * bytes , int n)
{
	int i;

	for(i = 0; i < n; i++){
		printf("%d" , bytes[i]);
	}
	printf("\n");
	
	return;
}

//Prints hexadecimal value of array of n bytes 
void print_hex(uint8_t * bytes , int n)
{
	int i;

	for(i = 0; i < n; i++){
		printf("%02x " , bytes[i]);
	}
	printf("\n");
	
	return;
}

