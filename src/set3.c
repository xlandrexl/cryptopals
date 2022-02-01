#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/basics.h" 
#include "../include/xor_ciphers.h"
#include "../include/aes.h"

//challenge17 not done
char * challenge18(char * in , char * key , int nounce);
//challenge19 not done
//challenge20 not done
//challenge21 not done
//challenge22 not done
//challenge23 not done
//challenge24 not done


/*int main()
{
	char key[]="YELLOW SUBMARINE";
	int nounce = 0;
	char in[] = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==";

	char * output = challenge18(in, key, nounce);

	printf("%s\n" , output);
	
	return 0;
}*/

char * challenge18(char * in , char * key , int nounce)
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
		return NULL;
	}
	ctr_decrypt(ct , pt , pt_size , (uint8_t *)key , nounce);

	str_out = bytes2string(pt , pt_size); 
	
	free(pt);
	free(ct);

	return str_out;
} 
