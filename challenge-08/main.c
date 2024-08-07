#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char filename2[] = "../files/set1-chal8.txt";
	ret = challenge8(filename2);

	return ret;
}
