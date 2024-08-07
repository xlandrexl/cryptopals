#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set3.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char filename[] = "../files/set3-chal20.txt";
	
	ret = challenge20(filename);

	return ret;
}
