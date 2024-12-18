#ifndef TEXT_MONITOR_H
#define TEXT_MONITOR_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#define FILE_NAME "shopping_data.txt"
#define PYTHON_SCRIPT "userInput.py"


char **parse_line(const char *line, int *count) {
    static char *components[256]; // To store parsed components
    char *line_copy = strdup(line);
    *count = 0;

    char *token = strtok(line_copy, ",");
    while (token != NULL) {
        components[(*count)++] = strdup(token);
        token = strtok(NULL, ",");
    }

    free(line_copy);
    return components;  // components[0] = username, components[1] = is_repeated, ...
}

// // Function to get the last line from the file, old one, used to work, but had some bugs.
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

char *get_last_line() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("Error: Could not open file.\n");
        return NULL;
    }

    char temp_line[256];
    char *last_line = NULL;


    while (fgets(temp_line, sizeof(temp_line), file)) {
        if (last_line) {
            free(last_line); // Free previous allocation
        }
        last_line = strdup(temp_line); // Duplicate the current line
    }

    fclose(file);

    if (last_line) {
        // Remove trailing newline or spaces
        size_t len = strlen(last_line);
        while (len > 0 && (last_line[len - 1] == '\n' || last_line[len - 1] == ' ' || last_line[len - 1] == '\r')) {
            last_line[--len] = '\0';
        }
    }
    return last_line; // Return dynamically allocated last line
}



/////////////////////////////////////////////// these two functions were not working and there will be changes on them soon.
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

#endif