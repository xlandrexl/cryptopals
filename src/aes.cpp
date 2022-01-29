#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>

#include "../include/basics.h"
#include "../include/aes.h"
#include "../include/xor_ciphers.h"

/* AES key for Encryption and Decryption */
const static unsigned char aes_key[]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};

void ecb_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[16])
{
	/* Buffers */
	uint8_t in[16];
	uint8_t out[16];
	int cycles = bytes / 16;

	/* Prepare AES-128 bit ECB Encryption */
	AES_KEY prv_key;
	AES_set_encrypt_key(key, sizeof(aes_key)*8, &prv_key); // Size of key is in bits
	
	for(int i = 0 ; i < cycles ; i++){
		memcpy(in , pt + (i*16) , 16);
		AES_ecb_encrypt(in, out, &prv_key, AES_ENCRYPT);
		memcpy(ct + (i*16) , out , 16);
	}

	return;
}

void ecb_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[16])
{
	/* Buffers */
	uint8_t in[16];
	uint8_t out[16];
	int cycles = bytes / 16;

	/* AES-128 bit ECB Decryption */
	AES_KEY prv_key;
	AES_set_decrypt_key(key, sizeof(aes_key)*8, &prv_key); // Size of key is in bits

	for(int i = 0 ; i < cycles ; i++){
		memcpy(in , ct + (i*16) , 16);
		AES_ecb_encrypt(in, out, &prv_key, AES_DECRYPT);
		memcpy(pt + (i*16) , out , 16);
	}
}

void ecb_decrypt_file( char filename[FILENAMEBUFFER] , uint8_t local_key[16])
{
	//uint8_t local_key[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E'};
	//char filename[FILENAMEBUFFER] = "../files/set1-chal7.txt";

	int aes_input_size;
	uint8_t * aes_input = b64file2bytes(filename , &aes_input_size);
	
	/* Buffers for Decryption */
	uint8_t * dec_out = (uint8_t*)malloc(aes_input_size * sizeof(uint8_t));
	ecb_decrypt( aes_input , dec_out, aes_input_size , local_key);

	print_char(dec_out , aes_input_size); //May be intersting to return as string!
	return;
}


void cbc_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[16] , uint8_t iv[16])
{
	/* Buffers */
	uint8_t * in = (uint8_t *)malloc(16 * sizeof(uint8_t));
	uint8_t * out = (uint8_t *)malloc(16 * sizeof(uint8_t));
	int cycles = bytes / 16;

	/* Prepare AES-128 bit ECB Encryption */
	AES_KEY prv_key;
	AES_set_encrypt_key(key, sizeof(aes_key)*8, &prv_key); // Size of key is in bits
	
	/*First one, xor with iv */
	memcpy(in , pt , 16);
	in = fixed_xor(in, iv, 16);
	AES_ecb_encrypt(in, out, &prv_key, AES_ENCRYPT);
	memcpy(ct , out , 16);

	for(int i = 1 ; i < cycles ; i++){
		memcpy(in , pt + (i*16) , 16);
		in = fixed_xor(in, ct + ((i-1)*16) , 16); //XOR with previous ciphertext
		AES_ecb_encrypt(in, out, &prv_key, AES_ENCRYPT);
		memcpy(ct + (i*16) , out , 16);
	}

	free(in);
	free(out);

	return;
}

void cbc_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[16] , uint8_t iv[16])
{
	/* Buffers */
	uint8_t * in = (uint8_t *)malloc(16 * sizeof(uint8_t));
	uint8_t * out = (uint8_t *)malloc(16 * sizeof(uint8_t));
	int cycles = bytes / 16;

	/* Prepare AES-128 bit ECB Encryption */
	AES_KEY prv_key;
	AES_set_decrypt_key(key, sizeof(aes_key)*8, &prv_key); // Size of key is in bits
	
	/*First one, xor with iv */
	memcpy(in , ct , 16);
	AES_ecb_encrypt(in, out, &prv_key, AES_DECRYPT);
	out = fixed_xor(out, iv, 16);
	memcpy(pt , out , 16);

	for(int i = 1 ; i < cycles ; i++){
		memcpy(in , ct + (i*16) , 16);
		AES_ecb_encrypt(in, out, &prv_key, AES_DECRYPT);
		out = fixed_xor(out, ct + ((i-1)*16) , 16);
		memcpy(pt + (i*16), out , 16);
	}

	free(in);
	free(out);

	return;
}

void cbc_decrypt_file( char filename[FILENAMEBUFFER] , uint8_t local_key[16] , uint8_t iv[16])
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

void gen_key(uint8_t key[16])
{
	for(int i = 0; i < 16 ;i++)
	{
		key[i] = rand() % 256;
	}

	return;
}

int main()
{
	//srand((unsigned) time(&t));

	/* Input data */
	uint8_t local_key[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E'};
	uint8_t iv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	char filename[FILENAMEBUFFER] = "../files/set2-chal10.txt";
	
	cbc_decrypt_file(filename, local_key,iv);

	//char filename[FILENAMEBUFFER] = "../files/set1-chal7.txt";
	//ecb_decrypt_file( filename, local_key);
	
	/*int n = 32;
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
	print_char(rec , n); */

	
	
	return 0;
}
