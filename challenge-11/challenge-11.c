#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set2.h"
#include "../include/basics.h"
#include "../include/aes.h"

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
