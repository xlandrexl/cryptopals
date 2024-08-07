#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set2.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char pt[]="YELLOW SUBMARINE";
	int pad = 20;
	ret = challenge9(pt , pad);

	return ret;
}
