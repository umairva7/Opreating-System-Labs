#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int found = 0;  // Global flag to indicate if the string has been found
int total_lines = 0;  // Total lines in the file
int lines_per_thread = 0;  // Number of lines each thread will process
char* filename;  // Global file name
char* key_string;  // String to search for

// Structure to pass data to each thread
typedef struct {
    int start_line;
    int end_line;
} ThreadData;

// Thread function to search for the key string
void* search_for_string(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    FILE* file = fopen(filename, "r");
    char line[1024];
    int current_line = 0;

    // Move to the start line
    for (int i = 0; i < data->start_line; i++) {
        fgets(line, sizeof(line), file);
    }

    // Search the lines assigned to this thread
    for (int i = data->start_line; i < data->end_line && fgets(line, sizeof(line), file); i++) {
        current_line++;
        if (strstr(line, key_string) != NULL) {
            printf("Found \"%s\" at line %d: %s", key_string, i + 1, line);
            found = 1;
            break;  // Exit if found
        }
    }

    fclose(file);
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Usage: %s <filename> <number of threads> <key string>\n", argv[0]);
        return 1;
    }

    filename = argv[1];
    int n = atoi(argv[2]);
    key_string = argv[3];

    // Open the file and count the total lines
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        total_lines++;
    }
    fclose(file);

    lines_per_thread = total_lines / n;

    // Create threads
    pthread_t threads[n];
    ThreadData data[n];

    for (int i = 0; i < n; i++) {
        data[i].start_line = i * lines_per_thread;
        data[i].end_line = (i == n - 1) ? total_lines : (i + 1) * lines_per_thread;

        // Create the thread to search for the string in the assigned lines
        pthread_create(&threads[i], NULL, search_for_string, &data[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    if (!found) {
        printf("The string \"%s\" was not found in the file.\n", key_string);
    }

    return 0;
}
