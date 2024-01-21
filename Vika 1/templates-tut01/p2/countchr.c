#include "countchr.h"

int countchr(char *string, char c) {
	(void) string;
	(void) c;
	int counter = 0; // How many leters there are
	int iterator = 0; // How far we are through the list
	while (string[iterator] != '\0'){
		if (string[iterator] == c){
			counter++;
		}
		iterator++;
	}
	return counter;
}