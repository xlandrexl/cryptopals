#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set2.h"
#include "../include/basics.h"
#include "../include/aes.h"

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
