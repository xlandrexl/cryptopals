#include <gtest/gtest.h>
#include "src/basics.cpp"

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
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
