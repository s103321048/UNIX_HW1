#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

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
            
            int end = strlen(pDirent->d_name);
            int digitflag = 1;
            for (int i =0; i< end; i++)
                if (isdigit( pDirent->d_name[i] ) == 0){
                    digitflag = 0;
                    break;
                }
            
            if (digitflag==1)
                strcpy( array[where++], pDirent->d_name);
            
        }
        closedir (pDir);
        for(int i = where; i >= 0; i--)
        {
                printf("%s \n", array[i]);
            
        }
        return 0;
    }



