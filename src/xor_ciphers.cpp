#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../include/basics.h"
#include "../include/xor_ciphers.h"

//Produces bitwise XOR between two same-sized hexadecimal strings. 
//Returns allocated hexadecimal string with the result.
//Returns NULL on error.
uint8_t * fixed_xor(uint8_t * bytes1 , uint8_t * bytes2, int bytes_size)
{
	for(int i = 0; i < bytes_size; i++){
		bytes1[i] = bytes1[i] ^ bytes2[i];
	}

	return bytes1;
}

//Produces bitwise XOR between two same-sized hexadecimal strings. 
//Returns allocated hexadecimal string with the result.
//Returns NULL on error.
char * fixed_xor_hex(char * hex1 , char * hex2)
{
	int bytes_size;	
	uint8_t * bytes1;
	uint8_t * bytes2;
	char * hex_out;

	bytes1 = hex2bytes(hex1, &bytes_size);
	if(bytes1 == NULL)
		return NULL;
	bytes2 = hex2bytes(hex2, &bytes_size);
	if(bytes2 == NULL)
	{
		free(bytes1);
		return NULL;
	}

	bytes1 = fixed_xor(bytes1 , bytes2, bytes_size);

	hex_out = bytes2hex(bytes1, bytes_size);

	free(bytes1);
	free(bytes2);
	
	return hex_out;
}


//Returns correlation coefficient of two n-sized float arrays. 
float correlationCoefficient(float * X , float * Y, int n)
{
	int sum_X = 0, sum_Y = 0, sum_XY = 0;
    int squareSum_X = 0, squareSum_Y = 0;
  
    for (int i = 0; i < n; i++)
    {
        // sum of elements of array X.
        sum_X = sum_X + X[i];
  
        // sum of elements of array Y.
        sum_Y = sum_Y + Y[i];
  
        // sum of X[i] * Y[i].
        sum_XY = sum_XY + X[i] * Y[i];
  
        // sum of square of array elements.
        squareSum_X = squareSum_X + X[i] * X[i];
        squareSum_Y = squareSum_Y + Y[i] * Y[i];
    }
  
    // use formula for calculating correlation coefficient.
    float corr = (float)(n * sum_XY - sum_X * sum_Y) 
                  / sqrt((n * squareSum_X - sum_X * sum_X) 
                      * (n * squareSum_Y - sum_Y * sum_Y));
  
    return corr;
}

void count_char_freq(char * buf , float char_freq[NCHARS])
{
	char c;
	int m;

	for(int i = 0; i < NCHARS; i++){
		char_freq[i] = 0;
	}

	for(int i = 0; i < (int)strlen(buf); i++)
	{
		//Lets skip spaces so we penalty non leter characters!
		if(c == ' ')
			continue;

		c = toupper(buf[i]);
		m = 0;
		for(int j = 0; j < NCHARS; j++){
			if (c == english_chars[j]){
				char_freq[j] += 1;
				m = 1;
			}
		}
		if(m == 0){
			//If it wasnt any of those chars... Add to "other"
			char_freq[NCHARS-1] += 1;	
		}
	}
	for(int i = 0; i < NCHARS; i++){
		char_freq[i] /= strlen(buf);
		char_freq[i] *= 100;
	}

	return;
}  

char single_byte_xor_cipher(uint8_t * bytes , int size_bytes)
{
	float char_freq[NCHARS] = {0};
	float corr_table[END_CHAR-START_CHAR+1];
	char c;
	char * buf = NULL;
	float max;
	int idx_max;

	buf = (char*)malloc((size_bytes+1) * sizeof(char));	
	buf[size_bytes] = '\0';

	int j = 0;
	for(c = START_CHAR ; c <= END_CHAR ; c++,j++)
	{
		//printf("%c : idx %d : ", c ,j);
		for(int i = 0; i < size_bytes; i++)
		{
			buf[i] = bytes[i] ^ c;
			//printf("%c", buf[i]);
		}
		
		//Count chars of buff
		count_char_freq(buf,char_freq);

		for(int z = 0 ; z < NCHARS ; z++)
			//printf(" %.0f" , char_freq[z]);
	
		corr_table[j] = correlationCoefficient(char_freq , (float *)english_freq , NCHARS);

		//printf("\t: corr %.3f\n" , corr_table[j]);	
	}

	//Find highest value in corr_table
	max = -1;
	idx_max = -1;
	for(int i = 0; i < END_CHAR-START_CHAR+1 ; i++)
	{
		if(corr_table[i] > max)
		{
			idx_max = i;
			max = corr_table[i];
		}
	}
	
	//printf("Max idx %d with char %c and corr %f\n" , idx_max , (START_CHAR + idx_max) , corr_table[idx_max]);
	
	free(buf);

	return START_CHAR + idx_max;
}

char single_byte_xor_cipher_hex(char * hex)
{
	int size_bytes;
	uint8_t * bytes = hex2bytes(hex, &size_bytes);
	char c;

	c = single_byte_xor_cipher(bytes , size_bytes);

	free(bytes);	

	return c;
}

char * repeat_key_xor_cipher(char * in , char * key)
{
	uint8_t * buf = (uint8_t*)malloc( strlen(in) * sizeof(uint8_t));
	int key_size = strlen(key);
	char * hex = NULL;
	
	for(int i = 0; i < (int)strlen(in); i++)
	{
		buf[i] = in[i] ^ key[i % key_size];
	}

	hex = bytes2hex(buf, strlen(in));
	
	return hex;
}


/*int main()
{	
	//char * key = break_repeat_key_xor_file("../files/set1-chal6.txt");

	char hex[] = "Now transpose the blocks: make a block that is the first byte of every block, and a block that is the second byte of every block, and so on.";
	char mykey[] = "Hello";
	char * out; //060a1b4c1b3a04021f1f2716094c1b20004c0e032706071f5568080d070a68044c0e032706074c1b2004184c063b4518040a6803051e1c3c450e151b2d45030a4f2d13091e16680700030c23494c0d012c450d4c0d240a0f074f3c0d0d184f21164c18072d451f090c270b084c0d3111094c002e45091a0a3a1c4c0e03270607404f290b084c1c2745030241

	out = repeat_key_xor_cipher(hex,mykey);
	b64out = hex2b64(out);
	//printf("b64: %s\n" , b64out);

	
	//Now lets see why my code doesnt work...
	int bytes_size;
	char * key;
	uint8_t * bytes = hex2bytes(out, &bytes_size);
	//print_hex(bytes , bytes_size);

	key = break_repeat_key_xor(bytes , bytes_size);
	printf("key: %s\n" , key);

	return 0;
}*/


