#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXCHAR 1024
char * rdLink(int arr_proc, int arr_fd);

int isDigt(char* str){
    int end = strlen(str);
    for (int i =0; i< end; i++)
        if (isdigit( str[i] ) == 0)
             return 0;
    return 1;
}

char * sysCmd(char * cmdstring){
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
   return str;
}

void getFileN(char *addr, int *arr){
    struct dirent *pDirent;
    DIR *pDir;
    pDir = opendir(addr);
    int where = 0;
    while ((pDirent = readdir(pDir)) != NULL) {
//      printf ("[%s]\n", pDirent->d_name);
        if (isDigt(pDirent->d_name))
            arr[where++] = strtol(pDirent->d_name,NULL,10);

    }
    closedir (pDir);
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

void print_arr(int *arr_fd){
    int i_ =1;
    printf("%d\n", arr_fd[0]);
    while(arr_fd[i_] != 0)
        printf("%d\n", arr_fd[i_++]);
}

int main (int argc, char *argv[]) {
    struct dirent *pDirent;
    DIR *pDir;

    char proc_array[1000][1000];
    if (argc < 2) {
        printf ("Usage: testprog <dirname>\n");
        return 1;
    }
    pDir = opendir (argv[1]);
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", argv[1]);
        return 1;
    }
    int where = 0;
    while ((pDirent = readdir(pDir)) != NULL) {
//      printf ("[%s]\n", pDirent->d_name);
        if (isDigt(pDirent->d_name)) 
            strcpy( proc_array[where++], pDirent->d_name);
    }
    closedir (pDir);
    for(int i = where; i >= 0; i--);
//        printf("%s \n", proc_array[i]);

//    char str[1000] = "lr-x------ 1 dragonfly dragonfly 64  十  15 20:57 27 -> /var/lib/apt/lists/ftp (deleted)";
//    printf("inode= %s \n", find_inode(str));

    char addr[30] = "/proc/5717/fd/";
    int arr_fd[100]={0};
    getFileN(addr, arr_fd);
//    print_arr(arr_fd);

    int i_ =1;
    while(arr_fd[i_++] != 0){
	char cmd[2000] = "readlink /proc/5717/fd/";
	sprintf(cmd, "%s%d", cmd, arr_fd[i_]);
	if (i_ == 2){
	    sprintf(cmd, "%s%d", cmd, arr_fd[0]);
            printf("%d/fd/%d= %s\n", 5717,arr_fd[0], rdLink(5717, arr_fd[0]));
	    sprintf(cmd, "%s%d", cmd, arr_fd[1]);
            printf("%d/fd/%d= %s\n", 5717,arr_fd[1], rdLink(5717, arr_fd[1]));
	}
        printf("%d/fd/%d= %s\n", 5717,arr_fd[i_], rdLink(5717, arr_fd[i_]));
    }

//    sysreadL()
    return 0;
}

char * rdLink(int arr_proc, int arr_fd){
    char *cmd = malloc(MAXCHAR);
    sprintf(cmd, "readlink /proc/%d/fd/%d", arr_proc, arr_fd);

    return sysCmd(cmd);
}
