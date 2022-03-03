#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "../include/basics.h"
#include "../include/xor_ciphers.h"
#include "../include/aes.h"
#include "../include/mtwister.h"

int challenge17(void); //Functions done. There is one error in the optimization.
int challenge18(char * in , char * key , int nounce);
int challenge19(void); // Functions done. In progress... But same as 20.
int challenge20(void); // Done. How to prove better?
int challenge21(int seed);
int challenge22();
//challenge23 not done
//challenge24 not done


/*int main()
{
	int ret = 0;

	ret = challenge20();

	return ret;
}*/

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

/*Get ciphertext and make copy
ct = cbc_pad_oracle_encrypt(&ct_size , iv);
ctc = (uint8_t *)malloc(ct_size * sizeof(uint8_t));
pt = (uint8_t *)malloc(ct_size * sizeof(uint8_t));
memcpy(ctc,ct,ct_size);

max_blocks = ct_size / 16;

for(int blocks = 0 ; blocks < max_blocks - 1 ; blocks++){
	for(int k = 15 ; k >= 0 ; k--){
		pad = AES_BLOCK_SIZE - k;
		//lets start with 2 blocks...
		ci = k + blocks*AES_BLOCK_SIZE;
		pi = ci + AES_BLOCK_SIZE + blocks*AES_BLOCK_SIZE;

		for(int i = ci+1 ; i % 16 != 0 ; i++){
			ctc[i] = ibyte[i % 16] ^ pad;
		}

		for(int i = 0 ; i < 256 ; i++){
			ctc[ci] = i;
			if(cbc_pad_oracle_check(ctc + blocks*AES_BLOCK_SIZE , 32 , iv) == 1){
				ibyte[k] = ctc[ci] ^ pad;
				pt[pi] = ibyte[k] ^ ct[ci];
				//printf("Itx %d - Recovered P[%d] = %c\n" , i,pi+ blocks*AES_BLOCK_SIZE , (char)pt[pi+ blocks*AES_BLOCK_SIZE]);
				break;
			}
		}
	}
}*/

int challenge18(char * in , char * key , int nounce)
{
	//char key[]="YELLOW SUBMARINE";
	//int nounce = 0;
	//char in[] = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ=="

	int pt_size;
	uint8_t * pt;
	uint8_t * ct;
	char * str_out;

	ct = b642bytes(in, &pt_size);
	pt = (uint8_t *)malloc(pt_size * sizeof(uint8_t));

	if(strlen(key) != 16){
		printf("Key is not 16 bytes\n. Exiting...\n");
		return -1;
	}
	ctr_decrypt(ct , pt , pt_size , (uint8_t *)key , nounce);

	str_out = bytes2string(pt , pt_size);

	printf("%s" , str_out);

	free(str_out);
	free(pt);
	free(ct);

	return 1;
}

int challenge19(){
	char filename[] = "../files/set3-chal19.txt";
	char ** b64lines;
	uint8_t ** pts;
	int * pts_size;
	uint8_t ** cts;
	int nlines;

	//Read lines  of file
	b64lines = file2strings(filename , &nlines);
	nlines--;//To remove one final wrong line... sorry!

	//Malloc structures
	pts = (uint8_t **)malloc(nlines * sizeof(uint8_t *));
	cts = (uint8_t **)malloc(nlines * sizeof(uint8_t *));
  pts_size = (int*)malloc(nlines * sizeof(int));

	//Decode lines of file and encrypt
	for(int i = 0 ; i < nlines ; i++){
		pts[i] = b642bytes(b64lines[i], &(pts_size[i]) );
		cts[i] = (uint8_t *)malloc(pts_size[i] * sizeof(uint8_t));
		same_key_ctr_encrypt(pts[i] , cts[i] , pts_size[i]);
	}

	/* Find the keystream */
	/* Get plaintext */

	//Free memory
	for(int i = 0 ; i < nlines ; i++){
		free(b64lines[i]);
		free(pts[i]);
		free(cts[i]);
	}
	free(b64lines);
	free(pts);
	free(cts);
	free(pts_size);

	return 1;
}

int challenge20(){
	char filename[] = "../files/set3-chal20.txt";

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

int challenge21(int seed)
{
	init_genrand(seed);
	printf("%d\n" , (int)genrand_int32());
	return 1;
}

int challenge22()
{
	int bef = (time(0));
	//printf("bef: %d\n" , bef);
	int d1 = 40 + rand() % (1000-40);
	int seed = bef + d1;
	init_genrand(seed);
	//int d2 = 40 + rand() % (1000-40);

	int out = (int)genrand_int32();

	//Ok... How many seconds have elapsed at max?
	int d = 2000;
	int now = (time(0));
	//printf("now: %d\n", now);
	int cmp;

	for(int i = -d ; i < d ; i++){
		init_genrand(now-i);
		cmp = (int)genrand_int32();
		if(cmp == out){
			printf("Seed found: %d\n" , now-i);
			return 1;
		}
	}

	printf("Seed not found\n");

	return 0;
}
