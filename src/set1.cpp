#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/basics.h" 
#include "../include/xor_ciphers.h"
#include "../include/aes.h"

char * challenge1(char * in);
char * challenge2(char * hex1 , char * hex2);
char challenge3(char * hex);
//challenge4 not done
char * challenge5(char * in , char * key);
char * challenge6(char * filename);
char * challenge7(char * filename , char * key);
char * challenge8(char * filename);

char * challenge1(char * in)
{
	uint8_t * bytes;
	int size_bytes;	
	char * b64;
	
	bytes = hex2bytes(in, &size_bytes); 
	if(bytes == NULL)
		return NULL;

	b64 = bytes2b64(bytes, size_bytes);
	free(bytes);
	
	return b64;
}

char * challenge2(char * hex1 , char * hex2)
{
	int bytes_size;	
	uint8_t * bytes1;
	uint8_t * bytes2;
	char * hex_out;

	bytes1 = hex2bytes(hex1, &bytes_size);
	if(bytes1 == NULL)
		return NULL;
	bytes2 = hex2bytes(hex2, &bytes_size);
	if(bytes2 == NULL)
	{
		free(bytes1);
		return NULL;
	}

	bytes1 = fixed_xor(bytes1 , bytes2, bytes_size);

	hex_out = bytes2hex(bytes1, bytes_size);

	free(bytes1);
	free(bytes2);
	
	return hex_out;
}

char challenge3(char * hex)
{
	int size_bytes;
	uint8_t * bytes = hex2bytes(hex, &size_bytes);
	char c;

	c = break_single_byte_xor(bytes , size_bytes);

	free(bytes);	

	return c;
}

//challenge4 not done

char * challenge5(char * in , char * key)
{
	int in_size = strlen(in);
	int key_size = strlen(key);
	uint8_t * out;
	char * out_hex;

	out = repeat_key_xor_cipher((uint8_t * )in, in_size, (uint8_t * )key, key_size);
	out_hex = bytes2hex(out, in_size);
	free(out);

	return out_hex;
}

char * challenge6(char * filename)
{
	//char filename[FILENAMEBUFFER] = "../files/set1-chal6.txt";
	char * key;
	uint8_t * bytes;
	int bytes_size;
	uint8_t * decrypted_bytes;
	char * str_out;

	//Read file
	bytes = b64file2bytes(filename , &bytes_size);

	//Calculate key
	key = break_repeat_key_xor(bytes , bytes_size);

	//Decrypt it
	decrypted_bytes = repeat_key_xor_cipher(bytes, bytes_size, (uint8_t *)key, strlen(key));
	
	//Convert to string
	str_out = bytes2string( decrypted_bytes , bytes_size);

	free(decrypted_bytes);
	free(bytes);
	free(key);
	
	return str_out;
}

char * challenge7(char * filename , char * key)
{
	//uint8_t key[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E'};
	//char filename[FILENAMEBUFFER] = "../files/set1-chal7.txt";

	int aes_input_size;
	uint8_t * aes_input = NULL;
	uint8_t * dec_out = NULL;
	uint8_t local_key[16];
	char * str_out;

	if(strlen(key) != 16){
		printf("Key is not 16 bytes\n. Exiting...\n");
		return NULL;
	}

	strncpy( (char *)local_key, key, 16);

	aes_input = b64file2bytes(filename , &aes_input_size);
	dec_out = (uint8_t*)malloc(aes_input_size * sizeof(uint8_t));

	ecb_decrypt( aes_input , dec_out, aes_input_size , local_key);

	str_out = bytes2string(dec_out , aes_input_size); 
	
	free(dec_out);
	free(aes_input);

	return str_out;
}

char * challenge8(char * filename)
{
	//char filename[FILENAMEBUFFER] = "../files/set1-chal8.txt";

	int lines;
	char ** strings = file2strings(filename , &lines);
	int * cols_table = count_colisions_strings(strings , lines);
	int max = -1;
	int max_idx = -1;
	char * ret;
	int retsize;

	//find max
	for(int i = 0; i < lines ; i++){
		if(cols_table[i] > max){
			max = cols_table[i];
			max_idx = i;
		}
	}

	//copy from strings[i] to ret so we can free remaining memory
	retsize = strlen(strings[max_idx]);
	ret = (char*)malloc( (retsize+1) * sizeof(char));
	ret[retsize] = '\0';
	strcpy(ret, strings[max_idx]);
	
	//free what we can free
	for(int i = 0; i < lines ; i++){
		free(strings[i]);
	}
	free(strings);

	return ret;
}
