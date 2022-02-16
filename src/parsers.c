#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/basics.h" 

#define JSON_CAPACITY 10
#define STR_SIZE 128
#define BUFFER_SIZE 256

typedef struct Json {
	char names[JSON_CAPACITY][STR_SIZE];
	char attributes[JSON_CAPACITY][STR_SIZE];
	int n;
} Json;

char * profile_for(char * mail)
{
	char * buffer;

	//Verify everything is allright!
	if(strlen(mail) < 5 || strlen(mail) > (BUFFER_SIZE - 30))
		return NULL;

	for(int i = 0; i < strlen(mail); i++){
		if(mail[i] == '&' || mail[i] == '='){
			return NULL;
		}
	}

	buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
	strcat(buffer, "email=");
	strcat(buffer, mail);
	strcat(buffer, "&uid=10&role=user");	
	
	return buffer;	
}

Json parse_json(char * input){
	char * p1 = input;
	char * p2 = p1;
	int n = 0;
	Json json;
	
	//Find &
	while(1){
		while( *p2 != '\0' && *p2 != '=' ){
			p2++;
			n++;
		}

		//\0 found. Lets return
		if(*p2 == '\0'){ 
			break;
		}
		
		//= found. Lets copy between p1 and p2 to names and restart
		strncpy(json.names[json.n] , p1 , n);
		json.names[json.n][n] = '\0';
		p1 = p2;
		n = 0;
		
		while( *p2 != '\0' && *p2 != '&' ){
			p2++;
			n++;
		}

		if(n == 0){ 
			break;
		}		

		//& found. Lets copy between p1 and p2 to atributes and restart
		strncpy(json.attributes[json.n] , p1 , n);
		json.names[json.n][n] = '\0';
		json.n += 1; //This time we also increase number of counts.
		p1 = p2;
		n = 0;

		//\0 found. Lets return
		if(*p2 == '\0'){ 
			break;
		}

	}
		
	return json;
}

void print_json(Json json){
	for(int i = 0; i < json.n; i++){
		printf("%s : %s\n" , json.names[i] , json.attributes[i]);
	}
	
	return;
}



