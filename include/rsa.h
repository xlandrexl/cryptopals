#ifndef RSA_H
#define RSA_H

void generate_keys(mpz_t p, mpz_t g, mpz_t private_key, mpz_t public_key);
void compute_shared_secret(mpz_t shared_secret, mpz_t other_public_key, mpz_t private_key, mpz_t p);

#endif
