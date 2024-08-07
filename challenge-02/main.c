#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/basics.h"

int challenge1(char * in);

int main()
{
	int ret = 0;
	char in0[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	ret = challenge1(in0);

	return ret;
}

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
