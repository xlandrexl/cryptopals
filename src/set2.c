#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/basics.h" 
#include "../include/xor_ciphers.h"
#include "../include/aes.h"

int challenge9(char * pt , int n);
int challenge10(char * filename , char * key);
//challenge11 not done
//challenge12 not done
//challenge13 not done
//challenge14 not done
int challenge15(char * string);
//challenge16 not done

/*int main()
{
	char key[]={"YELLOW SUBMARINE"};
	char filename[] = "../files/set1-chal7.txt";

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
*/

int challenge9(char * pt , int n)
{
	//char pt[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E','\0'};
	//int n = 20;

	char * str_out;
	int bytes_size;
	uint8_t * bytes;
	
	bytes = string2bytes(pt, &bytes_size);
	bytes = pkcs( bytes , bytes_size , n);

	str_out = bytes2string(bytes , bytes_size + n);

	printf("%s" , str_out);
	
	free(bytes);
	free(str_out);

	return 1;
}

int challenge10(char * filename , char * key)
{
	//uint8_t key[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E'};
	//char filename[FILENAMEBUFFER] = "../files/set2-chal10.txt";

	uint8_t iv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int aes_input_size = 0;
	uint8_t * aes_input = NULL;
	uint8_t * dec_out = NULL;
	uint8_t local_key[16];
	char * str_out;

	if(strlen(key) != 16){
		printf("Key is not 16 bytes\n. Exiting...\n");
		return -1;
	}

	strncpy( (char *)local_key, key, 16);

	printf("Going in... Filename %s and aes_input_size %d \n" , filename, aes_input_size);
	aes_input = b64file2bytes(filename , &aes_input_size); //This line is giving error
	printf("...Out\n");

	dec_out = (uint8_t*)malloc(aes_input_size * sizeof(uint8_t));
	cbc_decrypt( aes_input , dec_out, aes_input_size , local_key , iv);

	str_out = bytes2string(dec_out , aes_input_size); 
	
	printf("%s" , str_out);

	free(str_out);
	free(dec_out);
	free(aes_input);

	return 1;
}

int challenge15(char * plaintext)
{
	//char plaintext[]="ICE ICE BABY\x04\x04\x04\x04";

	int bytes_size;
	uint8_t * bytes = string2bytes(plaintext , &bytes_size);
	int valid = 0;	
	char * str_out;

	bytes = validate_pkcs(bytes , &bytes_size , &valid);

	if(valid == 0){
		printf("inv");
		return 1;
	}
	
	str_out = bytes2string(bytes , bytes_size);
	
	printf("%s" , str_out);	

	free(str_out);
	free(bytes);

	return 1;
}


