#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set2.h"
#include "../include/basics.h"
#include "../include/aes.h"

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
