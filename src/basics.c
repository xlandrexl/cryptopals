#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/basics.h" 

//Characters used in base64 encoding
static const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
//Characters used in base64 decoding -- VERIFY!!!
static const int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
					   59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
						6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
					  	21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
						29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
						43, 44, 45, 46, 47, 48, 49, 50, 51 };

//Transforms an hexadecimal string into an array of bytes. 
//Returns allocated bytes array and its size in size_bytes.
//Returns NULL on error.
uint8_t * hex2bytes(char * hex, int * size_bytes)
{
	uint8_t * bytes = NULL;
	char * p = hex;
	int size_hex = strlen(hex);	

	/*Get size of bytes array*/
	if(size_hex % 2 == 0){ /*Even*/
		(*size_bytes) = size_hex/2;
	}else{ /*Odd*/
		(*size_bytes) = (size_hex+1)/2; 
	}
	
	bytes = (uint8_t*)malloc((*size_bytes) * sizeof(uint8_t));
	if(bytes == NULL){
		printf("Error allocating memory for bytes array.\n");
		return NULL;
	}	
	
	for(int j = 0; j < (*size_bytes); j++){
		sscanf(p, "%2hhx", &(bytes[j])); /*Warning gave me the hh. Check it out*/
		p += 2;
	}
	
	return bytes;
}

//Transforms an array of bytes into an hexadecimal string. 
//Returns allocated hexadecimal string.
//Returns NULL on error.
char * bytes2hex(uint8_t * bytes, int size_bytes)
{
	char * hex = NULL;
	int size_hex = 2 * size_bytes;
	char * p;

	hex = (char*)malloc((size_hex + 1) * sizeof(char));// +1 to add \0
	if(hex == NULL){
		printf("Error allocating memory for hexadecimal string.\n");
		return NULL;
	}	
	hex[size_hex] = '\0';
	p = hex;

	for(int i = 0; i < size_bytes ; i++){
		sprintf(p, "%02x", bytes[i]);
		p += 2;
	}
	
	return hex;
}

//Transforms an array of bytes into a padded base64 encoded string. 
//Returns allocated base64 string.
//Returns NULL on error.
char * bytes2b64(uint8_t * bytes, int size_bytes)
{
	char * b64 = NULL;
	long int val; //Assure it holds 3+ bytes!
	int size_b64;
	
	//Get size of bytes array
	size_b64 = size_bytes; 
	if( size_b64 % 3 != 0){ //Make size_b64 divisible by 3
		size_b64 += 3 - (size_b64 % 3); 
	}
	size_b64 = size_b64 * 4 / 3; //Multyply by 4/3 because b64 encodes 3 bytes in 4 chars.
	
	b64 = (char*)malloc((size_b64 + 1) * sizeof(char));// +1 to add \0
	if(b64 == NULL){
		printf("Error allocating memory for base64 string.\n");
		return NULL;
	}	
	b64[size_b64] = '\0';
	
	for(int i=0,j=0; i < size_bytes; i+=3 , j+=4){
		/*Push (up to) 3 bytes into val (assignation and left-shift) */
		val = bytes[i];
		val = (i + 1 < size_bytes) ? val << 8 | bytes[i+1] : val << 8;
		val = (i + 2 < size_bytes) ? val << 8 | bytes[i+2] : val << 8;

		/*In b64, one char is represented by 6 bits. Right shift followed by & with 0b111111 to separate every 6-bit part */
		b64[j] = b64chars[(val >> 18) & 0x3F]; 
		b64[j+1] = b64chars[(val >> 12) & 0x3F];
		/*Assure if necessary. Otherwise, pad with =*/
		b64[j+2] = (i + 1 < size_bytes) ? b64chars[(val >> 6) & 0x3F] : '='; 
		b64[j+3] = (i + 2 < size_bytes) ? b64chars[val & 0x3F] : '=';
	}

	return b64;
}

//Transforms base64 encoded string into an array of bytes. 
//Returns allocated array.
//Returns NULL on error.
uint8_t * b642bytes(char * b64, int * size_bytes)
{
	uint8_t * bytes = NULL;
	long int val; //Assure it holds 3+ bytes!
	int b64len = strlen(b64);
	int valid;

	//Verify length
	if(b64len % 4 != 0){
		printf("Invalid input length for base64 string.\n");
		return NULL;
	}
	
	//Verify characters
	for(int i = 0 ; i < b64len ; i++){
		valid = 0;
		for(int j = 0 ; j < (int)strlen(b64chars) ; j++){
			if(b64[i] == b64chars[j]){
				valid = 1;
				break;
			}
		}
		if(valid == 0){
			printf("Invalid input character for base64 string.\n");
			return NULL;
		}
	}

	//Get size of bytes array
	(*size_bytes) = b64len;
	(*size_bytes) = (*size_bytes) / 4 * 3;
	for(int i = b64len - 1 ; i > 0 ; i--){
		if(b64[i] == '='){
			(*size_bytes) -= 1;
		}else{	
			break;
		}
	}

	bytes = (uint8_t *)malloc( (*size_bytes) * sizeof(uint8_t));
	if(bytes == NULL){
		printf("Error allocating memory for bytes array.\n");
		return NULL;
	}	
	
	for(int i=0,j=0; i < b64len ; i+=4 , j+=3){
		/*  */
		val = b64invs[b64[i]-43];
		val = (val << 6) | b64invs[b64[i+1]-43];
		val = b64[i+2]=='=' ? val << 6 : (val << 6) | b64invs[b64[i+2]-43];
		val = b64[i+3]=='=' ? val << 6 : (val << 6) | b64invs[b64[i+3]-43];

		/* */
		bytes[j] = (val >> 16) & 0xFF;
		if (b64[i+2] != '=')
			bytes[j+1] = (val >> 8) & 0xFF;
		if (b64[i+3] != '=')
			bytes[j+2] = val & 0xFF;
	}

	return bytes;
}

uint8_t * b64file2bytes(char * filename ,int * bytes_size)
{
	FILE * fp;
	int filesize;
	char * b64;
	uint8_t * bytes;
	int count;
	char ch;

	//Open file
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error openning file\n"); 
		return NULL;
	}

	//Get size of file and allocate buffer
	fseek(fp, 0, SEEK_END);
  	filesize = ftell(fp);
 	fseek(fp, 0, SEEK_SET);
	b64 = (char*)malloc((filesize) * sizeof(char));
	if(b64 == NULL){
		printf("Error allocating memory for base64 string.\n");
		return NULL;
	}	
	
	//Read char by char to avoid \n
	count = 0;
	while(!feof(fp)){
  		ch = fgetc(fp);
		if(ch == '\n'){ //Can add other characters to avoid!
    		continue;
  		}
		b64[count] = ch;
		count++;
	}
	b64[count-1] = '\0'; //Put \0 in the end. I always read one more byte than I want...

	printf("%s" , b64);
	printf("File size: %d \nRead bytes: %d \nStrlen: %d\n" , filesize , count, (int)strlen(b64));

	//Convert b64 to bytes
	bytes = b642bytes(b64, &(*bytes_size));
	free(b64);

	fclose(fp);

	return bytes;
}

//Separates contents in a file by the \n. Returns array of strings.
char ** file2strings(char * filename , int * lines)
{
	FILE * fp;
	char ** strings = NULL;
	char * buffer = NULL;
	int linesize;
	int len;
	int line;
	char ch;

	//Open file
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error openning file\n"); 
		return NULL;
	}

	//Count lines in file
	*lines = 0;
	while(!feof(fp)){
  		ch = fgetc(fp);
		if(ch == '\n'){
    		*lines += 1;
  		}
	}
	fseek(fp, 0, SEEK_SET);

	//Malloc lines
	strings = (char**)malloc(*lines * sizeof(char*));
	
	line = 0;
	while ((linesize = getline(&buffer, (size_t*)&len, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        //printf("%s", line);
	
		strings[line] = (char*)malloc(linesize * sizeof(char)); //Linesize includes \n but not \0... We will just substitute it later
		memcpy(strings[line], buffer, linesize);
		strings[line][linesize-1] = '\0';
		line++;
    }	

	//Free buffer
	free(buffer);	
	fclose(fp);

	/*Print to check
	for(int i = 0; i < *lines; i++){
		printf("Line %d: %s\n" , i, strings[i]);
	}*/
	
	return strings;
}

char * bytes2string(uint8_t * bytes , int bytes_size)
{
	char * str = NULL;

	str = (char*)malloc((bytes_size+1) * sizeof(char));
	str[bytes_size] = '\0';
	memcpy(str, bytes, bytes_size);
	
	return str;
}

uint8_t * string2bytes(char * str , int * bytes_size)
{
	uint8_t * bytes = NULL;

	(*bytes_size) = strlen(str);

	bytes = (uint8_t *)malloc( (*bytes_size) * sizeof(uint8_t));
	memcpy( bytes, str, (*bytes_size) );
	
	return bytes;
}

//Prints decimal value of array of n bytes 
void print_dec(uint8_t * bytes , int n)
{
	int i;

	for(i = 0; i < n; i++){
		printf("%d" , bytes[i]);
	}
	printf("\n");
	
	return;
}

//Prints hexadecimal value of array of n bytes 
void print_hex(uint8_t * bytes , int n)
{
	int i;

	for(i = 0; i < n; i++){
		//printf("%02x " , bytes[i]);
		printf("%02x" , bytes[i]);
	}
	//printf("\n");
	
	return;
}


//Prints array of n bytes as string
void print_char(uint8_t * bytes , int n)
{
	int i;

	for(i = 0; i < n; i++){
		printf("%c" , bytes[i]);
	}
	//printf("\n");
	
	return;
}

//Hamming distance between two bytes
int hamming_distance_byte(uint8_t x, uint8_t y)
{
    int dist = 0;
	uint8_t val;

    // The ^ operators sets to 1 only the bits that are different
    for (val = x ^ y; val > 0; ++dist)
    {
        // We then count the bit set to 1 using the Peter Wegner way
        val = val & (val - 1); // Set to zero val's lowest-order 1
    }

    return dist;
}

//Hamming distance between two strings
/*int hamming_distance_str(char * str1 , char * str2)
{
	int i = 0, dist = 0;

    while(str1[i]!='\0' && str2[i]!='\0')
    {
        dist += hamming_distance_byte(str1[i], str2[i]);
        i++;
    }
    return dist;
}*/

//Hamming distance between two byte arrays
int hamming_distance(uint8_t * bytes1 , uint8_t * bytes2 , int bytes_size)
{
	int dist = 0;

	for (int i = 0 ; i < bytes_size ; i++){
        dist += hamming_distance_byte(bytes1[i], bytes2[i]);
    }
    return dist;
}
