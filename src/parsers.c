#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/basics.h"
#define BUFFER_SIZE 256

char * profile_for(char * mail)
{
	char * buffer = NULL;

	//Verify everything is allright!
	if(strlen(mail) < 5 || strlen(mail) > (BUFFER_SIZE - 30))
		return NULL;

	for(int i = 0; i < strlen(mail); i++){
		if(mail[i] == '&' || mail[i] == '='){
			return NULL;
		}
	}

	buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
	for(int i = 0; i < BUFFER_SIZE; i++){
		buffer[i] = '\0';
	}

	strcat(buffer, "email=");
	strcat(buffer, mail);
	strcat(buffer, "&uid=10&role=user");

	return buffer;
}

Json parse_json(char * input)
{
	int n;
	Json json;
	int u1;
	int u2;

	//printf("Input: %s\n" , input);

	json.n = 0;
	u1 = 0;
	u2 = 0;
	n = 0;
	//Find &
	while(1){
		while( input[u2] != '\0' && input[u2] != '=' ){
			u2++;
			n++;
		}

		//\0 found. Lets return
		if(input[u2] == '\0'){
			break;
		}

		//= found. Lets copy between p1 and p2 to names and restart
		strncpy(json.names[json.n] , input + u1 , n - 1);
		json.names[json.n][n] = '\0';
		u1 = u2 + 1;
		n = 0;

		while( input[u2] != '\0' && input[u2] != '&' ){
			u2++;
			n++;
		}

		if(n == 0){
			break;
		}

		//& found. Lets copy between p1 and p2 to atributes and restart
		strncpy(json.attributes[json.n] , input + u1 , n - 1);
		json.attributes[json.n][n] = '\0';
		json.n += 1; //This time we also increase number of counts.
		u1 = u2 + 1;
		n = 0;

		//\0 found. Lets return
		if(input[u2] == '\0'){
			break;
		}
	}

	return json;
}

void print_json(Json json)
{
	for(int i = 0; i < json.n; i++){
		printf("%s : %s\n" , json.names[i] , json.attributes[i]);
	}

	return;
}
