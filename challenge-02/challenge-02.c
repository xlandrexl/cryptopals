#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"

int challenge2(char * hex1 , char * hex2)
{
	int bytes_size;
	uint8_t * bytes1;
	uint8_t * bytes2;
	char * hex_out;

	bytes1 = hex2bytes(hex1, &bytes_size);
	if(bytes1 == NULL)
		return -1;
	bytes2 = hex2bytes(hex2, &bytes_size);
	if(bytes2 == NULL)
	{
		free(bytes1);
		return -1;
	}

	bytes1 = fixed_xor(bytes1 , bytes2, bytes_size);

	hex_out = bytes2hex(bytes1, bytes_size);

	printf("%s" , hex_out);

	free(bytes1);
	free(bytes2);
	free(hex_out);

	return 1;
}
