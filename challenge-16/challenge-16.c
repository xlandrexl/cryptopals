#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set2.h"
#include "../include/basics.h"
#include "../include/aes.h"

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
