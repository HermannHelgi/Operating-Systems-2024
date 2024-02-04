#include "greet.h"

void greet(int32_t times) {
	(void) times;
	
	for (int i = 1; i <= times; i++){
		printf(i + "Hello World \n");
	} 

}