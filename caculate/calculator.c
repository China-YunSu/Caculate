#include <stdio.h>

#include "./include/showError.h"
#include "./include/experssion.h"

int main(int argc, char const *argv[])
{
	 char string[50];
	// int index = 0;
	 double vaule = 0;
	// int count = 0;
	
	 printf("������ʽ(���磺 (3 + 2) / 2 ^ 2  )��");
	 gets(string);

	 if(!experssionAnalysis(string, &vaule)) {
	 	showError();
	 } else {
		printf("������ = %.6f\n ",vaule); 	
	 }
}