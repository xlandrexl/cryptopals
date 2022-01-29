#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../include/basics.h"
#include "../include/xor_ciphers.h"

int break_repeat_key_xor(char * filename)
{
	int keysize;
	FILE * fp;
	int n;
	char * b64;

	//Open file
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error openning file\n"); 
		return -1;
	}

	//Read file to buffer
	fseek(fp, 0, SEEK_END);
  	n = ftell(fp);
 	fseek(fp, 0, SEEK_SET);
	b64 = (char*)malloc((n + 1) * sizeof(char));
	if(b64 == NULL){
		printf("Error allocating memory for base64 string.\n");
		return NULL;
	}	
	fread(b64, 1, n, fp);

	printf("%s\n",b64);

	return 1;
}


