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

	ct = ecb_encrypt_pad(tmp_pt , tmp_size , out_bytes , key);

	//Free memory
	free(tmp_pt);

	return ct;
}

uint8_t * ecb_same_key_rand_encrypt_oracle(uint8_t * pt , int in_bytes , int  * out_bytes)
{
	int count;
	int tmp_size;
	uint8_t * tmp_pt;
	uint8_t * ct;

	count = rand() % 512;
	tmp_size = in_bytes + count;
	tmp_pt = (uint8_t *)malloc(tmp_size * sizeof(uint8_t));

	for(int i = 0; i < count; i++){
		tmp_pt[i] = rand() % 256;
	}

	memcpy(tmp_pt + count , pt , in_bytes);

	ct = ecb_same_key_encrypt_oracle(tmp_pt, tmp_size , out_bytes);

	free(tmp_pt);

	return ct;
}

//Encrypt and encode Json
uint8_t * ecb_encrypt_encode(char * mail , int * ct_size)
{
	uint8_t key[] = "YELLOW SUBMARINE";
 	char * encoded_user;
	uint8_t * pt;
	uint8_t * ct;
	int pt_size;

	encoded_user = profile_for(mail);
	pt = string2bytes(encoded_user , &pt_size);
	ct = ecb_encrypt_pad(pt , pt_size , ct_size , key);

	free(encoded_user);
	free(pt);
	return ct;
}

Json ecb_decrypt_parse(uint8_t * ct , int ct_size)
{
	uint8_t key[] = "YELLOW SUBMARINE";
  uint8_t * pt;
	int pt_size;
	char * user;
	Json user_json;

	pt = ecb_decrypt_pad(ct , ct_size , &pt_size , key);
	user = bytes2string(pt , pt_size);
	free(pt);

	user_json = parse_json(user);
	free(user);

	return user_json;
}

uint8_t * cbc_flip_encrypt_oracle(char * string , int  * ct_size , uint8_t iv[AES_BLOCK_SIZE])
{
	uint8_t key[] = "YELLOW SUBMARINE";
	//uint8_t iv[] = "YELLOW SUBMARINE";
	uint8_t * ct = NULL;
	uint8_t * pt = NULL;
	uint8_t * padded_pt = NULL;
	int pt_size;

	char prepend_str[] = "comment1=cooking%20MCs;userdata=";
	char append_str[] = ";comment2=%20like%20a%20pound%20of%20bacon";
	char * buffer;

	/*Quote out ; and = */
	for(int i = 0 ; i < strlen(string) ; i++){
		if(string[i] == ';' || string[i] == '='){
			string[i] = '_';
		}
	}

	pt_size = strlen(string) + strlen(prepend_str) + strlen(append_str);
	buffer = (char*)malloc((pt_size+1)*sizeof(char));
	buffer[0] = '\0';
	strcat(buffer, prepend_str);
	strcat(buffer, string);
	strcat(buffer, append_str);
	buffer[pt_size] = '\0';

	printf("String:%s\n",buffer);

	//Prepare plaintext
	pt = string2bytes(buffer , &pt_size);
	padded_pt = pkcs_auto(pt , pt_size , ct_size);

	//Encrypt
	ct = (uint8_t *)malloc(*ct_size * sizeof(uint8_t));
	cbc_encrypt(padded_pt , ct , *ct_size , key , iv);

	free(padded_pt);
	free(buffer);
	free(pt);

	return ct;
}

int cbc_flip_check_oracle(uint8_t * ct , int ct_size , uint8_t iv[AES_BLOCK_SIZE] )
{
	uint8_t key[] = "YELLOW SUBMARINE";
	//uint8_t iv[] = "YELLOW SUBMARINE";
	uint8_t * padded_pt = NULL;
	uint8_t * pt = NULL;
	int valid;
	char * string;

	padded_pt = (uint8_t *)malloc(ct_size*sizeof(uint8_t));
	cbc_decrypt(ct , padded_pt  , ct_size , key , iv);
	pt = validate_pkcs(padded_pt , &ct_size , &valid);
	if(valid == 0){
		printf("Warning! Invallid PAD detected!n");
	}

	string = bytes2string(pt,ct_size);
	free(pt);

	/* Check */
	if(strstr(string, ";admin=true;") != NULL){
		free(string);
		return 1;
	}

	free(string);

	return 0;
}

uint8_t * cbc_pad_oracle_encrypt(int * ct_size , uint8_t iv[AES_BLOCK_SIZE])
{
	char * b64[] = {"MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=",
									"MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bXBpbic=",
									"MDAwMDAyUXVpY2sgdG8gdGhlIHBvaW50LCB0byB0aGUgcG9pbnQsIG5vIGZha2luZw==",
									"MDAwMDAzQ29va2luZyBNQydzIGxpa2UgYSBwb3VuZCBvZiBiYWNvbg==",
									"MDAwMDA0QnVybmluZyAnZW0sIGlmIHlvdSBhaW4ndCBxdWljayBhbmQgbmltYmxl",
									"MDAwMDA1SSBnbyBjcmF6eSB3aGVuIEkgaGVhciBhIGN5bWJhbA==",
									"MDAwMDA2QW5kIGEgaGlnaCBoYXQgd2l0aCBhIHNvdXBlZCB1cCB0ZW1wbw==",
									"MDAwMDA3SSdtIG9uIGEgcm9sbCwgaXQncyB0aW1lIHRvIGdvIHNvbG8=",
									"MDAwMDA4b2xsaW4nIGluIG15IGZpdmUgcG9pbnQgb2g=",
									"MDAwMDA5aXRoIG15IHJhZy10b3AgZG93biBzbyBteSBoYWlyIGNhbiBibG93",
								 };
	uint8_t * pt;
	int pt_size;
	uint8_t * padded_pt;
	int padded_pt_size;
	uint8_t * ct;
	uint8_t key[] = "YELLOW SUBMARINE";

	//b64 decode
	//pt = b642bytes(b64[ rand() % (sizeof(b64)/sizeof(char *)) ], &pt_size);
	pt = b642bytes(b64[0], &pt_size);

	//generate random iv
	gen_key(iv);

	//pad
	padded_pt = pkcs_auto(pt , pt_size , &padded_pt_size);
	free(pt);

	//encrypt
	ct = (uint8_t *)malloc(padded_pt_size*sizeof(uint8_t));
	cbc_encrypt(padded_pt , ct , padded_pt_size , key , iv);
	free(padded_pt);

	*ct_size = padded_pt_size;

	return ct;
}

int cbc_pad_oracle_check(uint8_t * ct , int ct_size , uint8_t iv[AES_BLOCK_SIZE])
{
	uint8_t * padded_pt;
	uint8_t key[] = "YELLOW SUBMARINE";
	uint8_t * pt;
	int valid;

	//decrypt
	padded_pt = (uint8_t *)malloc(ct_size*sizeof(uint8_t));
	cbc_decrypt(ct , padded_pt , ct_size , key , iv);

	//check pad
	pt = validate_pkcs(padded_pt , &ct_size , &valid);
	free(pt);

	return valid;
}

void same_key_ctr_encrypt(uint8_t * pt , uint8_t * ct , int bytes){
	uint8_t key[] = "YELLOW SUBMARINE";
	ctr_encrypt(pt , ct , bytes , key ,  0);
	return;
}

uint8_t ** same_key_ctr_encrypt_b64_strings(char * filename , int * ncts , int ** cts_size){

	char ** b64lines;
	int nlines;

	uint8_t ** pts;
	uint8_t ** cts;
	int * pts_size;

	//Read lines  of file
	b64lines = file2strings(filename , &nlines);

	//Malloc structures
	pts = (uint8_t **)malloc(nlines * sizeof(uint8_t *));
	cts = (uint8_t **)malloc(nlines * sizeof(uint8_t *));
  pts_size = (int*)malloc(nlines * sizeof(int));

	//Decode lines of file and encrypt
	for(int i = 0 ; i < nlines ; i++){
		//printf("Decoding line %d: %s\n" , i , b64lines[i]);
		pts[i] = b642bytes(b64lines[i], &(pts_size[i]) );
		cts[i] = (uint8_t *)malloc(pts_size[i] * sizeof(uint8_t)); //is it dangerous?
		same_key_ctr_encrypt(pts[i] , cts[i] , pts_size[i]);
	}

	//free
	for(int i = 0 ; i < nlines ; i++){
		free(pts[i]);
		free(b64lines[i]);
	}
	free(pts);
	free(b64lines);

	*ncts = nlines;
	*cts_size = pts_size;
	return cts;
}


// Set 4
uint8_t * cbc_key_is_iv_encrypt_oracle(uint8_t * pt , int pt_size , int * ct_size){

	uint8_t key[] = "YELLOW SUBMARINE";
	uint8_t * ct;

 	ct = cbc_encrypt_pad(pt , pt_size , ct_size , key, key);

	return ct;
}

uint8_t * cbc_key_is_iv_decrypt_oracle(uint8_t * ct , int ct_size , int * pt_size , int * error){

	uint8_t key[] = "YELLOW SUBMARINE";
	uint8_t * pt;

	pt = cbc_decrypt_pad(ct , ct_size , pt_size , key , key);
	
	//check pt
	for(int i = 0; i < *pt_size ; i++){
		if(pt[i] > 127){
			*error = 1;
		}
	}

	return pt;
}
