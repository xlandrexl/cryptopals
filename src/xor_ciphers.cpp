#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../include/basics.h"
#include "../include/xor_ciphers.h"
//#include "break_repeat_key_xor.cpp"

//Produces bitwise XOR between two same-sized hexadecimal strings. 
//Returns allocated hexadecimal string with the result.
//Returns NULL on error.
char * fixed_xor(char * hex1 , char * hex2)
{
	int size_bytes;	
	uint8_t * bytes1;
	uint8_t * bytes2;
	char * hex_out;

	bytes1 = hex2bytes(hex1, &size_bytes);
	if(bytes1 == NULL)
		return NULL;
	bytes2 = hex2bytes(hex2, &size_bytes);
	if(bytes2 == NULL)
	{
		free(bytes1);
		return NULL;
	}

	for(int i = 0; i < size_bytes; i++){
		bytes1[i] = bytes1[i] ^ bytes2[i];
	}

	hex_out = bytes2hex(bytes1, size_bytes);

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

void count_char_freq(char * buf , float char_freq[26])
{
	char c;

	for(int i = 0; i < 26; i++){
		char_freq[i] = 0;
	}

	for(int i = 0; i < (int)strlen(buf); i++)
	{
		c = toupper(buf[i]);
		for(int j = 0; j < 26; j++){
			if (c == english_chars[j]){
				char_freq[j] += 1;
				break; //No point in going on!
			}	
		}
	}

	for(int i = 0; i < 26; i++){
		char_freq[i] /= strlen(buf);
		char_freq[i] *= 100;
	}

	return;
}  

char single_byte_xor_cipher(char * hex)
{
	int size_bytes;	
	uint8_t * bytes;
	float char_freq[26] = {0};
	float corr_table[122-32];
	char c;
	
	bytes = hex2bytes(hex , &size_bytes);
	char * buf = (char*)malloc((size_bytes+1) * sizeof(char));	
	buf[size_bytes] = '\0';

	int j = 0;
	for(c = 32 ; c <= 122 ; ++c , j++)
	//for(char c = 88 ; c <= 89 ; ++c)
	{
		printf("%c : idx %d : ", c ,j);
		for(int i = 0; i < size_bytes; i++)
		{
			buf[i] = bytes[i] ^ c;
			printf("%c", buf[i]);
		}
		
		//Count chars of buff
		count_char_freq(buf,char_freq);
		corr_table[j] = correlationCoefficient(char_freq , english_freq , 26);

		printf("corr: %.3f\n" , corr_table[j]);	
	}

	//Find highest value in corr_table
	float max = -1;
	int idx_max = -1;
	for(int i = 0; i < j; i++)
	{
		if(corr_table[i] > max)
		{
			idx_max = i;
			max = corr_table[i];
		}
	}
	
	printf("Max idx %d with char %c and corr %f\n" , idx_max , (32+idx_max) , corr_table[idx_max]);
	
	free(buf);

	return 32 + idx_max;
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

	//Finish!
	hex = bytes2hex(buf, strlen(in));
	//print_hex(buf,strlen(in));
	return hex;
}

/*
int main()
{
	//char hex[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	//char a = single_byte_xor_cipher(hex);

	//char hex[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	//char key[] = "ICE";

	//char * ciphertext = repeat_key_xor_cipher(hex,key);
	//printf("%s\n" , ciphertext);

	//char str1[] = "this is a test";
	//char str2[] = "wokka wokka!!!";

	//int hd = hamming_distance_str(str1, str2);

	//printf("Hamming Distance: %d\n", hd);

	//int a = break_repeat_key_xor("../files/set1-chal6.txt");

	/*int size_bytes;
	char hex[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	uint8_t * bytes = hex2bytes(hex , &size_bytes);
	
	printf("Input bytes: ");
	print_hex(bytes, size_bytes);
	printf("Size bytes: %d\n" , size_bytes);

	char * b64 = bytes2b64(bytes, size_bytes);
	printf("\nB64 enc: %s\n" , b64);

	int bytes_dec_size;
	uint8_t * bytes_dec = b642bytes(b64, &bytes_dec_size);
	printf("Dec bytes:   ");
	print_hex(bytes_dec, bytes_dec_size);

	return 1;
}
*/

