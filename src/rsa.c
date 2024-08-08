#include <gmp.h>
#include <time.h>

#include "../include/rsa.h"

void generate_keys(mpz_t p, mpz_t g, mpz_t private_key, mpz_t public_key) 
{
    // Seed the random number generator
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    // Generate a random private key (1 < private_key < p)
    mpz_urandomm(private_key, state, p);
    if (mpz_cmp_ui(private_key, 0) == 0) {
        mpz_add_ui(private_key, private_key, 1); // Ensure the private key is at least 1
    }

    // Compute public key: public_key = g^private_key mod p
    mpz_powm(public_key, g, private_key, p);

    // Clean up the random state
    gmp_randclear(state);
    
    return;
}

void compute_shared_secret(mpz_t shared_secret, mpz_t other_public_key, mpz_t private_key, mpz_t p) {
    // Compute shared secret: shared_secret = other_public_key^private_key mod p
    mpz_powm(shared_secret, other_public_key, private_key, p);
    
    return;
}
