#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"

int challenge1(char * in)
{
	uint8_t * bytes;
	int size_bytes;
	char * b64;

	bytes = hex2bytes(in, &size_bytes);
	if(bytes == NULL)
		return -1;

	b64 = bytes2b64(bytes, size_bytes);

	printf("%s" , b64);

	free(bytes);
	free(b64);

	return 1;
}
