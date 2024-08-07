#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"
#include "../include/xor_ciphers.h"

int challenge6(char * filename)
{
	//char filename[] = "../files/set1-chal6.txt";
	char * key;
	uint8_t * bytes;
	int bytes_size;
	uint8_t * decrypted_bytes;
	char * str_out;

	//Read file
	bytes = b64file2bytes(filename , &bytes_size);

	//Calculate key
	key = break_repeat_key_xor(bytes , bytes_size, -1);

	//Decrypt it
	decrypted_bytes = repeat_key_xor_cipher(bytes, bytes_size, (uint8_t *)key, strlen(key));

	//Convert to string
	str_out = bytes2string( decrypted_bytes , bytes_size);

	printf("%s" , str_out);

	free(str_out);
	free(decrypted_bytes);
	free(bytes);
	free(key);

	return 1;
}
