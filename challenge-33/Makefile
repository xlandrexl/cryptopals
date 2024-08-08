CC=gcc
CFLAGS = -g -Wall -Wextra -Wno-deprecated-declarations

INCLUDE = -I. -I../include -I../src
LINCLUDE = -L. 
LIBS = -lcrypto -lgmp
SOURCES = challenge-33.c ../src/rsa.c 

all: 
	$(CC) $(CFLAGS) $(INCLUDE) -o test main.c $(SOURCES) $(LINCLUDE) $(LIBS)
	
clean:
	rm -f *.out *.o test
