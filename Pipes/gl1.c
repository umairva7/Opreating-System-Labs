#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


int main(int argc, char *argv[]) {
    int size = argc - 1;
    int *data = (int *)malloc(size * sizeof(int));

    for (int i = 1; i <= size; i++) {
        data[i - 1] = atoi(argv[i]);
        
    }
    
    int fd1[2],fd2[2];
    __pid_t pid;
    pipe(fd1);
    pipe(fd2);

    write(fd1[1],data,size*sizeof(int));

    pid=fork();
      
    if (pid == 0) {
    // Child Process
    int *buffer = (int *)malloc(size * sizeof(int));

    // Read data from fd1
    read(fd1[0], buffer, size * sizeof(int));

    // Sort the array (Bubble Sort)
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (buffer[j] < buffer[i]) {
                // Properly swap the elements
                int temp = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = temp;
            }
        }
    }

    // Write sorted data to fd2
    write(fd2[1], buffer, size * sizeof(int));

    free(buffer);
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    exit(0); // Exit child process
}


    int *result=(int*)malloc(size*sizeof(int));
    read(fd2[0],result,size*sizeof(int));

    printf("Sorted Array: \n");
    for(int i=0;i<size;i++)
    {
        printf("%d   ",result[i]);
       
    }
     printf("\n");

    close(fd1[1]);
    close(fd1[2]);
    close(fd2[1]);
    close(fd2[2]);
    

    return 0;
}