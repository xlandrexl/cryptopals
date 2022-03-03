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
int challenge12(void); //Works! Can be broken into functions. And leaves trail of A's
int challenge13(void);
//challenge14 not done //Ideas in internet! (ECB Byte at a time (Hard))
int challenge15(char * string);
int challenge16(void);

/*int main()
{
	int ret = 1;

	//char filename[] = "../files/set2-chal9.txt";
	//int tries = 1000;
	ret = challenge16();

	return ret;
}*/

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
	printf("Correct guesses: %d / %d (%.2f%%)\n" , correct , tries, (float)correct/tries * 100);

	free(pt);

	return 1;
}

int challenge12()
{
	uint8_t * pt = NULL;
	uint8_t * ct = NULL;
	uint8_t * tmp = NULL;
	uint8_t * tmp_ct = NULL;
	int pt_size = 0;
	int ct_size = 0;
	int tmp_size = 0;
	int tmp_ct_size = 0;
	int cols = 0;
	int max_block_size = 128;
	int prev_ct_size = -1;
	int block_size;

	int string_blocks;
	uint8_t * string_bytes;

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
	printf("Block size: %d\n" , block_size);

	//Discover AES (done)
	pt_size = max_block_size * 16;
	pt = (uint8_t *)malloc(pt_size * sizeof(uint8_t));
	for(int i = 0; i < pt_size ; i++){
 		pt[i] = 'A';
	}
	ct = ecb_same_key_encrypt_oracle(pt , pt_size , &ct_size);
	cols = count_colisions(ct , ct_size);
	if(cols > 0){
		printf("AES-ECB Detected\n");
	}
	free(pt);
	free(ct);

	//Discover length of strings
	ct = ecb_same_key_encrypt_oracle(pt , 0 , &string_blocks);
	string_blocks = string_blocks / 16;
	printf("String blocks: %d\n" , string_blocks);

	//Discover Bytes Loop
	string_bytes = (uint8_t *)malloc(string_blocks * 16 * sizeof(uint8_t));
	for(int blocks = 0; blocks < 2 ; blocks++ )
	{
		//Discover Bytes' bits Loop
		for(int k = 0 ; k < 15 ; k++){
			//Craft input with one byte short
			pt_size = 15-k;
			pt = (uint8_t *)malloc(pt_size * sizeof(uint8_t));
			for(int i = 0 ; i < pt_size; i++){
				pt[i] = 'A';
			}

			ct = ecb_same_key_encrypt_oracle(pt , pt_size , &ct_size); //First block of ciphertext will have first byte of unknown string in its last position

			//Brute force all possible last bytes to see which is similar
			tmp_size = 16;
			tmp = (uint8_t *)malloc(tmp_size * sizeof(uint8_t));
			memcpy(tmp , pt , pt_size);

			//Copy already discovered bytes
			for(int i = 15-k,z=0 ; i < 15 ; i++, z++){
				tmp[i] = string_bytes[16*blocks + z];
			}

			for(int i = 0 ; i < 256 ; i++){
				tmp[15] = i;
				//Encrypt this tmp
				tmp_ct = ecb_same_key_encrypt_oracle(tmp , tmp_size , &tmp_ct_size);

				if(memcmp(ct + (blocks*16) , tmp_ct , 16) == 0){
					//printf("Byte found: %c (%d)\n" , (char)i , i);
					string_bytes[16*blocks + k] = i;
					free(tmp_ct);
					break;
				}
				free(tmp_ct);
			}
			free(pt);
			free(tmp);
		}
	}

	print_char(string_bytes, string_blocks * 16);
	printf("\n");

	free(string_bytes);

	return 1;
}

int challenge13()
{
	uint8_t * ct1;
	int ct1_size;
	uint8_t * ct2;
	int ct2_size;
	Json user;
	uint8_t * ct_final;
	int ct_final_size;

	//Users are encoded as: email=foo@bar.com&uid=10&role=user
	//email=foo@bar.com&uid=10&role=  has 30 bytes. Lets add 2 more to make 32.
	//email=foooo@bar.com&uid=10&role=  has 32 bytes. (BLOCKS [A][B])
	char mail1[] = "foooo@bar.com";
	ct1 = ecb_encrypt_encode(mail1 , &ct1_size);
	//user = ecb_decrypt_parse(ct1 ,ct1_size);print_json(user);printf("\n\n");

	//fo@bar.comadmin isolate  admin__________ in one BLOCK [C]
	char mail2[] = "fo@bar.comadmin\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B";
	ct2 = ecb_encrypt_encode(mail2 , &ct2_size);
	//user = ecb_decrypt_parse(ct2 ,ct2_size);print_json(user);printf("\n\n");

	//Paste block at its place [A][B][C]
	ct_final_size = 3 * 16;
	ct_final = (uint8_t *)malloc(ct_final_size * sizeof(uint8_t));

	for(int i = 0; i < 32; i++){
		ct_final[i] = ct1[i];
	}
	for(int i = 32; i < 48; i++){
		ct_final[i] = ct2[i-16];
	}

	user = ecb_decrypt_parse(ct_final , ct_final_size);
	free(ct1);
	free(ct2);
	free(ct_final);

	print_json(user);

	return 1;
}

//Challenge 14 not done

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

int challenge16()
{
	uint8_t iv[AES_BLOCK_SIZE] = "GREEN SUBMARINE";
	char string[] = "I dont really care about this string";
	uint8_t * ct;
	int ct_size;
	uint8_t pt1[AES_BLOCK_SIZE] = "comment1=cooking";
	uint8_t ptd[AES_BLOCK_SIZE] = "aa;admin=true;aa";
	uint8_t ctd[AES_BLOCK_SIZE];
	uint8_t ivd[AES_BLOCK_SIZE];
	int ret;

	ct = cbc_flip_encrypt_oracle(string , &ct_size , iv);

  //ctd = pt1 XOR iv
	for(int i = 0; i < AES_BLOCK_SIZE; i++){
		ctd[i] = pt1[i] ^ iv[i];
	}

	//ivd = ctd XOR ptd
	for(int i = 0; i < AES_BLOCK_SIZE; i++){
		ivd[i] = ctd[i] ^ ptd[i];
	}

	ret = cbc_flip_check_oracle( ct , ct_size , ivd);

	if(ret == 1){
		printf("Found! I am admin.\n");
	}else{
		printf("Not found! I am not admin.\n");
	}

	free(ct);

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
