CC=gcc
CFLAGS = -g -Wall -Wextra -Wno-deprecated-declarations

INCLUDE = -I. -I../include -I../src
LINCLUDE = -L. 
LIBS = -lcrypto
SOURCES = challenge-12.c ../src/basics.c ../src/xor_ciphers.c ../src/aes.c ../src/parsers.c ../src/aes_oracles.c ../src/aes_break.c

all: 
	$(CC) $(CFLAGS) $(INCLUDE) -o test main.c $(SOURCES) $(LINCLUDE) $(LIBS)
	
clean:
	rm -f *.out *.o test
