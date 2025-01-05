#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int size = argc - 1;
    int *array = (int *)malloc(size * sizeof(int));

    for (int i = 1; i <= size; i++) {
        array[i - 1] = atoi(argv[i]);
    }

    int fd1[2], fd2[2], fd3[2], fd4[2];
    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child 1: Calculate Sum
        close(fd1[1]); // Close write end
        int *array2 = (int *)malloc(size * sizeof(int));
        read(fd1[0], array2, size * sizeof(int));
        close(fd1[0]);

        int sum = 0;
        for (int i = 0; i < size; i++) {
            sum += array2[i];
        }

        write(fd3[1], &sum, sizeof(int));
        close(fd3[1]);

        free(array2);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child 2: Calculate Max
        close(fd2[1]); // Close write end
        int *array2 = (int *)malloc(size * sizeof(int));
        read(fd2[0], array2, size * sizeof(int));
        close(fd2[0]);

        int max = array2[0];
        for (int i = 1; i < size; i++) {
            if (array2[i] > max) {
                max = array2[i];
            }
        }

        write(fd4[1], &max, sizeof(int));
        close(fd4[1]);

        free(array2);
        exit(0);
    }

    // Parent process
    close(fd1[0]);
    close(fd2[0]);
    write(fd1[1], array, size * sizeof(int));
    write(fd2[1], array, size * sizeof(int));
    close(fd1[1]);
    close(fd2[1]);

    wait(NULL); // Wait for Child 1
    wait(NULL); // Wait for Child 2

    int sum, max;
    read(fd3[0], &sum, sizeof(int));
    read(fd4[0], &max, sizeof(int));
    close(fd3[0]);
    close(fd4[0]);

    printf("Sum is %d\n", sum);
    printf("Maximum is %d\n", max);

    free(array);
    return 0;
}
