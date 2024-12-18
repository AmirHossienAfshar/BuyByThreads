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
    static char *components[256];
    char *line_copy = strdup(line);
    *count = 0;

    char *token = strtok(line_copy, ",");
    while (token != NULL) {
        components[(*count)++] = strdup(token);
        token = strtok(NULL, ",");
    }

    free(line_copy);
    return components;
}


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
            free(last_line);
        }
        last_line = strdup(temp_line);
    }

    fclose(file);

    if (last_line) {
        size_t len = strlen(last_line);
        while (len > 0 && (last_line[len - 1] == '\n' || last_line[len - 1] == ' ' || last_line[len - 1] == '\r')) {
            last_line[--len] = '\0';
        }
    }
    return last_line;
}

#endif