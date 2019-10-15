#include <stdio.h>  
#include <getopt.h>  
char *l_opt_arg;  
char* const short_options = "tu";  
struct option long_options[] = {  
     { "tcp",     0,   NULL,    't'     },  
     { "udp",     0,   NULL,    'u'     },  
     { "love",     1,   NULL,    'l'     },  
     {      0,     0,     0,     0},  
};  
int main(int argc, char *argv[])  
{  
     int c;  
     while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)  
     {  
         switch (c)  
         {  
         case 't':  
             printf("TCP.\n");  
             break;  
         case 'u':  
             printf("UDP.\n");  
             break;  
         case 'l':  
             l_opt_arg = optarg;  
             printf("Our love is %s!\n", l_opt_arg);  
             break;  
         }  
     }  
     return 0;  
}  
