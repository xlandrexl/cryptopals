#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../include/basics.h"
#include "../include/xor_ciphers.h"

#define INF 1000


int get_score(uint8_t * bytes , int size_bytes)
{
	char c;
	int score = 0;

	for(int i = 0; i < size_bytes; i++){
		c = toupper( (char)bytes[i] );
		for(int j = 0; j < NCHARS; j++){
			if (c == english_famous_chars[j]){
				score++;
				break;
			}
		}
	}

	return score;
}
  

char break_single_byte_xor(uint8_t * bytes , int size_bytes)
{
	int k;
	int score;
	uint8_t * buf = NULL;

	int max_k;
	int max_score;

	buf = (uint8_t *)malloc(size_bytes * sizeof(uint8_t));	

	max_score = -1;
	for(k = 0 ; k < 256 ; k++){
		for(int i = 0; i < size_bytes; i++){
			buf[i] = bytes[i] ^ k;
		}
		
		//get points
		score = get_score(buf , size_bytes);

		//save best
		if(score > max_score){	
			max_score = score;
			max_k = k;
		}
	}

	//printf("Max key %d with char %c and score %d\n", max_k , (char)max_k , max_score);
	
	free(buf);

	return (char)max_k;
}

//We can make this not dependent on block! 
//Easy idea: bytes / max
//Harder idea: bytes / keysize, then normalize. 
#define BLOCKS 10

int * get_keysize_ranking(uint8_t * bytes , int min, int max)
{
	int keysize;
	float * distances = NULL;
	int * ranking = NULL;
	float min_val;
	int min_idx; 

	distances = (float *)malloc( max * sizeof(float) ); //0 to min will be unused, but much simpler.
	if(distances == NULL){
		printf("Error allocating memory for distances array.\n");
		return NULL;
	}	

	for(int i = 0; i < min ; i++){
		distances[i] = INF; //placeholder for unused values
	}
	for(int i = min; i < max ; i++){
		distances[i] = 0; //initialize
	}

	//Get hamming distance of every keysize
	for(keysize = min; keysize < max; keysize++){

		//Doing test multiple times
		for (int i = 0; i < BLOCKS; ++i){
			distances[keysize] += hamming_distance(bytes + (i * keysize), bytes + ((i+1) * keysize), keysize);
		}

		distances[keysize] /= keysize;
	}

	//Now lets make a rank (ordered array)
	ranking = (int *)malloc( (max-min+1) * sizeof(int) ); 
	if(ranking == NULL){
		printf("Error allocating memory for ranking array.\n");
		return NULL;
	}		
	
	//VERY not optimized!
	for(int i = 0; i < (max-min) ; i++){
		min_val = INF;
		min_idx = -1;
		//Loop to get biggest value and index in distances array
		for(int j = 0; j < max ; j++){
			if(distances[j] < min_val){
				min_val = distances[j];
				min_idx = j;
			}
		}
		//Put it in the ranking and decrease its value to not be triggered again
		ranking[i] = min_idx;
		distances[min_idx] = INF;

		//printf("Rank %d has keysize %d\n" , i, ranking[i]);
	}

	free(distances);
	
	return ranking;
}

char * break_repeat_key_xor(uint8_t * bytes , int bytes_size)
{
	uint8_t ** tr_blocks = NULL;
	int tr_blocks_size;
	char * key = NULL;
	int * key_ranking = NULL;
	int keysize;

	key_ranking = get_keysize_ranking(bytes , 2, 40);
	//key_ranking = get_keysize_ranking(bytes , 2, 10);
	keysize = key_ranking[0];
	
	//Alloc memory for transposed blocks
	tr_blocks_size = bytes_size / keysize;
	tr_blocks = (uint8_t**)malloc( keysize * sizeof(uint8_t*));
	for(int i = 0; i < keysize ; i++){
		tr_blocks[i] = (uint8_t*)malloc( tr_blocks_size * sizeof(uint8_t));
	}

	//Build transposed blocks
	for(int i = 0; i < keysize ; i++){
		for(int j = i , k = 0 ; k < tr_blocks_size ; j += keysize , k++){
			tr_blocks[i][k] = bytes[j];
		}
	}

	//Break key of each block as single byte xor
	key = (char*)malloc( (keysize+1) * sizeof(char));
	key[keysize] = '\0';

	for(int i = 0; i < keysize ; i++){
		key[i] = break_single_byte_xor(tr_blocks[i] , tr_blocks_size);
	}
	
	//Free memory
	free(key_ranking);
	for(int i = 0; i < keysize ; i++)
		free(tr_blocks[i]);
	free(tr_blocks);	

	return key;
}

/*b64 it
int main()
{	
	//char pt_asci[] = "Now transpose the blocks: make a block that is the first byte of every block, and a block that is the second byte of every block, and so on.";
	//char mykey[] = "Hello";
	//char * ct_asci; //060a1b4c1b3a04021f1f2716094c1b20004c0e032706071f5568080d070a68044c0e032706074c1b2004184c063b4518040a6803051e1c3c450e151b2d45030a4f2d13091e16680700030c23494c0d012c450d4c0d240a0f074f3c0d0d184f21164c18072d451f090c270b084c0d3111094c002e45091a0a3a1c4c0e03270607404f290b084c1c2745030241
	//char * ct_b64;
	
	int bytes_size;
	uint8_t * bytes;
	char * key;
	char filename[124] = "../files/set1-chal6.txt";
	uint8_t * plaintext;

	//Encrypt it and b64 it
	//ct_asci = repeat_key_xor_cipher(pt_asci,mykey);
	//bytes = hex2bytes(ct_asci, &bytes_size);
	//ct_b64 = bytes2b64(bytes, bytes_size);
	//bytes = b642bytes(ct_b64, &bytes_size);

	bytes = b64file2bytes(filename , &bytes_size);

	//Get key
	key = break_repeat_key_xor(bytes , bytes_size);

	//Decrypt file
	plaintext = repeat_key_xor_cipher(bytes, bytes_size, (uint8_t *)key, strlen(key));

	printf("key: %s\n" , key);
	print_char(plaintext, bytes_size);

	free(key);
	free(plaintext);

	return 0;
}*/
