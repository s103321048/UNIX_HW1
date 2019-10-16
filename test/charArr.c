#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void pass0( char **arr){
	char *str="AAA";
	int end = strlen(str);
	for (int i = 0; i<end; i++)
		*arr++ = str;

}

int main(){
	char *arr[100];
	pass0(arr);


	for (int i = 0; i< 10; i++)
		printf( "%d, %s \n" , i, *arr);
	return 0;
}
