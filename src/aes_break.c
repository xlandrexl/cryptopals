#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

#include "../include/basics.h"
#include "../include/aes.h"
#include "../include/xor_ciphers.h"

//Counts how many colisions of 16-byte blocks exist in a array of bytes with bytes_size elements.
//That is, the sum of repetitions accross all blocks.
int count_colisions(uint8_t * bytes , int bytes_size)
{
	int cols = 0;

	//Pointer 1 to every first element of each block of the plaintext
	for(int i = 0; i < (bytes_size - 16) ; i+=16){
		//Pointer 2 to every sequent first element of each block of the plaintext
		for(int j = i + 16; j < (bytes_size) ; j+=16){
			if( memcmp(bytes + i , bytes + j , 16) == 0 ){
				cols++;
			}
		}
	}

	return cols;
}

//Counts how many colisions of 16-byte blocks exist in each array of strings of the strings structure.
int * count_colisions_strings(char ** strings , int nstrings)
{
	int * cols_table = NULL;

	// Allocate table to store and fill it
	cols_table = (int *)malloc(nstrings * sizeof(int));
	for(int i = 0; i < nstrings ; i++){
		cols_table[i] = count_colisions( (uint8_t *)strings[i] , strlen(strings[i]) );
	}

	/*Print to check
	for(int i = 0; i < nstrings; i++){
		printf("Line %d: %d\n" , i, cols_table[i]);
	}*/

	return cols_table;
}
