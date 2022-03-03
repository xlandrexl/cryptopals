#ifndef BASICS_H
#define BASICS_H

#include <stdint.h>

#define FILENAMEBUFFER 248
#define MAXLINESIZE 1024

/* Basic Convertors */
uint8_t * hex2bytes(char * hex, int * size_bytes);
char * bytes2hex(uint8_t * bytes, int size_bytes);
char * bytes2b64(uint8_t * bytes, int size_bytes);
uint8_t * b642bytes(char * b64, int * size_bytes);

uint8_t * b64file2bytes(char * filename , int * bytes_size);
char ** file2strings(char * filename , int * lines);

char * bytes2string(uint8_t * bytes , int n);
uint8_t * string2bytes(char * str , int * bytes_size);

/* Basic Prints */
void print_dec(uint8_t * bytes , int n);
void print_hex(uint8_t * bytes , int n);
void print_char(uint8_t * bytes , int n);

/* Basic Operators */
int hamming_distance_byte(uint8_t x, uint8_t y);
int hamming_distance(uint8_t * bytes1 , uint8_t * bytes2 , int bytes_size);
//int hamming_distance_str(char * str1 , char * str2);

/* Basic Parsers */
#define JSON_CAPACITY 10
#define JSON_STR_SIZE 128

typedef struct Json {
	char names[JSON_CAPACITY][JSON_STR_SIZE];
	char attributes[JSON_CAPACITY][JSON_STR_SIZE];
	int n;
} Json;

char * profile_for(char * mail);
Json parse_json(char * input);
void print_json(Json json);

#endif
