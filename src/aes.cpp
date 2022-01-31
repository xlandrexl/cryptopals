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

void cbc_decrypt_file( char filename[FILENAMEBUFFER] , uint8_t local_key[KEY_SIZE] , uint8_t iv[AES_BLOCK_SIZE])
{
	//uint8_t local_key[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E'};
	//char filename[FILENAMEBUFFER] = "../files/set2-chal10.txt";
	//uint8_t iv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	int aes_input_size;
	uint8_t * aes_input = b64file2bytes(filename , &aes_input_size);
	
	/* Buffers for Decryption */
	uint8_t * dec_out = (uint8_t*)malloc(aes_input_size * sizeof(uint8_t));
	cbc_decrypt( aes_input , dec_out, aes_input_size , local_key , iv);

	print_char(dec_out , aes_input_size); //May be intersting to return as string!
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

uint8_t * validate_pkcs(uint8_t * pt , int pt_size , int ct_size)
{
	int count = ct_size - pt_size;
	if(count <= 0)
		return NULL;

	pt = (uint8_t *)realloc(pt, ct_size);

	for(int i = pt_size ; i < ct_size ; i++){
		pt[i] = count;
	}
	
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

/*int main()
{
	//srand((unsigned) time(&t));

	// Input data 
	uint8_t local_key[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E'};
	uint8_t iv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	char filename[FILENAMEBUFFER] = "../files/set2-chal10.txt";
	
	//cbc_decrypt_file(filename, local_key,iv);
	
	//int n = 13;
	uint8_t * pt = (uint8_t *)malloc(n * sizeof(uint8_t));
	for(int i = 0; i < n ; i++)
		pt[i] = local_key[i]; 

	printf("\nBefore: \t");
	print_hex(pt , n); 
	pt = pkcs_bef( pt , n , 16);
	printf("\n After: \t");
	print_hex(pt , 16);
	printf("\n");

	printf("\nBefore: \t");
	print_hex(local_key , 16); 
	gen_key(local_key);
	printf("\n After: \t");
	print_hex(local_key , 16); 
	

	//char filename[FILENAMEBUFFER] = "../files/set1-chal7.txt";
	//ecb_decrypt_file( filename, local_key);
	
	//int n = 32;
	uint8_t * pt = (uint8_t *)malloc(n * sizeof(uint8_t));
	uint8_t * ct = (uint8_t *)malloc(n * sizeof(uint8_t));
	uint8_t * rec = (uint8_t *)malloc(n * sizeof(uint8_t));
	for(int i = 0; i < n ; i++)
		pt[i] = 'A'; 

	printf("\nPlaintext: \t");
	print_char(pt , n); 

	cbc_encrypt(pt , ct , n , local_key , iv);
	printf("\nCiphertext: \t");
	print_char(ct , n); 

	cbc_decrypt(ct , rec , n , local_key , iv);
	printf("\nRecovered: \t");
	print_char(rec , n); 

	
	return 0;
}*/
