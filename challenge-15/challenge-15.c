#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set2.h"
#include "../include/basics.h"
#include "../include/aes.h"

int challenge15(char * plaintext)
{
	//char plaintext[]="ICE ICE BABY\x04\x04\x04\x04";

	int bytes_size;
	uint8_t * bytes = string2bytes(plaintext , &bytes_size);
	int valid = 0;
	char * str_out;

	bytes = validate_pkcs(bytes , &bytes_size , &valid);

	if(valid == 0){
		printf("inv");
		free(bytes);
		return 1;
	}

	str_out = bytes2string(bytes , bytes_size);

	printf("%s" , str_out);

	free(str_out);
	free(bytes);

	return 1;
}
