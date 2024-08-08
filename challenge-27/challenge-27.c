#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "../include/set4.h"
#include "../include/basics.h"
#include "../include/aes.h"
#include "../include/xor_ciphers.h"

int challenge27(void){
	int pt_size = 3 * AES_BLOCK_SIZE;
	uint8_t * pt;
	int ct_size;
	uint8_t * ct;
	uint8_t * ct_m;
	int error = 0;
	uint8_t * key;

	//Lets encrypt a message 3 blocks long
	pt = (uint8_t *)malloc(pt_size * sizeof(uint8_t));
	for(int i = 0; i < pt_size; i++){
		pt[i] = i;
	}

	ct = cbc_key_is_iv_encrypt_oracle(pt , pt_size , &ct_size);
	free(pt);

	//printf("Pt_size=%d , ct_size=%d\n", pt_size, ct_size);

	//Lets modify the message
	ct_m = (uint8_t *)malloc(ct_size * sizeof(uint8_t));

	memcpy(ct_m , ct , ct_size);
	for(int i = 16; i < 32; i++){
		ct_m[i] = 0;
	}
	memcpy(ct_m + 32 , ct , 16);

	pt = cbc_key_is_iv_decrypt_oracle(ct_m , ct_size , &pt_size , &error);

	if(error == 0){
		printf("Error not found!\n");
	}else{
		key = (uint8_t *)malloc(16 * sizeof(uint8_t));
		memcpy(key, pt,16);
		key = fixed_xor(key, pt+32,16);
		printf("\n");
		print_char(key,16);
		free(key);
	}

	free(ct);
	free(ct_m);
	free(pt);

	return 1;
}
