#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set3.h"
#include "../include/basics.h"
#include "../include/aes.h"

int challenge20(char * filename)
{
	//char filename[] = "../files/set3-chal20.txt";

	uint8_t ** cts;
	int * cts_size;
	int ncts;

	int cts_size_smaller;
	uint8_t * ct_cnt;
	char * key;

	uint8_t * rec_pt;

	cts = same_key_ctr_encrypt_b64_strings(filename , &ncts , &cts_size);

	//Truncate to smallest ct
	cts_size_smaller = cts_size[0];
	for(int i = 0 ; i < ncts ; i++){
		if(cts_size[i] < cts_size_smaller){
			cts_size_smaller = cts_size[i];
		}
	}

	//Concatenate
	ct_cnt = (uint8_t *)malloc(ncts * cts_size_smaller * sizeof(uint8_t));
	for(int i = 0 ; i < ncts ; i++){
		memcpy(ct_cnt + i*cts_size_smaller, cts[i] , cts_size_smaller);
	}

	key = break_repeat_key_xor(ct_cnt , ncts * cts_size_smaller , cts_size_smaller); //-1 for call

	printf("key: %s\n", key);

	//Confirm
	rec_pt = (uint8_t *)malloc(cts_size_smaller * sizeof(uint8_t));
	memcpy(rec_pt , cts[0], cts_size_smaller);
	rec_pt = fixed_xor(rec_pt , (uint8_t *) key, strlen(key));
	//print_char(rec_pt , cts_size_smaller);
	free(rec_pt);

	//Free memory
	for(int i = 0 ; i < ncts ; i++){
		free(cts[i]);
	}

	free(cts);
	free(ct_cnt);
	free(cts_size);

	return 1;
}
