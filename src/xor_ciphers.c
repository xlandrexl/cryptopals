#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../include/basics.h"
#include "../include/xor_ciphers.h"

//Produces bitwise XOR between two same-sized byte arrays, storing the value in the first one.
//Returns pointer to the first array. 
//Returns NULL on error.
uint8_t * fixed_xor(uint8_t * bytes1 , uint8_t * bytes2, int bytes_size)
{
	for(int i = 0; i < bytes_size; i++){
		bytes1[i] = bytes1[i] ^ bytes2[i];
	}

	return bytes1;
}

//Ciphers a byte array of dimension bytes_size with the repeating key xor cipher, using the provided key and keysize.
//Returns allocated array of bytes with the ciphertext. 
uint8_t * repeat_key_xor_cipher(uint8_t * bytes, int bytes_size, uint8_t * key, int key_size)
{
	uint8_t * out = (uint8_t*)malloc( bytes_size * sizeof(uint8_t));
	
	for(int i = 0; i < bytes_size; i++)
	{
		out[i] = bytes[i] ^ key[i % key_size];
	}

	//hex = bytes2hex(buf, strlen(in));
	
	return out;
}
