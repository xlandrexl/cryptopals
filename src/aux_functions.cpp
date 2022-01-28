#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void print_dec(uint8_t * bytes , int size)
{
	int i;

	for(i = 0; i < size; i++){
		printf("%d" , bytes[i]);
	}
	printf("\n");
	
	return;
}


void print_hex(uint8_t * bytes , int size)
{
	int i;

	for(i = 0; i < size; i++){
		printf("%02x " , bytes[i]);
	}
	printf("\n");
	
	return;
}

