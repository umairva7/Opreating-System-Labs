#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int size = 0;

// Function to find the average
void* find_avg(void* arr) {
    int* num = (int*)arr;

    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += num[i];
    }

    double* avg = (double*)malloc(sizeof(double));
    *avg = (double)sum / size;

    pthread_exit((void*)avg); 
}

void* find_max(void* arr) {
    int* num = (int*)arr;

    
    int* max=(int*)malloc(sizeof(int));
    *max = num[0];
    for (int i = 1; i < size; i++) {
        if(*max<num[i])
        {
            *max=num[i];
        }
    }

    pthread_exit((void*)max);

     
}


void* find_min(void* arr) {
    int* num = (int*)arr;

    int* min=(int*)malloc(sizeof(int));
    *min = num[0];
    for (int i = 1; i < size; i++) {
        if(*min>num[i])
        {
            *min=num[i];
        }
    }

    pthread_exit((void*)min);

}


int main(int argc, char* argv[]) {
    

    size = argc - 1;
    int* arr = (int*)malloc(size * sizeof(int));
    
    for (int i = 0; i < size; i++) {
        arr[i] = atoi(argv[i + 1]);
        printf("%d ", arr[i]);
    }
    printf("\n");

    
    pthread_t tid1,tid2,tid3;
    double* result;
    int* max,*min;
    pthread_create(&tid1, NULL, find_avg, (void*)arr);
    pthread_create(&tid2, NULL, find_max, (void*)arr);
    pthread_create(&tid3, NULL, find_min, (void*)arr);

    // Wait for the thread to complete
    pthread_join(tid1, (void**)&result) ;
    pthread_join(tid2, (void**)&max) ;
    pthread_join(tid3, (void**)&min) ;

    
    printf("Average: %.2f\n", *result);
    printf("Max: %.d\n", *max);
    printf("Min: %.d\n", *min);


    // Free allocated memory
    free(result);
    free(arr);

    return 0;
}
