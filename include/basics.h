#ifndef BASICS_H
#define BASICS_H

#include <stdint.h>

#define FILENAMEBUFFER 248

uint8_t * hex2bytes(char * hex, int * size_bytes);
char * bytes2hex(uint8_t * bytes, int size_bytes);
char * bytes2b64(uint8_t * bytes, int size_bytes);
uint8_t * b642bytes(char * b64, int * size_bytes);

uint8_t * b64file2bytes(char * filename , int * bytes_size);
char ** file2strings(char * filename , int * lines);

char * bytes2string(uint8_t * bytes , int n);
uint8_t * string2bytes(char * str , int * bytes_size);

void print_dec(uint8_t * bytes , int n);
void print_hex(uint8_t * bytes , int n);
void print_char(uint8_t * bytes , int n);

int hamming_distance_byte(uint8_t x, uint8_t y);
int hamming_distance(uint8_t * bytes1 , uint8_t * bytes2 , int bytes_size);
//int hamming_distance_str(char * str1 , char * str2);

#endif
