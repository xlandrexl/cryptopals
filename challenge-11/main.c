#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set2.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char filename[] = "../files/set2-chal9.txt";
	int tries = 20;
	ret = challenge11(filename , tries);

	return ret;
}
