#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set2.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char filename[] = "../files/set2-chal10.txt";
	char key[] = "YELLOW SUBMARINE";
	ret = challenge10(filename , key);

	return ret;
}
