#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set3.h"
#include "../include/basics.h"
#include "../include/mtwister.h"

int challenge21(int seed)
{
	init_genrand(seed);
	printf("%d\n" , (int)genrand_int32());
	return 1;
}
