#ifndef SHA1_H
#define SHA1_H

#include "stdint.h"

typedef struct
{
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
} SHA1_CTX;

// Private
void SHA1Transform(
    uint32_t state[5],
    const unsigned char buffer[64]
    );

void SHA1Init(
    SHA1_CTX * context
    );

void SHA1Update(
    SHA1_CTX * context,
    const unsigned char *data,
    uint32_t len
    );

void SHA1Final(
    unsigned char digest[20],
    SHA1_CTX * context
    );



// Public
void _SHA1(char *hash_out,    const char *str,    uint32_t len);
void SHA1(char * hexresult, char const string[]);
char * secret_prefix_mac_sha1(char * key, char * message);

#endif /* SHA1_H */

