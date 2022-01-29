#ifndef XOR_CIPHERS_H
#define XOR_CIPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


//Char freq is not being a perfect metric, but lets go on for now!
// % Frequencies of A-Z   A		   B        C        D        E         F	     G        ...
float english_freq[26] = {8.4966 , 2.0720 , 4.5388 , 3.3844 , 11.1607 , 1.8121 , 2.4705 , 3.0034 , 7.5448 , 0.1965 , 1.1016 , 5.4893 , 
					      3.0129 , 6.6544 , 7.1635 , 3.1671 ,  0.1962 , 7.5809 , 5.7351 , 6.9509 , 3.6308 , 1.0074 , 1.2899 , 0.2902 , 
					      1.7779 , 0.2722}; 

char english_chars[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

char * fixed_xor(char * hex1 , char * hex2);
float correlationCoefficient(float * X , float * Y, int n);
void count_char_freq(char * buf , float char_freq[26]);
char single_byte_xor_cipher(char * hex);
char * repeat_key_xor_cipher(char * in , char * key);

#endif
