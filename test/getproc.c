#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXCHAR 1024

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
//	printf("%s\n", addr);
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
    printf("%d", just_count);
    return just_count;
}

int main(){
    char *inodes[1000];
    char *fileName[1000];
    int num = get_inode_fileName(inodes, fileName);
    for (int i = 0; i < num; i++)
	printf("%s %s \n",inodes[i], fileName[i]);
    return 0;
}
