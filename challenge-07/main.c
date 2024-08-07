#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char filename1[] = "../files/set1-chal7.txt";
	char key1[] = "YELLOW SUBMARINE";
	ret = challenge7(filename1 , key1);

	return ret;
}
