#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double pi = 0.0;            


typedef struct {
    int start;              // Starting term for this thread
    int end;                // Ending term for this thread
} ThreadArgs;

// Function for each thread to compute its part of the series
void* compute_pi(void* args) {
    ThreadArgs* data = (ThreadArgs*)args;
    double local_sum = 0.0; // Local sum for this thread

    // Calculate the partial sum
    for (int i = data->start; i < data->end; i++) {
        double term = (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1); // (-1)^i / (2i + 1)
        local_sum += term;
    }

    
    pi += local_sum;


    free(data); 
    return NULL;
}

int main(int argc, char* argv[]) {
    
    int num_threads = atoi(argv[1]); // Number of threads
    int n = atoi(argv[2]);           // Number of terms

    // Ensure n is valid and evenly divisible by the number of threads
    if (n <= 100000 || n % num_threads != 0) {
        fprintf(stderr, "Error: n must be greater than 100,000 and evenly divisible by the number of threads.\n");
        return EXIT_FAILURE;
    }

    pthread_t threads[num_threads];  // Array of thread IDs
    int chunk_size = n / num_threads; // Number of terms each thread will process

   
    // Create threads
    for (int i = 0; i < num_threads; i++) {
        ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        args->start = i * chunk_size;          // Start index for this thread
        args->end = (i + 1) * chunk_size;      // End index for this thread
        pthread_create(&threads[i], NULL, compute_pi, (void*)args);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    

    pi *= 4; // Multiply the result by 4 to get the final value of pi

    printf("Estimated value of pi: %.15f\n", pi);
    return 0;
}
