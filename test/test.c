#include <stdlib.h> 
#include<stdio.h>
#include <string.h>

#define MAXCHAR 200
void split(char **arr, char *str, const char *del) {
    char *s = strtok(str, del);
    while(s != NULL) {
        *arr++ = s;
        s = strtok(NULL, del);
    }   
}

char * str2ipv4(char *str){
    char *addr = malloc(30);
    char *arr[100];
    const char *del = ":";
    char target[20];
    char port[20];
    split(arr, str, del);
    strcpy(target, *(arr) );
    strcpy(port, *(arr+1) );
    for (int i=6; i>=0; i-=2){
        char str3[3];
        char str4[10];
        strncpy( str3, target + i, 2 );
        str3[2] = '\0';
        sprintf(str4, "%ld", strtol(str3,NULL,16));
        if (i!=0)
            strcat(str4, ".");
        else
            strcat(str4, ":");
        strcat(addr, str4);
    }
    char str5[10];
    sprintf(port, "%ld", strtol(port, NULL, 16));
    strcat(addr, port);
    return addr;
}

void print_ipv6(char* filename){
    FILE *fp;
    char ch;
    char str[MAXCHAR]; // each port

    int first=0;
    printf("%s", str);
    int count = 0;
    char local[MAXCHAR];
    char foreign[MAXCHAR];
    char inode[MAXCHAR];

    if(first==0) first = 1;// delete first line
        else{ // split to local, foregin, inode
        char *arr[100];
        const char *del = " ";
            int i = 0;
            split(arr, str, del);
            while(i<10){
                switch(i){
                    case 1:
                        strcpy(local, *(arr+i));
                        break;
                    case 2:
                        strcpy(foreign, *(arr+i));
                        break;
                    case 9:
                        strcpy(inode,*(arr+i));
                        break;
                    }
                    i++;
            }
//                printf("%*s %*s %*s \n", 20 , str2ipv4(local), 20, str2ipv4(foreign), 15, inode);
        }
}

int main(){
    char* filename = "./source/tcp6.txt";                                                                                           
    print_ipv6(filename);

}
