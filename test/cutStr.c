#include <stdio.h>
#include <string.h>

int main()
{
     char str1[] = "this is a test";
     char str2[20];
     char str3[30];

     strncpy( str2, str1, 5 );
     str2[5] = '\0';
     strncpy( str3, str1 + 1, 5 );
     str3[5] = '\0';
     printf("%s", str3);
}
