#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set3.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char in[] = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==";
	char key[]="YELLOW SUBMARINE";
	int nounce = 0;
	
	ret = challenge18(in, key, nounce);

	return ret;
}
