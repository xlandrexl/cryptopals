#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

#include "../include/set5.h"
#include "../include/basics.h"

int challenge34(void){
	
	// Declare variables
	mpz_t p, g, a, A, b, B;
	mpz_t s_a, s_b;
	mpz_inits(p, g, a, A, b, B, s_a, s_b, NULL);
       
  	// Initialize constants
	mpz_set_str(p,  "ffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024"
			"e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd"
			"3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec"
			"6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f"
			"24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361"
			"c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552"
			"bb9ed529077096966d670c354e4abc9804f1746c08ca237327fff"
			"fffffffffffff", 16);
		
	mpz_set_ui(g, 2);
	
	// Generate keys for A and B
   	generate_keys(p, g, a, A);
   	generate_keys(p, g, b, B);

	// Generate shared secrets
    	compute_shared_secret(s_a, B, a, p);
    	compute_shared_secret(s_b, A, b, p);
    	
    	// Hash shared secrets to generate Key
    	char s_a_sha1[21];
    	char s_a_buffer[1024];
	gmp_sprintf(s_a_buffer, "%Zd\n", s_a);
	_SHA1( s_a_sha1, s_a_buffer, strlen(s_a_buffer));
	s_a_sha1[16] = '\0';
	
	// Encrypt
	uint8_t iv[16];
	gen_key(iv);
	uint8_t * pt = NULL;
	bytes = strlen(
	 
	cbc_encrypt(pt , ct , int bytes , s_a_sha1 , iv)
	
	return 1;
}
