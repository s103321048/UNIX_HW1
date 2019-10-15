#include <stdio.h>
#include <string.h>


void split(char **arr, char *str, const char *del) {
    char *s = strtok(str, del);
    while(s != NULL) {
        *arr++ = s;
        s = strtok(NULL, del);
    }
}
 
int main() {
    char str[100] = "10,20,30";
    char *arr[100];
    const char *del = ",";
    int i = 0;
    split(arr, str, del);
    while(i<3)
        printf("%s\n", *(arr+i++));
}
