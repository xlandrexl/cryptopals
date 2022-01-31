#ifndef AES_H
#define AES_H

#define KEY_SIZE 16
#define AES_BLOCK_SIZE 16

void ecb_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[16]);
void ecb_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[16]);

void cbc_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[16] , uint8_t iv[16]);
void cbc_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[16] , uint8_t iv[16]);

#endif
