#ifndef XOR_CIPHERS_H
#define XOR_CIPHERS_H

#include <stdint.h>

//Cipher functions
uint8_t * fixed_xor(uint8_t * bytes1 , uint8_t * bytes2, int bytes_size);
uint8_t * repeat_key_xor_cipher(uint8_t * bytes, int bytes_size, uint8_t * key, int key_size);

//Public break functions
char break_single_byte_xor(uint8_t * bytes , int size_bytes);
char * break_repeat_key_xor(uint8_t * bytes , int bytes_size , int fixed_keysize);

#endif
