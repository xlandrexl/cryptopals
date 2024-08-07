#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set3.h"
#include "../include/basics.h"
#include "../include/aes.h"

int challenge18(char * in , char * key , int nounce)
{
	//char key[]="YELLOW SUBMARINE";
	//int nounce = 0;
	//char in[] = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ=="

	int pt_size;
	uint8_t * pt;
	uint8_t * ct;
	char * str_out;

	ct = b642bytes(in, &pt_size);
	pt = (uint8_t *)malloc(pt_size * sizeof(uint8_t));

	if(strlen(key) != 16){
		printf("Key is not 16 bytes\n. Exiting...\n");
		return -1;
	}
	ctr_decrypt(ct , pt , pt_size , (uint8_t *)key , nounce);

	str_out = bytes2string(pt , pt_size);

	printf("%s" , str_out);

	free(str_out);
	free(pt);
	free(ct);

	return 1;
}
