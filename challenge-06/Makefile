CC=gcc
CFLAGS = -g -Wall -Wextra

INCLUDE = -I. -I../include -I../src
LINCLUDE = -L. 
LIBS = -lcrypto
SOURCES = challenge-06.c ../src/basics.c ../src/xor_ciphers.c ../src/xor_ciphers_break.c

all: 
	$(CC) $(CFLAGS) $(INCLUDE) -o test main.c $(SOURCES) $(LINCLUDE) $(LIBS)
	
clean:
	rm -f *.out *.o test
