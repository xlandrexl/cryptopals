#include <gtest/gtest.h>
#include <openssl/aes.h>

#include "src/basics.c"
#include "src/xor_ciphers.c"
#include "src/xor_ciphers_break.c"
#include "src/aes.c"
#include "src/aes_break.c"
#include "src/set1.c"
#include "src/set2.c"
#include "src/set3.c"

#define PRINT 0 //Prints output in tests where a solution is not provided

//SET 1
//Challenge 1
TEST(Set1_Challenge1, Basic) {
	char in[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	char exp_out[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";	
	int ret = 0;
	
	/* Capture the stdout so you dont mess with my memory */
	testing::internal::CaptureStdout();
	ret = challenge1(in);
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	ASSERT_TRUE( output == exp_out );
}

//Challenge 2
TEST(Set1_Challenge2, Basic) {
	char in1[] = "1c0111001f010100061a024b53535009181c";
	char in2[] = "686974207468652062756c6c277320657965";
	char exp_out[] = "746865206b696420646f6e277420706c6179";
	int ret = 0;

	testing::internal::CaptureStdout();
	ret = challenge2(in1,in2);
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	ASSERT_TRUE( output == exp_out );
} 

//Challenge 3
TEST(Set1_Challenge3, Basic) {
	char in[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	char exp_out = 'X';
	int ret = 0;

	testing::internal::CaptureStdout();
	ret = challenge3(in);
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	ASSERT_TRUE( output[0] == exp_out );
}

//Challenge 4 not done

//Clallenge 5
TEST(Set1_Challenge5, Basic) {
	char in[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	char key[] = "ICE";
	char exp_out[] = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
	int ret = 0;

	testing::internal::CaptureStdout();
	ret = challenge5(in,key);
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	ASSERT_TRUE( output == exp_out );
}

//Challenge 6 - How to properly test, without the solution?
TEST(Set1_Challenge6, Basic) {
	char filename[] = "../files/set1-chal6.txt";
	int ret = 0;

	testing::internal::CaptureStdout();
	ret = challenge6(filename);
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	#if PRINT
		std::cout << output;
	#endif
}

//Challenge 7 - How to properly test, without the solution?
TEST(Set1_Challenge7, Basic) {
	char filename[] = "../files/set1-chal7.txt";
	char key[] = "YELLOW SUBMARINE";
	int ret = 0;
	
	testing::internal::CaptureStdout();
	ret = challenge7(filename , key);
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	#if PRINT
		std::cout << output;
	#endif
} 

//Challenge 8 - How to properly test, without the solution?
TEST(Set1_Challenge8, Basic) {
	char filename[] = "../files/set1-chal8.txt";
	int ret = 0;
	
	testing::internal::CaptureStdout();
	ret = challenge8(filename );
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	#if PRINT
		std::cout << output;
	#endif
} 

//SET 2
/*Challenge 9
TEST(Set2_Challenge9, Basic) {
	char pt[]="YELLOW SUBMARINE";
	int pad = 20;
	char exp_out[] = "YELLOW SUBMARINE\x04\x04\x04\x04";
	int ret = 0;
	
	testing::internal::CaptureStdout();
	ret = challenge9(pt , pad);
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	ASSERT_TRUE( memcmp(output.c_str(), exp_out, pad) == 0 );
} */

//Challenge 10 - How to properly test, without the solution?
TEST(Set2_Challenge10, Basic) {
	char filename[] = "../files/set2-chal10.txt";
	char key[] = "YELLOW SUBMARINE";
	int ret = 0;
	
	testing::internal::CaptureStdout();
	ret = challenge10(filename , key);
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	#if PRINT
		std::cout << output;
	#endif
} 

//Challenge 11 not done
//Challenge 12 not done
//Challenge 13 not done
//Challenge 14 not done

//Challenge 15
TEST(Set2_Challenge15, Basic) {
	char pt1[]="ICE ICE BABY\x04\x04\x04\x04";
	char pt2[]="ICE ICE BABY\x05\x05\x05\x05";
	char pt3[]="ICE ICE BABY\x01\x02\x03\x04";
	char exp_out1[]="ICE ICE BABY";
	char exp_out2[]="inv";
	char exp_out3[]="inv";
	int ret = 0;
	std::string output;
	
	testing::internal::CaptureStdout();
	ret = challenge15(pt1);
	output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	ASSERT_TRUE( output == exp_out1 );

	testing::internal::CaptureStdout();
	ret = challenge15(pt2);
	output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	ASSERT_TRUE( output == exp_out2 );

	testing::internal::CaptureStdout();
	ret = challenge15(pt3);
	output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	ASSERT_TRUE( output == exp_out3 );
} 

//Challenge 16 not done

//SET 3

//Challenge 17 not done

//Challenge 18
TEST(Set2_Challenge18, Basic) {
	char ct[] = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==";
	char key[] = "YELLOW SUBMARINE";
	int nounce = 0;
	int ret = 0;

	testing::internal::CaptureStdout();
	ret = challenge18(ct , key, nounce);
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_TRUE( ret == 1 );
	#if PRINT
		std::cout << output;
	#endif
}

//Challenge 19 not done
//Challenge 20 not done
//Challenge 21 not done
//Challenge 22 not done
//Challenge 23 not done
//Challenge 24 not done

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

