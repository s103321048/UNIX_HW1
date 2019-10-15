#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

int isDigt(char* str){

    int end = strlen(str);
    for (int i =0; i< end; i++)
        if (isdigit( str[i] ) == 0)
             return 0;
        
    return 1;
	
}


    int main (int c, char *v[]) {
        struct dirent *pDirent;
        DIR *pDir;

        char array[1000][1000];
        if (c < 2) {
            printf ("Usage: testprog <dirname>\n");
            return 1;
        }
        pDir = opendir (v[1]);
        if (pDir == NULL) {
            printf ("Cannot open directory '%s'\n", v[1]);
            return 1;
        }
        int where = 0;
        while ((pDirent = readdir(pDir)) != NULL) {
//            printf ("[%s]\n", pDirent->d_name);
           if (isDigt(pDirent->d_name)) 
                strcpy( array[where++], pDirent->d_name);
            
        }
        closedir (pDir);
        for(int i = where; i >= 0; i--)
        {
                printf("%s \n", array[i]);
            
        }
        return 0;
    }



