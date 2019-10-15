#include <stdlib.h>
#include<stdio.h>
#include<string.h>

void split(char **arr, char *str, const char *del) {
    char *s = strtok(str, del);
    while(s != NULL) {
        *arr++ = s;
        s = strtok(NULL, del);
    }
}


char* str2ip(char *target){
    return target;
}

int main(){
    char *arr[100];
    const char *del = ":";
    char target[20];
    char port[20];
    char str[20] = "020011AC:D218";
    split(arr, str, del);
    strcpy(target, *(arr) );
    strcpy(port, *(arr+1) );
//    printf( "%s, %li", str2ip(target), strtol(port, NULL, 16) );
//   printf( "%li", strtol(port, NULL, 16) );

    char addr[100];
    for (int i=0; i<8; i+=2){
        char str2[3];
        char str3[3];
        char str4[10];
        strncpy( str2, target, i+2 );
        str2[2] = '\0';
        strncpy( str3, target + i, 2 );
        str3[2] = '\0';
//        printf("%s ", str3);
     //   printf( "%li", strtol(str2,NULL,16) );
        sprintf(str4, "%ld", strtol(str3,NULL,16));
        if (i!=6)
            strcat(str4, ".");
        else
            strcat(str4, ":");
        strcat(addr, str4);
    }
    char str5[10];
    sprintf(port, "%ld", strtol(port, NULL, 16));
    strcat(addr, port);
    printf("%s ",addr);
        
        
    //str2ip(target);
}
