#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXCHAR 1024

int isDigt(char* str){
    int end = strlen(str);
    for (int i =0; i< end; i++)
        if (isdigit( str[i] ) == 0)
             return 0;
    return 1;
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
    while ((pDirent = readdir(pDir)) != NULL) {
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

int main (int argc, char *argv[]) {

    int proc_arr[500]={0};
    char addr_proc[30] = "/proc";
    getFileN(addr_proc, proc_arr);
//    print_arr(proc_arr);

/*
//    char str[1000] = "lr-x------ 1 dragonfly dragonfly 64  十  15 20:57 27 -> /var/lib/apt/lists/ftp (deleted)";
//    printf("inode= %s \n", find_inode(str));

    char addr[30] = "/proc/5717/fd/";
    int fd_arr[100]={0};
    getFileN(addr, fd_arr);
    //print_arr(fd_arr);

    int tmp = 169;
    int tmp1 = 1;
    printf("%d/fd/%d= %s\n", proc_arr[tmp] , fd_arr[tmp1], rdLink(proc_arr[tmp], fd_arr[tmp1]));
*/
    int end_proc = arr_len(proc_arr);

    for(int i = 0; i < end_proc; i++){
	int fd_arr[100] = {0};
	char addr[100] = "";
	sprintf( addr, "/proc/%d/fd/", proc_arr[i] );
	printf("%s\n", addr);
/*	getFileN(addr,fd_arr); */

//      the problem is without using sudo Permission Deny, lead to segmentation fail;
//	using sudo will change proc_arr for no reason... lead to "proc/X/fd/" where X didn't exist...



//	int end_fd = arr_len(fd_arr);
//	for (int j = 0; j< end_fd; j++){
//            printf("%d/fd/%d= %s\n", proc_arr[i] , fd_arr[j], rdLink(proc_arr[i], fd_arr[j]));
//	}
    }
    return 0;
}

