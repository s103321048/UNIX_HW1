#include <stdlib.h> 
#include<stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXCHAR 200

#include <getopt.h>  
char *l_opt_arg;                                                                            
char* const short_options = "tu";  
struct option long_options[] = {   
     { "tcp",     0,   NULL,    't'     },  
     { "udp",     0,   NULL,    'u'     },  
     { "love",     1,   NULL,    'l'     },  
     {      0,     0,     0,     0},  
};  

void split(char **arr, char *str, const char *del) {
    char *s = strtok(str, del);
    while(s != NULL) {
        *arr++ = s;
        s = strtok(NULL, del);
    }   
}
char * lower(char *str){
    char *addr = malloc(30);
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }   
    strcpy(addr, str);
    return addr;
}

char * nozero(char *str){
    char *result = malloc(10);
    char get[20]="";
    char getrest=0;
    for(int i = 0; str[i]; i++)
        if (str[i]!='0'){
            getrest=i;
            break;
        }   
    strncpy(result, str + getrest, 4-getrest);
    return(result);
}

char * rev_ipv6(char *str){
    char *addr = malloc(30);
    char str1[20]="";
    char str2[20]="";

    char str3[3];
    strncpy( str3, str + 6, 2 );
    str3[2] = '\0';
    strcat(str1, lower(str3) );

    strncpy( str3, str + 4, 2 );
    str3[2] = '\0';
    strcat(str1, lower(str3) );

    strncpy( str3, str + 2, 2 );
    str3[2] = '\0';
    strcat(str2, lower(str3) );

    strncpy( str3, str + 0, 2 );
    str3[2] = '\0';
    strcat(str2, lower(str3) );

    char str4[20]="";
    if(strcmp(str1,"0000")==0 && strcmp(str2,"0000")==0)
        strcpy(addr, "");
    else if(strcmp(str1,"0000")!=0 && strcmp(str2,"0000")==0){
        strcpy(addr, nozero(str1));
    }   
    else if(strcmp(str1,"0000")==0 && strcmp(str2,"0000")!=0){
        strcpy(addr, nozero(str2));
    }   
    else{
        strcat(addr, nozero(str1) );
        strcat(addr, ":" );
        strcat(addr, nozero(str2) );
    }   
//    printf("??? str1=%s str2=%s \n", str1, str2);
    return addr;
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
    int int_port = strtol(port,NULL,16);
    if (int_port == 0)
        strcat(addr, "*");
    else{
        sprintf(port, "%d", int_port);
        strcat(addr, port);
    }   
    return addr;
}

char * str2ipv6(char *str){
    char *addr = malloc(50);
    char *arr[100];
    const char *del = ":";
    char target[20];
    char port[20];
    split(arr, str, del);
    strcpy(target, *(arr) );
    strcpy(port, *(arr+1) );
    for (int i=0; i<32; i+=8){
        char str3[10];
        char str4[10];
        strncpy( str3, target + i, 8 );
        str3[8] = '\0';
        strcat(addr, rev_ipv6(str3));
        if (i!=0)
            strcat(addr, ":");
//        printf("%s", rev_ipv6(str3) );
    }
    char str5[10]; //connet port             
    if (strcmp(port, "")==0 || strcmp(port,"0000")==0)
        strcat(addr, "*");
    else{
        sprintf(port, "%ld", strtol(port,NULL,16));
        strcat(addr, port);
    }
    return addr;
}

void print_ipv(char* filename, char* proto){
    FILE *fp;
    char ch;
    char str[MAXCHAR]; // each port

    // open file
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
    }
    else{
        int first=0;
        while (fgets(str, MAXCHAR, fp) != NULL){
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
                if (strcmp(proto, "tcp6")==0 || strcmp(proto, "udp6")==0){
                    printf("%-*s", 6 , proto);
                    printf("%-*s %-*s %-*s \n", 23 , str2ipv6(local), 23, str2ipv6(foreign), 10, inode);
                }
                else if (strcmp(proto, "tcp")==0 || strcmp(proto, "udp")==0){
                    printf("%-*s", 6 , proto);
                    printf("%-*s %-*s %-*s \n", 23 , str2ipv4(local), 23, str2ipv4(foreign), 15, inode);
                }
            }
        }
    }
    fclose(fp);
}


int main(int argc, char *argv[]){
    char* filename_tcp = "./source/tcp_file";
    char* filename_udp6 = "./source/udp6.txt";
    char* filename_udp = "./source/udp.txt";
    char* filename_tcp6 = "./source/tcp6.txt";
//    system("cat /proc/net/tcp > a");
//    system("cat /proc/net/tcp6 >> a");
//    system("cat /proc/net/udp >> a");
//    system("cat /proc/net/udp6 >> a");

     int c;
     while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)
     {
         switch (c){
         case 't':
             printf("List of TCP connections:\n");
             printf("Proto Local Address           Foreign Address         PID/Program name and arguments\n");
             print_ipv(filename_tcp, "tcp");
             print_ipv(filename_tcp6, "tcp6");
             break;
         case 'u':
             printf("List of UDP connections:\n");
             printf("Proto Local Address           Foreign Address         PID/Program name and arguments\n");
             print_ipv(filename_udp, "udp");
             print_ipv(filename_udp6, "udp6");
             break;
         case 'l':
             l_opt_arg = optarg;
             printf("Our love is %s!\n", l_opt_arg);
             break;
         }
    //     if (argv[optind] == NULL || argv[optind + 1] == NULL) {
         if (argv[optind+1] == NULL) {
             printf("List of TCP connections:\n");
             printf("Proto Local Address           Foreign Address         PID/Program name and arguments\n");
             print_ipv(filename_tcp, "tcp");
             print_ipv(filename_tcp6, "tcp6");
             printf("\n");
             printf("List of UDP connections:\n");
             printf("Proto Local Address           Foreign Address         PID/Program name and arguments\n");
             print_ipv(filename_udp, "udp");
             print_ipv(filename_udp6, "udp6");
         }
     
     }
     
    return 0;
}
