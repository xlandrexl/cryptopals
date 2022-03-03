#ifndef AES_H
#define AES_H

#define KEY_SIZE 16
#define AES_BLOCK_SIZE 16

/* Encryption Functions */
void ecb_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[KEY_SIZE]);
void ecb_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[KEY_SIZE]);

void cbc_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[KEY_SIZE] , uint8_t iv[AES_BLOCK_SIZE]);
void cbc_decrypt(uint8_t * ct , uint8_t * pt , int bytes , uint8_t key[KEY_SIZE] , uint8_t iv[AES_BLOCK_SIZE]);

void ctr_encrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[KEY_SIZE] , uint64_t nonce);
void ctr_decrypt(uint8_t * pt , uint8_t * ct , int bytes , uint8_t key[KEY_SIZE] , uint64_t nonce);

uint8_t * ecb_encrypt_pad(uint8_t * pt , int pt_size , int * ct_size , uint8_t key[KEY_SIZE]);
uint8_t * ecb_decrypt_pad(uint8_t * ct , int ct_size , int * pt_size , uint8_t key[KEY_SIZE]);

uint8_t * cbc_encrypt_pad(uint8_t * pt , int pt_size , int * ct_size , uint8_t key[KEY_SIZE], uint8_t iv[KEY_SIZE]);
uint8_t * cbc_decrypt_pad(uint8_t * ct , int ct_size , int * pt_size , uint8_t key[KEY_SIZE], uint8_t iv[KEY_SIZE]);


/* PAD Functions */
uint8_t * pkcs(uint8_t * pt , int pt_size , int ct_size);
uint8_t * pkcs_bef(uint8_t * pt , int pt_size , int ct_size);
uint8_t * validate_pkcs(uint8_t * pt , int * pt_size , int * valid);
uint8_t * pkcs_auto(uint8_t * pt , int pt_size , int * padded_pt_size);

/* Break AES functions */
void gen_key(uint8_t key[KEY_SIZE]);
int count_colisions(uint8_t * bytes , int bytes_size);
int * count_colisions_strings(char ** strings , int nstrings);

/* AES Oracles */
uint8_t * ecb_cbc_encrypt_oracle(uint8_t * pt , int in_bytes , int * out_bytes , int * mode);
uint8_t * ecb_same_key_encrypt_oracle(uint8_t * pt , int in_bytes , int  * out_bytes);
uint8_t * ecb_encrypt_encode(char * mail , int * ct_size);
Json ecb_decrypt_parse(uint8_t * ct , int ct_size);

uint8_t * cbc_flip_encrypt_oracle(char * string , int  * out_bytes , uint8_t iv[AES_BLOCK_SIZE]);
int cbc_flip_check_oracle(uint8_t * ct , int  ct_size , uint8_t iv[AES_BLOCK_SIZE] );

uint8_t * cbc_pad_oracle_encrypt(int * ct_size , uint8_t iv[AES_BLOCK_SIZE]);
int cbc_pad_oracle_check(uint8_t * ct , int ct_size , uint8_t iv[AES_BLOCK_SIZE]);

void same_key_ctr_encrypt(uint8_t * pt , uint8_t * ct , int bytes);
uint8_t ** same_key_ctr_encrypt_b64_strings(char * filename , int * ncts , int ** cts_size);

uint8_t * cbc_key_is_iv_encrypt_oracle(uint8_t * pt , int pt_size , int * ct_size);
uint8_t * cbc_key_is_iv_decrypt_oracle(uint8_t * ct , int ct_size , int * pt_size , int * error);

#endif
