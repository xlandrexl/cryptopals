#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/basics.h"
#include "../include/xor_ciphers.h"
#include "../include/aes.h"

int challenge9(char * pt , int n);
int challenge10(char * filename , char * key);
int challenge11(char * filename , int tries);
int challenge12(void);
//challenge13 not done
//challenge14 not done
int challenge15(char * string);
//challenge16 not done

int main()
{
	int ret = 1;

	char filename[] = "../files/set2-chal9.txt";
	int tries = 1000;
	ret = challenge12();

	return ret;
}

int challenge9(char * pt , int n)
{
	//char pt[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E','\0'};
	//int n = 20;

	char * str_out;
	int bytes_size;
	uint8_t * bytes;

	bytes = string2bytes(pt, &bytes_size);
	bytes = pkcs( bytes , bytes_size , n);
	bytes_size = n;

	str_out = bytes2string(bytes , bytes_size);

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

	aes_input = b64file2bytes(filename , &aes_input_size);

	dec_out = (uint8_t*)malloc(aes_input_size * sizeof(uint8_t));
	cbc_decrypt( aes_input , dec_out, aes_input_size , local_key , iv);

	str_out = bytes2string(dec_out , aes_input_size);

	printf("%s" , str_out);

	free(str_out);
	free(dec_out);
	free(aes_input);

	return 1;
}

int challenge11(char * filename , int tries)
{
	//char filename[] = "../files/set2-chal9.txt";
	//int tries = 20;

	uint8_t * pt = NULL;
	uint8_t * ct = NULL;
	int pt_size = 0;
	int ct_size = 0;
	int mode = 0;
	int cols = 0;
	int correct = 0;

	pt = b64file2bytes(filename , &pt_size);

	for(int i = 0; i < tries; i++){

		ct = ecb_cbc_encrypt_oracle(pt , pt_size , &ct_size , &mode);
		cols = count_colisions(ct , ct_size);
		free(ct);

		if(cols > 0){ //AES-ECB detected!
			if(mode == 1){ //If in fact was ECB
				correct++;
			}
		}else{ //AES-CBC detected!
			if(mode == 2){ //If in fact was CBC
				correct++;
			}
		}
	}

	//Print statistics
	//printf("Correct guesses: %d / %d (%.2f%%)\n" , correct , tries, (float)correct/tries * 100);

	free(pt);

	return 1;
}

int challenge12()
{
	char filename[] = "../files/set2-chal9.txt";
	uint8_t * pt = NULL;
	uint8_t * ct = NULL;
	uint8_t * tmp = NULL;
	uint8_t * tmp_ct = NULL;
	int pt_size = 0;
	int ct_size = 0;
	int tmp_size = 0;
	int tmp_ct_size = 0;

	int max_block_size = 128;
	int prev_ct_size;
	int block_size;

	//Discover block size (Done)
	pt = (uint8_t *)malloc(max_block_size * sizeof(uint8_t));
	for(int i = 1; i < max_block_size; i++)
	{
		ct = ecb_same_key_encrypt_oracle(pt , i , &ct_size);
		//printf("Input size: %d | Output size: %d\n" , i , ct_size);
		if(i != 1 && ct_size != prev_ct_size){
			block_size = ct_size - prev_ct_size;
			free(ct);
			break;
		}
		prev_ct_size = ct_size;
		free(ct);
	}
	free(pt);
	//printf("Block size: %d\n" , block_size);

	//Discover AES (done)

	//Craft input with one byte short
	pt_size = 15;
	pt = (uint8_t *)malloc(pt_size * sizeof(uint8_t));
	for(int i = 0 ; i < pt_size ; i++){
		pt[i] = 'A';
	}

	ct = ecb_same_key_encrypt_oracle(pt , pt_size , &ct_size); //First block of ciphertext will have first byte of unknown string in its last position

	//Brute force all possible last bytes to see which is similar
	tmp_size = 16;
	tmp = (uint8_t *)malloc(tmp_size * sizeof(uint8_t));
	memcpy(tmp , pt , pt_size);
	for(int i = 0 ; i < 256 ; i++){
		tmp[15] = i;
		//Encrypt this tmp
		tmp_ct = ecb_same_key_encrypt_oracle(tmp , tmp_size , &tmp_ct_size);
		if(memcmp(tmp , tmp_ct , 16) == 0){
			printf("Byte found: %c\n" , (char)i);
			free(tmp_ct);
			break;
		}
		free(tmp_ct);
	}

	free(pt);
	free(tmp);

	return 1;
}

int challenge13(){
	uint8_t key[] = "YELLOW SUBMARINE";
	uint8_t * ct;
	uint8_t * pt;
	int pt_size;
	int ct_size;
	char mail[] = "foo@bar.com";	//May allow user to provide

	char * encoded_user = profile_for(mail); //Encode. Dont forget to free this.
	pt * string2bytes(encoded_user , pt_size);
	free(encoded_user);

	//Encrypt
	ecb_encrypt_pad(pt , pt_size , ct , &ct_size , key);
	free(pt);

	/* I CAN ALTER THE CIPHERTEXT */
	//I will detect how long is the block with what I already have.
	//I choose a mail that isolates "user" in its own block
	//I choose a mail with admin PKCS#7 in its own block.
	//I will copy paste it! Contatenate

	//Decrypt
	ecb_decrypt_pad(ct , ct_size , pt , &pt_size , key);
	free(ct);

	//Parse
	encoded_user = bytes2string(pt , pt_size);
	free(pt);

	print_json(parse_json(encoded_user));

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
		free(bytes);
		return 1;
	}

	str_out = bytes2string(bytes , bytes_size);

	printf("%s" , str_out);

	free(str_out);
	free(bytes);

	return 1;
}

/*int main()
{
	int ret = 1;

	char pt[]="YELLOW SUBMARINE";
	int pad = 20;
	ret = challenge9(pt , pad);

	char filename[] = "../files/set2-chal10.txt";
	char key[] = "YELLOW SUBMARINE";
	ret = challenge10(filename , key);

	char pt1[]="ICE ICE BABY\x04\x04\x04\x04";
	char pt2[]="ICE ICE BABY\x05\x05\x05\x05";
	char pt3[]="ICE ICE BABY\x01\x02\x03\x04";
	ret = challenge15(pt1);
	ret = challenge15(pt2);
	ret = challenge15(pt3);

	return ret;
}*/
