CC=gcc
CFLAGS = -g -Wall -Wextra -Wdeprecated-declarations

INCLUDE = -I. -I../include -I../src
LINCLUDE = -L. 
LIBS = -lcrypto
SOURCES = challenge-07.c ../src/basics.c ../src/xor_ciphers.c ../src/aes.c 

all: 
	$(CC) $(CFLAGS) $(INCLUDE) -o test main.c $(SOURCES) $(LINCLUDE) $(LIBS)
	
clean:
	rm -f *.out *.o test
