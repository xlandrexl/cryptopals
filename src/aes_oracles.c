#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

#include "../include/basics.h"
#include "../include/aes.h"
#include "../include/xor_ciphers.h"

uint8_t * ecb_cbc_encrypt_oracle(uint8_t * pt , int in_bytes , int * out_bytes , int * mode)
{
	uint8_t key[KEY_SIZE];
	uint8_t iv[16];
	int count;
	uint8_t * ct = NULL;

	uint8_t * tmp_pt = NULL;
	int tmp_size;

	//I can pad this... 
	if(in_bytes % 16 != 0){
		printf("Invalid plaintext size. Exiting...\n");
		exit(0);
	}

	//Copy original plaintext to tmp_pt (in order to keep original intact)
	tmp_size = in_bytes;
	tmp_pt = (uint8_t *)malloc(tmp_size * sizeof(uint8_t));
	memcpy(tmp_pt , pt , in_bytes);
	
	//Generate random key
	gen_key(key);

	//Append before and after
	count = 5 + (rand() % 6);
	tmp_pt = pkcs(tmp_pt , tmp_size , tmp_size + count);
	tmp_size += count;

	//count = 5 + (rand() % 6);
	count = 16 - count; //So what it sums to 16

	tmp_pt = pkcs_bef(tmp_pt , tmp_size , tmp_size + count);
	tmp_size += count;

	(*out_bytes) = tmp_size;
	
	//Malloc ct
	ct = (uint8_t *)malloc( (*out_bytes) * sizeof(uint8_t)); 
	
	//Encrypt
	count = rand() % 2;
	if(count == 0){ //ECB
		ecb_encrypt(tmp_pt , ct , (*out_bytes) , key);
		*mode = 1;
	}else{
		gen_key(iv);
		cbc_encrypt(tmp_pt , ct , (*out_bytes) , key , iv);
		*mode = 2;
	}
	
	free(tmp_pt);	
	
	return ct;
}

//Plaintext left untouched. Returns allocated ciphertext.
uint8_t * ecb_same_key_encrypt_oracle(uint8_t * pt , int in_bytes , int  * out_bytes)
{
	uint8_t key[] = "YELLOW SUBMARINE";
	uint8_t * ct = NULL;

	uint8_t * tmp_pt = NULL;
	int tmp_size;

	char append_str[] = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

	uint8_t * append = NULL;
	int append_size;

	//Copy original plaintext to tmp_pt (in order to keep original intact)
	tmp_size = in_bytes + strlen(append_str);
	tmp_pt = (uint8_t *)malloc(tmp_size * sizeof(uint8_t));
	memcpy(tmp_pt , pt , in_bytes);
	
	//Append
	append = b642bytes(append_str, &append_size);
	memcpy(tmp_pt + in_bytes , append , append_size);
	
	//Pad
	if(tmp_size % 16 != 0){
		tmp_pt = pkcs( tmp_pt , tmp_size , tmp_size + (16 - tmp_size % 16) );
		tmp_size += (16 - tmp_size % 16);
	}else{
		tmp_pt = pkcs( tmp_pt , tmp_size , tmp_size + 16 );
		tmp_size += 16;
	}

	//Malloc ct
	(*out_bytes) = tmp_size;
	ct = (uint8_t *)malloc( (*out_bytes) * sizeof(uint8_t)); 
	
	//Encrypt
	ecb_encrypt(tmp_pt , ct , (*out_bytes) , key);

	//Free memory
	free(tmp_pt);	
	
	return ct;
}



