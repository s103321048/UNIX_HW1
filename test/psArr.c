#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
int isDigt(char* str){
    int end = strlen(str);
    for (int i =0; i< end; i++)
        if (isdigit( str[i] ) == 0)
             return 0;
    return 1;
}

void passArr(char *addr, int *arr){
    struct dirent *pDirent;
    DIR *pDir;
    pDir = opendir(addr);
    int where = 0;
    while ((pDirent = readdir(pDir)) != NULL) {
//      printf ("[%s]\n", pDirent->d_name);
        if (isDigt(pDirent->d_name))
            arr[where++] = strtol(pDirent->d_name,NULL,16);

    }
    closedir (pDir);
    for(int i = where; i >= 0; i--)
    {
        printf("%d \n", arr[i]);
    }

}

int main(){
    char addr[30] = "/proc/5717/fd/";
    int arr[100];
    passArr(addr, arr);
}
