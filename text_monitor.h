#ifdef TEXT_MONITOR_H
#define TEXT_MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define FILE_NAME "shopping_data.txt"

void clear_file() {
    FILE *file = fopen(FILE_NAME, "w");
    if (file) {
        fclose(file);
        printf("File cleared on exit.\n");
    }
}

void handle_signal(int signal) {
    clear_file();  // Clear the file before exiting
    exit(0);  // Exit program gracefully
}

char **parse_line(const char *line, int *count) {
    static char *components[256];  // To store parsed components
    char *line_copy = strdup(line);  // Create a mutable copy
    *count = 0;

    // Split into username and shopping list
    char *username = strtok(line_copy, ":");
    char *shopping_list = strtok(NULL, ":");

    if (username && shopping_list) {
        components[(*count)++] = strdup(username);  // Add username

        // Split shopping list by commas
        char *item = strtok(shopping_list, ",");
        while (item != NULL) {
            components[(*count)++] = strdup(item);
            item = strtok(NULL, ",");
        }
    }

    free(line_copy);  // Free allocated memory
    return components;  // Return parsed components
}

void new_user(const char *line) {
    printf("New Line Detected: %s\n", line);

    // Parse the line into components
    int count;
    char **parsed = parse_line(line, &count);

    if (count > 0) {
        printf("Username: %s\n", parsed[0]);
        printf("Shopping List Items:\n");
        for (int i = 1; i < count; i++) {
            printf("  - %s\n", parsed[i]);
        }
    }

    // Simulate long processing (debugging concurrency)
    for (int i = 0; i < 5; i++) {
        sleep(4);
        printf("Handler waited 4 seconds\n");
    }
}

// Function to get the last line from the file
char *get_last_line() {
    FILE *file = fopen(FILE_NAME, "r");
    static char last_line[256];

    if (!file) {
        printf("Error: Could not open file.\n");
        return NULL;
    }
    // Read the last line
    while (fgets(last_line, sizeof(last_line), file)) {
        // Read through all lines
    }
    fclose(file);
    return strlen(last_line) > 0 ? last_line : NULL;
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

#endif