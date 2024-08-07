#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set2.h"
#include "../include/basics.h"
#include "../include/aes.h"

int challenge9(char * pt , int n)
{
	//char pt[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E','\0'};
	//int n = 20;

	char * str_out;
	int bytes_size;
	uint8_t * bytes;

	bytes = string2bytes(pt, &bytes_size);
	bytes = pkcs( bytes , bytes_size , n);
	bytes_size = n;

	str_out = bytes2string(bytes , bytes_size);

	printf("%s" , str_out);

	free(bytes);
	free(str_out);

	return 1;
}
