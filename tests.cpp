#include <gtest/gtest.h>

#include "src/basics.cpp"
#include "src/xor_ciphers.cpp"

TEST(Set1_Challenge1, Basic) {
	char in[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	char out[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	ASSERT_STREQ(out, hex2b64(in) );
}

TEST(Set1_Challenge2, Basic) {
	char in1[] = "1c0111001f010100061a024b53535009181c";
	char in2[] = "686974207468652062756c6c277320657965";
	char out[] = "746865206b696420646f6e277420706c6179";
	ASSERT_STREQ(out, fixed_xor(in1,in2) );
} /*

TEST(Set1_Challenge5, Basic) {
	char in[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	char key[] = "ICE";
	char out[] = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
	ASSERT_STREQ(out, repeat_key_xor_cipher(in,key));
}*/

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
