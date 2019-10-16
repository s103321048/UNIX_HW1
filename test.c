#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#define MAXCHAR 200
#include <getopt.h>  

char *inodes[1000];
char *fileName[1000];
int num;

char * addName(char *inode);

char *l_opt_arg;                                                                            
char* const short_options = "tu";  
struct option long_options[] = {   
     { "tcp",     0,   NULL,    't'     },  
     { "udp",     0,   NULL,    'u'     },  
     { "love",     1,   NULL,    'l'     },  
     {      0,     0,     0,     0},  
};  
int isDigt(char* str){
    int end = strlen(str);
    for (int i =0; i< end; i++)
        if (isdigit( str[i] ) == 0)
             return 0;
    return 1;
}
int split_count(char **arr, char *str, const char *del) {
    char *s = strtok(str, del);
    int count = 0;
    while(s != NULL) {
        count++;
        *arr++ = s;
        s = strtok(NULL, del);
    }
    return count;
}


char * cmdline_name(char * str){
    char *name = malloc(2000);
    char *arr[100];
    const char *del = "/";
    int last_split = split_count(arr, str, del);

    strcpy(name, *(arr + last_split - 1) );
    const char *del2 = " ";
    int nothing = split_count(arr, name, del2);

    strcpy(name, *arr);
    //printf("%d, %s \n", last_split, name);

    return name;
}

char * sysCmd(char * cmdstring){ // return system command result in str
   char *tmpfile = "tmp";
   char cmd_string[1024];
   mkstemp(tmpfile);
   sprintf(cmd_string, "%s > %s", cmdstring, tmpfile);
   system(cmd_string);

   char *str = malloc(MAXCHAR);
   FILE * pFile;
   pFile = fopen(tmpfile,"r");
   fgets(str, MAXCHAR, pFile);
   fclose(pFile);
   if ( strcmp(str, "") )
        return str;
//   printf("sysCmd:%s \n", str);
   return "";
}

void split(char **arr, char *str, const char *del) {
    char *s = strtok(str, del);
    while(s != NULL) {
        *arr++ = s;
        s = strtok(NULL, del);
    }   
}
char * rdLink(int arr_proc, int arr_fd){
    char *cmd = malloc(MAXCHAR);
    sprintf(cmd, "readlink /proc/%d/fd/%d", arr_proc, arr_fd);

    return sysCmd(cmd);
}

void getFileN(char *addr, int *arr){
    struct dirent *pDirent;
    DIR *pDir;
    pDir = opendir(addr);
    int where = 0;
    while ( (pDir != NULL) && ( (pDirent = readdir(pDir)) != NULL)) {
//        printf ("[%s]\n", pDirent->d_name);
        if (isDigt(pDirent->d_name))
            arr[where++] = strtol(pDirent->d_name,NULL,10);
    }
    closedir (pDir);

//    printf("after close %s \n", addr);
}

char * find_inode(char* str){
    char *inode = malloc(30);
    char getinode[20] = "";
    int end = strlen(str);
    for (int i = 0; i< end; i++)
        if (str[i] == '['){
            int start = 0;
            i++; // to remove '['
            while(str[i] != ']'){
                getinode[start++] = str[i++];
            }
            //printf("%s", getinode);
        }
    if ( isDigt(getinode) )
        strcpy(inode, getinode);
    else
        return "";
    return inode;
}

void print_arr(int *arr){
    int i = 0;
    while(arr[++i] != 0){
        if (i == 1) // i_ == 0, 1
            printf("%d, %d \n", 0, arr[0]);
        printf("%d, %d \n", i, arr[i]);
    }
}

int arr_len(int *arr){
    int i = 0;
    while(arr[++i] != 0);
    return i;
}

int get_inode_fileName(char **inode, char **fileName) {

    int proc_arr[500]={0};
    char addr_proc[30] = "/proc";
    getFileN(addr_proc, proc_arr);
//    print_arr(proc_arr);

    int end_proc = arr_len(proc_arr);
    int just_count = 0;
    for(int i = 0; i < end_proc; i++){
        int fd_arr[100] = {0};
        char addr[100] = "";
        sprintf( addr, "/proc/%d/fd/", proc_arr[i] );
//      printf("%s\n", addr);
        getFileN(addr,fd_arr);

        int end_fd = arr_len(fd_arr);
        for (int j = 0; j< end_fd; j++){
            char *rd_inode =  find_inode(rdLink(proc_arr[i], fd_arr[j]));
            int int_rd_inode = strlen(rd_inode);

            char cmd[100] = "";
            if (int_rd_inode != 0){
                sprintf(cmd, "cat /proc/%d/cmdline", proc_arr[i]);
//                printf("%d/fd/%d inode<%s> %s\n", proc_arr[i] , fd_arr[j] ,
//                         find_inode( rdLink(proc_arr[i], fd_arr[j]) ), cmdline_name(sysCmd(cmd)) );
                *inode++ = find_inode( rdLink(proc_arr[i], fd_arr[j]) );
                *fileName++ = cmdline_name(sysCmd(cmd)) ;
                just_count++;
            }
        }
    }
//    printf("%d", just_count);
    return just_count;
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
                    printf("%-*s %-*s %-*s \n", 23 , str2ipv6(local), 23, str2ipv6(foreign), 10, addName(inode) );
                }
                else if (strcmp(proto, "tcp")==0 || strcmp(proto, "udp")==0){
                    printf("%-*s", 6 , proto);
                    printf("%-*s %-*s %-*s \n", 23 , str2ipv4(local), 23, str2ipv4(foreign), 15, addName(inode) );
                }
            }
        }
    }
    fclose(fp);
}

char * addName(char *inode){
    char *withName = inode;
    for (int i = 0; i<num; i++){
	if ( !strcmp(inode, inodes[i]) ){
	    strcat(withName, "/");
	    strcat(withName, fileName[i]);
//	    printf("there IS something similar!!!");
	}
    }
    return withName;
}


int main(int argc, char *argv[]){
    system("cat /proc/net/tcp > ./source/tcp.txt");
    system("cat /proc/net/tcp6 > ./source/tcp6.txt");
    system("cat /proc/net/udp > ./source/udp.txt");
    system("cat /proc/net/udp6 > ./source/udp6.txt");
    char* filename_tcp = "./source/tcp.txt";
    char* filename_udp6 = "./source/udp6.txt";
    char* filename_udp = "./source/udp.txt";
    char* filename_tcp6 = "./source/tcp6.txt";


    num = get_inode_fileName(inodes, fileName);

//    for (int i = 0; i < num; i++)
//        printf("%s %s \n",inodes[i], fileName[i]);

    int c;
    int uflag = 0;
    int tflag = 0;
    if (argc == 1){
        uflag = 1;
        tflag = 1;
    }
    while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1){
        switch (c){
        case 't':
            tflag = 1;
            break;
        case 'u':
            uflag = 1;
            break;
        case 'l':
            l_opt_arg = optarg;
            printf("Our love is %s!\n", l_opt_arg);
            break;
        }
    }

    if (tflag == 1){
       printf("List of TCP connections:\n");
       printf("Proto Local Address           Foreign Address         PID/Program name and arguments\n");
       print_ipv(filename_tcp, "tcp");
       print_ipv(filename_tcp6, "tcp6");
    }
    if (uflag == 1){
       printf("\nList of UDP connections:\n");
       printf("Proto Local Address           Foreign Address         PID/Program name and arguments\n");
       print_ipv(filename_udp, "udp");
       print_ipv(filename_udp6, "udp6");
    }
    return 0;
}
