#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set3.h"
#include "../include/basics.h"
#include "../include/aes.h"

int challenge19(char * filename)
{
	//char filename[] = "../files/set3-chal19.txt";
	
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
