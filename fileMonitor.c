#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "text_monitor.h"
#include "mainFunction.h"

pthread_mutex_t line_mutex = PTHREAD_MUTEX_INITIALIZER;


void new_user(const char *line) {
    printf("New Line Detected: this is the new_user function: %s\n", line);

    int count;
    char **parsed = parse_line(line, &count);

    // Format of line: user_name, id, threshold, is_repeated, good1, count1, good2, count2, ...

    char* username = parsed[0];
    int user = atoi(parsed[1]);
    int threshold = atoi(parsed[2]);
    int is_repeated = atoi(parsed[3]);

    // Initialize arrays for goods and counts
    int max_goods = (count - 4) / 2;
    char **items = malloc(max_goods * sizeof(char*));
    int *numitems = malloc(max_goods * sizeof(int));
    int n = 0;

    for (int i = 4; i < count; i += 2) {
        items[n] = strdup(parsed[i]);
        numitems[n] = atoi(parsed[i + 1]);
        n++;
    }

    printf("Calling main_function with user: %d, n: %d\n", user, n);
    for (int i = 0; i < n; i++) {
        printf("Item[%d]: %s, Count: %d\n", i, items[i], numitems[i]);
    }
    
    // Call main_function
    main_function(user, items, numitems, n, threshold, is_repeated, username);

    // Free memory
    for (int i = 0; i < n; i++) free(items[i]);
    free(items);
    free(numitems);
}

void *new_user_thread(void *arg) {
    char *line = (char *)arg;

    // Simulate processing the new user
    printf("[DEBUG] Processing new user: %s\n", line);
    new_user(line); // Call the actual new_user function

    free(line); // Free the dynamically allocated memory
    return NULL;
}

// File monitoring function
void *file_monitor(void *arg) {
    struct stat file_stat;
    time_t last_modified = 0;
    char *last_read_line = NULL;

    printf("Monitoring file changes in a separate thread...\n");

    while (1) {
        if (stat(FILE_NAME, &file_stat) == 0) {
            if (file_stat.st_mtime != last_modified) {
                last_modified = file_stat.st_mtime;

                char *new_line = get_last_line();
                if (new_line) {
                    pthread_mutex_lock(&line_mutex); 
                    if (!last_read_line || strcmp(new_line, last_read_line) != 0) {
                        printf("[DEBUG] New line detected: %s\n", new_line);

                        
                        free(last_read_line);
                        last_read_line = strdup(new_line); 

                        // a new thread to handle the new user
                        pthread_t thread;
                        char *line_copy = strdup(new_line); // Make a copy for the thread
                        if (pthread_create(&thread, NULL, new_user_thread, line_copy) != 0) {
                            perror("Failed to create new_user thread");
                            free(line_copy);
                        } else {
                            pthread_detach(thread);
                        }
                    }
                    pthread_mutex_unlock(&line_mutex);
                    free(new_line);
                }
            }
        } else {
            printf("File not found. Waiting for creation...\n");
        }

        sleep(1);
    }

    free(last_read_line);
    return NULL;
}


int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Failed to fork");
        exit(1);
    }

    if (pid == 0) {
        execlp("python3", "python3", PYTHON_SCRIPT, (char *)NULL);
        perror("Failed to start Python script");
        exit(1);
    } else {
        printf("Started Python script with PID: %d\n", pid);

        pthread_t monitor_thread;
        if (pthread_create(&monitor_thread, NULL, file_monitor, NULL) != 0) {
            perror("Failed to create file monitor thread");
            exit(1);
        }

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Python script exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Python script was terminated by signal %d\n", WTERMSIG(status));
        }

        printf("Terminating C program as the Python script has exited.\n");
        pthread_cancel(monitor_thread);
        pthread_join(monitor_thread, NULL);
        exit(0);
    }
}