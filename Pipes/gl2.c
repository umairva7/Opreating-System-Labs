#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


int main(int argc, char *argv[]) {
    int size = argc - 1;
    int *data = (int *)malloc(size * sizeof(int));

    for (int i = 1; i <= size; i++) {
        data[i - 1] = atoi(argv[i]);
        
    }

    int mid=size/2;
    int *data1 = (int *)malloc(mid * sizeof(int));
    int *data2 = (int *)malloc(mid * sizeof(int));

    for(int i=0;i<size;i++)
    {
        
    }
    

    return 0;
}