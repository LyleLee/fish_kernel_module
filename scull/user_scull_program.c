#include <stdio.h>
#include <string.h>

int main()
{
    FILE *fp;
    fp = fopen("/dev/scull0","r+");
    if(fp == NULL){
        printf("open scull error\n");
        return -1;
    }
    return 0;
}
