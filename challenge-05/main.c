#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char in4[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	char key0[] = "ICE";
	ret = challenge5(in4,key0);

	return ret;
}
