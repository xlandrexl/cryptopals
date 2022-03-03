#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../include/basics.h"
#include "../include/xor_ciphers.h"

#define INF 1000
#define NCHARS 27
//English characters. Used to distinguish english-written plaintexts.
static const char english_famous_chars[NCHARS] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', ' '};

//Counts how many elements in an array of bytes correspond to a character in the english_famous_char array.
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

//Breaks single byte xor and returns the key char. Given an array of bytes as ciphertext, check which character as a key would result in a plaintext resembling the english language the most. Uses the number of english textual characters as metric.
char break_single_byte_xor(uint8_t * bytes , int size_bytes)
{
	int k;
	int score;
	uint8_t * buf = NULL;

	int max_k;
	int max_score;

	buf = (uint8_t *)malloc(size_bytes * sizeof(uint8_t));

	max_k = -1;
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

//Given an array of bytes of ciphertext, and min/max values for the keysize, produces a ranking of keysizes where the position 0 has the most-likely keysize. The metric used is the hamming distance across keysize-sized blocks of ciphertext.
int * get_keysize_ranking(uint8_t * bytes , int bytes_size , int min, int max)
{
	int keysize;
	float * distances = NULL;
	int * ranking = NULL;
	float min_val;
	int min_idx;
	int blocks = bytes_size / max; //Number of blocks used to guess the keysize. Can be improved by using each keysize and then normalizing.

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
		for (int i = 0; i < (blocks - 1); ++i){
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

//Breaks repeating key xor encryption. Estimates the keysize using get_keysize_ranking(..), and estimates each byte of the key using break_single_byte_xor(...) to the concatenation of all the bytes encrypted with each key character.
char * break_repeat_key_xor(uint8_t * bytes , int bytes_size , int fixed_keysize) //-1 for call
{
	uint8_t ** tr_blocks = NULL;
	int tr_blocks_size;
	char * key = NULL;
	int * key_ranking = NULL;
	int keysize;

	if(fixed_keysize == -1){
		key_ranking = get_keysize_ranking(bytes , bytes_size , 2, 40); //lavesc on tamano entre 2 y 40
		//key_ranking = get_keysize_ranking(bytes , 2, 10);
		keysize = key_ranking[0];
	}else{
		keysize = fixed_keysize;
	}


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
