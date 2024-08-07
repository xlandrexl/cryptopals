#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set3.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char filename[] = "../files/set3-chal19.txt";
	
	ret = challenge19(filename);

	return ret;
}
