#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"
#include "../include/xor_ciphers.h"

int challenge3(char * hex)
{
	int size_bytes;
	uint8_t * bytes = hex2bytes(hex, &size_bytes);
	char c;

	c = break_single_byte_xor(bytes , size_bytes);

	printf("%c" , c);

	free(bytes);

	return 1;
}
