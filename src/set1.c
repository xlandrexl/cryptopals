#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/basics.h"
#include "../include/xor_ciphers.h"
#include "../include/aes.h"

int challenge1(char * in);
int challenge2(char * hex1 , char * hex2);
int challenge3(char * hex);
//challenge4 not done
int challenge5(char * in , char * key);
int challenge6(char * filename);
int challenge7(char * filename , char * key);
int challenge8(char * filename);

int challenge1(char * in)
{
	uint8_t * bytes;
	int size_bytes;
	char * b64;

	bytes = hex2bytes(in, &size_bytes);
	if(bytes == NULL)
		return -1;

	b64 = bytes2b64(bytes, size_bytes);

	printf("%s" , b64);

	free(bytes);
	free(b64);

	return 1;
}

int challenge2(char * hex1 , char * hex2)
{
	int bytes_size;
	uint8_t * bytes1;
	uint8_t * bytes2;
	char * hex_out;

	bytes1 = hex2bytes(hex1, &bytes_size);
	if(bytes1 == NULL)
		return -1;
	bytes2 = hex2bytes(hex2, &bytes_size);
	if(bytes2 == NULL)
	{
		free(bytes1);
		return -1;
	}

	bytes1 = fixed_xor(bytes1 , bytes2, bytes_size);

	hex_out = bytes2hex(bytes1, bytes_size);

	printf("%s" , hex_out);

	free(bytes1);
	free(bytes2);
	free(hex_out);

	return 1;
}

int challenge3(char * hex)
{
	int size_bytes;
	uint8_t * bytes = hex2bytes(hex, &size_bytes);
	char c;

	c = break_single_byte_xor(bytes , size_bytes);

	printf("%c" , c);

	free(bytes);

	return 1;
}

//challenge4 not done

int challenge5(char * in , char * key)
{
	int in_size = strlen(in);
	int key_size = strlen(key);
	uint8_t * out;
	char * out_hex;

	out = repeat_key_xor_cipher((uint8_t * )in, in_size, (uint8_t * )key, key_size);
	out_hex = bytes2hex(out, in_size);

	printf("%s" , out_hex);

	free(out);
	free(out_hex);

	return 1;
}

int challenge6(char * filename)
{
	//char filename[] = "../files/set1-chal6.txt";
	char * key;
	uint8_t * bytes;
	int bytes_size;
	uint8_t * decrypted_bytes;
	char * str_out;

	//Read file
	bytes = b64file2bytes(filename , &bytes_size);

	//Calculate key
	key = break_repeat_key_xor(bytes , bytes_size, -1);

	//Decrypt it
	decrypted_bytes = repeat_key_xor_cipher(bytes, bytes_size, (uint8_t *)key, strlen(key));

	//Convert to string
	str_out = bytes2string( decrypted_bytes , bytes_size);

	printf("%s" , str_out);

	free(str_out);
	free(decrypted_bytes);
	free(bytes);
	free(key);

	return 1;
}

int challenge7(char * filename , char * key)
{
	//uint8_t key[]={'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E'};
	//char filename[] = "../files/set1-chal7.txt";

	int aes_input_size;
	uint8_t * aes_input = NULL;
	uint8_t * dec_out = NULL;
	uint8_t local_key[16];
	char * str_out;

	if(strlen(key) != 16){
		printf("Key is not 16 bytes\n. Exiting...\n");
		return -1;
	}

	strncpy( (char *)local_key, key, 16);

	aes_input = b64file2bytes(filename , &aes_input_size);
	dec_out = (uint8_t*)malloc(aes_input_size * sizeof(uint8_t));

	ecb_decrypt( aes_input , dec_out, aes_input_size , local_key);

	str_out = bytes2string(dec_out , aes_input_size);

	printf("%s" , str_out);

	free(str_out);
	free(dec_out);
	free(aes_input);

	return 1;
}

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

//valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./test
/*int main()
{
	int ret = 0;
	char in0[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	ret = challenge1(in0);

	char in1[] = "1c0111001f010100061a024b53535009181c";
	char in2[] = "686974207468652062756c6c277320657965";
	ret = challenge2(in1,in2);

	char in3[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	ret = challenge3(in3);


	//challenge4 not done

	char in4[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	char key0[] = "ICE";
	ret = challenge5(in4,key0);

	char filename0[] = "../files/set1-chal6o.txt";
	ret = challenge6(filename0);

	char filename1[] = "../files/set1-chal7.txt";
	char key1[] = "YELLOW SUBMARINE";
	ret = challenge7(filename1 , key1);

	char filename2[] = "../files/set1-chal8.txt";
	ret = challenge8(filename2);

	return ret;
}*/
