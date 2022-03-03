#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/sha1.h"

char * authenticate_message(char * message){
  char key[] = "YELLOW SUBMARINE";
  SHA1Context sha;
  char * output;
  char * buffer;

  output = (char *)malloc( 41*sizeof(char));
  output[40] = '\0';
  buffer = (char *)malloc( (strlen(message) + strlen(key) + 1)*sizeof(char));

  strcpy(buffer,key);
  strcpy(buffer + strlen(key) , message);
  buffer[(strlen(message) + strlen(key))] = '\0';

	SHA1Reset(&sha);
	SHA1Input(&sha, (const unsigned char *) buffer, strlen(buffer));

  for(int i = 0; i < 5 ; i++){
		sprintf(output+i*8, "%X", sha.Message_Digest[i]);
	}

  free(buffer);
  return output;
}
