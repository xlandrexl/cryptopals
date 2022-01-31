#ifndef BASICS_H
#define BASICS_H

#include <stdint.h>

#define FILENAMEBUFFER 248

//Characters used in base64 encoding
const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//Characters used in base64 decoding -- VERIFY!!!
const int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
					   59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
						6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
					  	21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
						29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
						43, 44, 45, 46, 47, 48, 49, 50, 51 };


uint8_t * hex2bytes(char * hex, int * size_bytes);
char * bytes2hex(uint8_t * bytes, int size_bytes);
char * bytes2b64(uint8_t * bytes, int size_bytes);
uint8_t * b642bytes(char * b64, int * size_bytes);

uint8_t * b64file2bytes(char filename[] ,int * bytes_size);

char * bytes2string(uint8_t * bytes , int n);

void print_dec(uint8_t * bytes , int n);
void print_hex(uint8_t * bytes , int n);
void print_char(uint8_t * bytes , int n);

int hamming_distance_byte(uint8_t x, uint8_t y);
int hamming_distance_str(char * str1 , char * str2);

#endif
