#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

#include "../include/basics.h"
#include "../include/aes.h"
#include "../include/xor_ciphers.h"

void ecb_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[KEY_SIZE])
{
	/* Buffers */
	uint8_t in[AES_BLOCK_SIZE];
	uint8_t out[AES_BLOCK_SIZE];
	int cycles = bytes / AES_BLOCK_SIZE;

	/* Prepare AES-128 bit ECB Encryption */
	AES_KEY prv_key;
	AES_set_encrypt_key(key, KEY_SIZE * 8, &prv_key); // Size of key is in bits
	
	for(int i = 0 ; i < cycles ; i++){
		memcpy(in , pt + (i*AES_BLOCK_SIZE) , AES_BLOCK_SIZE);
		AES_ecb_encrypt(in, out, &prv_key, AES_ENCRYPT);
		memcpy(ct + (i*AES_BLOCK_SIZE) , out , AES_BLOCK_SIZE);
	}

	return;
}

void ecb_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[KEY_SIZE])
{
	/* Buffers */
	uint8_t in[AES_BLOCK_SIZE];
	uint8_t out[AES_BLOCK_SIZE];
	int cycles = bytes / AES_BLOCK_SIZE;

	/* AES-128 bit ECB Decryption */
	AES_KEY prv_key;
	AES_set_decrypt_key(key, KEY_SIZE*8, &prv_key); // Size of key is in bits

	for(int i = 0 ; i < cycles ; i++){
		memcpy(in , ct + (i*AES_BLOCK_SIZE) , AES_BLOCK_SIZE);
		AES_ecb_encrypt(in, out, &prv_key, AES_DECRYPT);
		memcpy(pt + (i*AES_BLOCK_SIZE) , out , AES_BLOCK_SIZE);
	}
}

void cbc_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[KEY_SIZE] , uint8_t iv[AES_BLOCK_SIZE])
{
	/* Buffers */
	uint8_t * in = (uint8_t *)malloc(AES_BLOCK_SIZE * sizeof(uint8_t));
	uint8_t * out = (uint8_t *)malloc(AES_BLOCK_SIZE * sizeof(uint8_t));
	int cycles = bytes / AES_BLOCK_SIZE;

	/* Prepare AES-128 bit ECB Encryption */
	AES_KEY prv_key;
	AES_set_encrypt_key(key, KEY_SIZE*8, &prv_key); // Size of key is in bits
	
	/*First one, xor with iv */
	memcpy(in , pt , AES_BLOCK_SIZE);
	in = fixed_xor(in, iv, AES_BLOCK_SIZE);
	AES_ecb_encrypt(in, out, &prv_key, AES_ENCRYPT);
	memcpy(ct , out , AES_BLOCK_SIZE);

	for(int i = 1 ; i < cycles ; i++){
		memcpy(in , pt + (i*AES_BLOCK_SIZE) , AES_BLOCK_SIZE);
		in = fixed_xor(in, ct + ((i-1)*AES_BLOCK_SIZE) , AES_BLOCK_SIZE); //XOR with previous ciphertext
		AES_ecb_encrypt(in, out, &prv_key, AES_ENCRYPT);
		memcpy(ct + (i*AES_BLOCK_SIZE) , out , AES_BLOCK_SIZE);
	}

	free(in);
	free(out);

	return;
}

void cbc_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[KEY_SIZE] , uint8_t iv[AES_BLOCK_SIZE])
{
	/* Buffers */
	uint8_t * in = (uint8_t *)malloc(AES_BLOCK_SIZE * sizeof(uint8_t));
	uint8_t * out = (uint8_t *)malloc(AES_BLOCK_SIZE * sizeof(uint8_t));
	int cycles = bytes / AES_BLOCK_SIZE;

	/* Prepare AES-128 bit ECB Encryption */
	AES_KEY prv_key;
	AES_set_decrypt_key(key, KEY_SIZE*8, &prv_key); // Size of key is in bits
	
	/*First one, xor with iv */
	memcpy(in , ct , AES_BLOCK_SIZE);
	AES_ecb_encrypt(in, out, &prv_key, AES_DECRYPT);
	out = fixed_xor(out, iv, AES_BLOCK_SIZE);
	memcpy(pt , out , AES_BLOCK_SIZE);

	for(int i = 1 ; i < cycles ; i++){
		memcpy(in , ct + (i*AES_BLOCK_SIZE) , AES_BLOCK_SIZE);
		AES_ecb_encrypt(in, out, &prv_key, AES_DECRYPT);
		out = fixed_xor(out, ct + ((i-1)*AES_BLOCK_SIZE) , AES_BLOCK_SIZE);
		memcpy(pt + (i*AES_BLOCK_SIZE), out , AES_BLOCK_SIZE);
	}

	free(in);
	free(out);

	return;
}


void ctr_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[KEY_SIZE] , uint64_t nonce)
{
	uint64_t in[2]; //So we just divide it in 2 easy. nonce || counter
	uint8_t * out = (uint8_t *)malloc(AES_BLOCK_SIZE * sizeof(uint8_t));
	int rem_bytes;
	
	//Fill buffer with nonce and counter
	in[0] = nonce;

	// Prepare AES-128 bit ECB Encryption 
	AES_KEY prv_key;
	AES_set_encrypt_key(key, KEY_SIZE * 8, &prv_key); // Size of key is in bits
	
	in[1] = 0; //Initialize counter
	for(int i = 0 ; i < bytes ; i+=AES_BLOCK_SIZE){
		AES_ecb_encrypt((uint8_t*)in, out, &prv_key, AES_ENCRYPT); //Encrypt the "iv" 

		rem_bytes = (bytes - i > 16) ? 16 : bytes - i; //Is there less than 16 bytes in this block?
	
		out = fixed_xor(out, pt + (i*AES_BLOCK_SIZE), rem_bytes); //XOR output with plaintext block	
		memcpy(ct + (i*AES_BLOCK_SIZE) , out , rem_bytes); //Copy it to ct
		in[1] += 1; //Increment counter
	}

	free(out);
}

void ctr_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[KEY_SIZE] , uint64_t nonce)
{
	ctr_encrypt(ct , pt , bytes , key , nonce);
	return;
}


void gen_key(uint8_t key[KEY_SIZE])
{
	for(int i = 0; i < KEY_SIZE ;i++)
	{
		key[i] = rand() % 256;
	}

	return;
}

uint8_t * pkcs(uint8_t * pt , int pt_size , int ct_size)
{
	int count = ct_size - pt_size;
	if(count <= 0)
		return NULL;

	pt = (uint8_t *)realloc(pt, ct_size);

	for(int i = pt_size ; i < ct_size ; i++){
		//pt[i] = 'H';
		//pt[i] = '\x00';
		pt[i] = count;
	}
	
	return pt;
}

uint8_t * pkcs_bef(uint8_t * pt , int pt_size , int ct_size)
{
	int count = ct_size - pt_size;
	if(count <= 0)
		return NULL;

	pt = (uint8_t *)realloc(pt, ct_size);

	for(int i = ct_size; i >= count ; i--){
		pt[i] = pt[i - count];
	}
	for(int i = 0 ; i < count ; i++){
		pt[i] = count;
	}
	
	return pt;
}

//Takes as input the plaintext and its size (pt_size)
uint8_t * validate_pkcs(uint8_t * pt , int * pt_size , int * valid)
{
	int count;

	//Whats the padding number? Lets check the last index
	count = pt[ (*pt_size) - 1];

	//Is it a pad number? If not, we can return!
	if(count <= 0 || count >= (AES_BLOCK_SIZE)){
		*valid = 1;
		return pt;
	}

	//Are all count numbers before the same?
	for(int i = (*pt_size) - 1 ; i > (*pt_size) - 1 - count ; i--){
		if(pt[i] != count){
			//Invalid!
			*valid = 0;
			return pt;
		}
	}

	//Otherwise, lets just remove them and return 
	pt = (uint8_t *)realloc(pt, (*pt_size) - count);
	(*pt_size) = (*pt_size) - count;
	*valid = 1;
	
	return pt;
}

uint8_t * ecb_cbc_encrypt(uint8_t * pt , int in_bytes , int * out_bytes)
{
	uint8_t key[KEY_SIZE];
	uint8_t iv[16];
	int count;
	uint8_t * ct = NULL;

	//Generate random key
	gen_key(key);

	//Append before and after
	count = 5 + (rand() % 6);
	pt = pkcs(pt , in_bytes , in_bytes + count);
	in_bytes += count;

	count = 5 + (rand() % 6);
	pt = pkcs_bef(pt , in_bytes , in_bytes + count);
	in_bytes += count;

	(*out_bytes) = in_bytes;
	//Correct to be multiple of AES_BLOCK_SIZE
	(*out_bytes) -= ( (*out_bytes) % AES_BLOCK_SIZE );
	
	//Malloc ct
	ct = (uint8_t *)malloc( (*out_bytes) * sizeof(uint8_t)); 
	
	//Encrypt
	count = rand() % 2;
	if(count == 0){ //ECB
		ecb_encrypt(pt , ct , (*out_bytes) , key);
	}else{
		gen_key(iv);
		cbc_encrypt(pt , ct , (*out_bytes) , key , iv);
	}
	
	return ct;
}
