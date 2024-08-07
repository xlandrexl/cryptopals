#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set3.h"
#include "../include/basics.h"
#include "../include/aes.h"

int challenge17(void)
{
	uint8_t iv[AES_BLOCK_SIZE];
	uint8_t * ct;
	int ct_size;
	uint8_t * pt;
	uint8_t * ctc;
	uint8_t ibyte[AES_BLOCK_SIZE];
	int ci;
	//int pi;
	int pad;
	int max_blocks;

	//Get ciphertext and make copy
	ct = cbc_pad_oracle_encrypt(&ct_size , iv);
	ctc = (uint8_t *)malloc(ct_size * sizeof(uint8_t));
	pt = (uint8_t *)malloc(ct_size * sizeof(uint8_t));

	//Put iv in beggining and then shift
	memcpy(ctc,iv, AES_BLOCK_SIZE);
	memcpy(ctc+AES_BLOCK_SIZE,ct,ct_size-AES_BLOCK_SIZE);

	max_blocks = ct_size / 16;

	for(int blocks = 0 ; blocks < max_blocks - 1 ; blocks++){
		for(int k = 15 ; k >= 0 ; k--){
			pad = AES_BLOCK_SIZE - k;

			ci = k + blocks*AES_BLOCK_SIZE; //Index we're dealing with

			for(int i = ci+1 ; i % AES_BLOCK_SIZE != 0 ; i++){
				ctc[i] = ibyte[i % AES_BLOCK_SIZE] ^ pad;
			}

			for(int i = 0 ; i < 256 ; i++){
				ctc[ci] = i;
				if(cbc_pad_oracle_check(ctc + blocks*AES_BLOCK_SIZE , 32 , iv) == 1){
					ibyte[k] = ctc[ci] ^ pad;
					pt[ci] = ibyte[k] ^ ct[ci+AES_BLOCK_SIZE];
					//printf("Itx %d - Recovered P[%d] = %c\n" , i,pi+ blocks*AES_BLOCK_SIZE , (char)pt[pi+ blocks*AES_BLOCK_SIZE]);
					break;
				}
			}
		}
	}

	//print_hex(pt + 16 , ct_size);
	print_char(pt , ct_size);
	printf("\n");

	free(pt);
	free(ct);
	free(ctc);

	return 1;
}
