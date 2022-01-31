#ifndef XOR_CIPHERS_H
#define XOR_CIPHERS_H

#include <stdint.h>

#define NCHARS 26+1 //26 English chars plus "other"
#define START_CHAR ' '
#define END_CHAR '~'

// % Frequencies of A-Z   A		   B        C        D        E         F	     G        ...
const float english_freq[NCHARS] = {8.4966 , 2.0720 , 4.5388 , 3.3844 , 11.1607 , 1.8121 , 2.4705 , 3.0034 , 7.5448 , 0.1965 , 1.1016 , 5.4893 , 
					      3.0129 , 6.6544 , 7.1635 , 3.1671 ,  0.1962 , 7.5809 , 5.7351 , 6.9509 , 3.6308 , 1.0074 , 1.2899 , 0.2902 , 
					      1.7779 , 0.2722 , 0}; //Last will be "other"

//\0 used as placeholder for other.
const char english_chars[NCHARS] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','\0'};

uint8_t * fixed_xor(uint8_t * bytes1 , uint8_t * bytes2, int bytes_size);

float correlationCoefficient(float * X , float * Y, int n);
void count_char_freq(char * buf , float char_freq[NCHARS]);
char single_byte_xor_cipher(uint8_t * bytes , int size_bytes);
char * repeat_key_xor_cipher(char * in , char * key);

char * break_repeat_key_xor(uint8_t * bytes , int size_bytes);
#endif
