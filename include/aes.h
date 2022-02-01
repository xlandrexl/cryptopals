#ifndef AES_H
#define AES_H

#define KEY_SIZE 16
#define AES_BLOCK_SIZE 16

void ecb_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[16]);
void ecb_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[16]);

void cbc_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[16] , uint8_t iv[16]);
void cbc_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[16] , uint8_t iv[16]);

/* Break AES functions */
void gen_key(uint8_t key[KEY_SIZE]);
uint8_t * pkcs(uint8_t * pt , int pt_size , int ct_size);
uint8_t * pkcs_bef(uint8_t * pt , int pt_size , int ct_size);
uint8_t * validate_pkcs(uint8_t * pt , int pt_size , int ct_size);

uint8_t * ecb_cbc_encrypt(uint8_t * pt , int in_bytes , int * out_bytes);

#endif
