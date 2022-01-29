#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../include/basics.h"
#include "../include/xor_ciphers.h"

#define BUFSIZE 124
#define INF 1000

int * get_keysize_ranking(uint8_t * bytes , int min, int max)
{
	int keysize;
	char buf1[BUFSIZE];
	char buf2[BUFSIZE];
	float * distances = NULL;
	int * ranking = NULL;
	float min_val;
	int min_idx; 

	distances = (float *)malloc( max * sizeof(float) ); //from 0 to min will be unused, but much simpler.
	if(distances == NULL){
		printf("Error allocating memory for distances array.\n");
		return NULL;
	}	
	for(int i = 0; i < max ; i++){
		distances[i] = INF; //placeholder for unused values. Not really necessary
	}

	//Get hamming distance of every keysize
	for(keysize = min; keysize < max; keysize++){
		for(int i = 0; i < keysize ; i++){
			buf1[i] = bytes[i];
			buf2[i] = bytes[i+keysize];
		}

		buf1[keysize] = '\0';
		buf2[keysize] = '\0';

		distances[keysize] = hamming_distance_str(buf1, buf2);

		printf("| Keysize %d | " , keysize);
		print_hex((uint8_t *)buf1 , keysize);
		printf(" | ");
		print_hex((uint8_t *)buf2 , keysize);
		printf(" | Dist %d | Norm Dist %.3f |\n" , (int)distances[keysize] , distances[keysize] / keysize);

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

	//key_ranking = get_keysize_ranking(bytes , 2, 40);
	key_ranking = get_keysize_ranking(bytes , 2, 10);
	keysize = key_ranking[0];
	//keysize = 5;
	
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

	//for(int i = 0; i < keysize ; i++){printf("Char %2d of key: " , i); print_hex(tr_blocks[i] , tr_blocks_size);}
	key = (char*)malloc( keysize * sizeof(char));
	for(int i = 0; i < keysize ; i++){
		key[i] = single_byte_xor_cipher(tr_blocks[i] , tr_blocks_size);
	}

	printf("key: %s\n" ,key);

	free(key_ranking);
	for(int i = 0; i < keysize ; i++)
		free(tr_blocks[i]);
	free(tr_blocks);

	return key;
}

char * break_repeat_key_xor_file(char * filename)
{
	uint8_t * bytes;
	int bytes_size;
	char * key;

	bytes = b64file2bytes(filename , &bytes_size);

	//Call main functions
	key = break_repeat_key_xor(bytes , bytes_size);
	free(bytes);
	
	return key;
}


