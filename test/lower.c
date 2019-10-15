#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * lower(char *str){
    char *addr = malloc(30);
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
    strcpy(addr, str);
    return addr;
}

int main(){
    char str[200] = "BACD0120000000000000000052965732:0277";

    printf("%s \n",lower(str));

}
