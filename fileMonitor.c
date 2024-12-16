#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "text_monitor.h"

void new_user(const char *line) {
    printf("New Line Detected: %s\n", line);

    int count;
    char **parsed = parse_line(line, &count);

    if (count > 0) {
        printf("Username: %s\n", parsed[0]);

        if (count > 1) {
            printf("Is Repeated: %s\n", parsed[1]);
        }

        printf("Shopping List Items:\n");
        for (int i = 2; i < count; i += 2) {
            if (i + 1 < count) {
                printf("  - %s, Count: %s\n", parsed[i], parsed[i+1]);
            }
        }
    }

    // Rest of your logic...
}

// File monitoring function
void *file_monitor(void *arg) {
    struct stat file_stat;
    time_t last_modified = 0;
    char last_read_line[256] = "";

    printf("Monitoring file changes in a separate thread...\n");

    while (1) {
        if (stat(FILE_NAME, &file_stat) == 0) {
            if (file_stat.st_mtime != last_modified) {
                last_modified = file_stat.st_mtime;
                
                char *new_line = get_last_line(); // Get the last line from the file

  				if (new_line) {
  					
  					// this must call a function that a new user is invoked in the text file.
                    new_user(new_line);
                    
                }
            }
        } else {
            printf("File not found. Waiting for creation...\n");
        }
        sleep(1);  // Check every second
    }
    return NULL;
}

// Simulate other tasks
void run_other_functionality() {
    printf("Running other tasks in the main thread...\n");
    for (int i = 0; i < 1000; i++) {
        printf("Main thread task iteration %d\n", i + 1);
        sleep(1);
    }
}

int main() {
    pthread_t monitor_thread;

    // Start the file monitoring thread
    if (pthread_create(&monitor_thread, NULL, file_monitor, NULL) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    // Run other tasks in the main thread
    run_other_functionality();

    // Wait for the file monitoring thread to finish
    pthread_join(monitor_thread, NULL);



    signal(SIGINT, handle_signal);   // Ctrl+C
    signal(SIGTERM, handle_signal);  // Termination

    return 0;
}





// #define FILE_NAME "shopping_data.txt"

// void clear_file() {
//     FILE *file = fopen(FILE_NAME, "w");
//     if (file) {
//         fclose(file);
//         printf("File cleared on exit.\n");
//     }
// }

// void handle_signal(int signal) {
//     clear_file();  // Clear the file before exiting
//     exit(0);  // Exit program gracefully
// }

// char **parse_line(const char *line, int *count) {
//     static char *components[256];  // To store parsed components
//     char *line_copy = strdup(line);  // Create a mutable copy
//     *count = 0;

//     // Split into username and shopping list
//     char *username = strtok(line_copy, ":");
//     char *shopping_list = strtok(NULL, ":");

//     if (username && shopping_list) {
//         components[(*count)++] = strdup(username);  // Add username

//         // Split shopping list by commas
//         char *item = strtok(shopping_list, ",");
//         while (item != NULL) {
//             components[(*count)++] = strdup(item);
//             item = strtok(NULL, ",");
//         }
//     }

//     free(line_copy);  // Free allocated memory
//     return components;  // Return parsed components
// }
// // Function to get the last line from the file
// char *get_last_line() {
//     FILE *file = fopen(FILE_NAME, "r");
//     static char last_line[256];

//     if (!file) {
//         printf("Error: Could not open file.\n");
//         return NULL;
//     }
//     // Read the last line
//     while (fgets(last_line, sizeof(last_line), file)) {
//         // Read through all lines
//     }
//     fclose(file);
//     return strlen(last_line) > 0 ? last_line : NULL;
// }

