#include <stdio.h>

const char *errorMess; 

void showError(){
	if(NULL != errorMess) {
		printf("%s\n",errorMess);
	}
}