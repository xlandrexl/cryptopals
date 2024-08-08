#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "../include/set4.h"
#include "../include/basics.h"
#include "../include/aes.h"
#include "../include/xor_ciphers.h"
#include "../include/sha1.h"

int challenge28(void)
{
	char key[] = "YELLOW SUBMARINE";
	char const message[] = "I am gonna be authenticated!";
	char * hexresult;
	
	hexresult = secret_prefix_mac_sha1(key, message);
	printf("%s\n" , hexresult);
	
	free(hexresult);
	
	return 1;
}
