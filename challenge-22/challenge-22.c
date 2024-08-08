#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "../include/set3.h"
#include "../include/basics.h"
#include "../include/mtwister.h"

int challenge22()
{
	int bef = (time(0));
	//printf("bef: %d\n" , bef);
	int d1 = 40 + rand() % (1000-40);
	int seed = bef + d1;
	init_genrand(seed);
	//int d2 = 40 + rand() % (1000-40);

	int out = (int)genrand_int32();

	//Ok... How many seconds have elapsed at max?
	int d = 2000;
	int now = (time(0));
	//printf("now: %d\n", now);
	int cmp;

	for(int i = -d ; i < d ; i++){
		init_genrand(now-i);
		cmp = (int)genrand_int32();
		if(cmp == out){
			printf("Seed found: %d\n" , now-i);
			return 1;
		}
	}

	printf("Seed not found\n");

	return 0;
}
