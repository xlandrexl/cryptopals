#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char in3[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	ret = challenge3(in3);

	return ret;
}
