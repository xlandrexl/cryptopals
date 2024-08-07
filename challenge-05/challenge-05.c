#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"
#include "../include/xor_ciphers.h"

int challenge5(char * in , char * key)
{
	int in_size = strlen(in);
	int key_size = strlen(key);
	uint8_t * out;
	char * out_hex;

	out = repeat_key_xor_cipher((uint8_t * )in, in_size, (uint8_t * )key, key_size);
	out_hex = bytes2hex(out, in_size);

	printf("%s" , out_hex);

	free(out);
	free(out_hex);

	return 1;
}
