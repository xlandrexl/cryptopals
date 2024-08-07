#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set1.h"
#include "../include/basics.h"
#include "../include/aes.h"

int challenge8(char * filename)
{
	//char filename[FILENAMEBUFFER] = "../files/set1-chal8.txt";

	int lines;
	char ** strings = file2strings(filename , &lines);
	int * cols_table = count_colisions_strings(strings , lines);
	int max = -1;
	int max_idx = -1;

	//find max
	for(int i = 0; i < lines ; i++){
		if(cols_table[i] > max){
			max = cols_table[i];
			max_idx = i;
		}
	}

	printf("%s" , strings[max_idx]);

	//free what we can free
	for(int i = 0; i < lines ; i++){
		free(strings[i]);
	}
	free(strings);
	free(cols_table);

	return 1;
}
