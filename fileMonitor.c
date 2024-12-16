#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "text_monitor.h"
#include "mainFunction.h"


void new_user(const char *line) {
    printf("New Line Detected: %s\n", line);

    int count;
    char **parsed = parse_line(line, &count);

    // Format of line: user_name, id, threshold, is_repeated, good1, count1, good2, count2, ...

    // Step 1: Extract user (shop ID)
    int user = atoi(parsed[1]);  // shop_id is at index 1
    int treshold = atoi(parsed[2]);
    int is_repeated = atoi(parsed[3]);

    // Step 2: Initialize arrays for goods and counts
    int max_goods = (count - 4) / 2;  // Goods start at index 4 (after threshold, is_repeated)
    char **items = malloc(max_goods * sizeof(char*));
    int *numitems = malloc(max_goods * sizeof(int));
    int n = 0;  // Count of different types of goods

    // Step 3: Populate items and numitems
    for (int i = 4; i < count; i += 2) {
        items[n] = strdup(parsed[i]);        // Copy the good name
        numitems[n] = atoi(parsed[i + 1]);   // Convert count to integer
        n++;
    }

    // Step 4: Call main_function with extracted parameters
    printf("Calling main_function with user: %d, n: %d\n", user, n);
    for (int i = 0; i < n; i++) {
        printf("Item[%d]: %s, Count: %d\n", i, items[i], numitems[i]);
    }

    main_function(user, items, numitems, n, treshold, is_repeated);

    // Step 5: Free dynamically allocated memory
    for (int i = 0; i < n; i++) {
        free(items[i]);  // Free each string in items
    }
    free(items);
    free(numitems);

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
// void run_other_functionality() {
//     printf("Running other tasks in the main thread...\n");
//     for (int i = 0; i < 1000; i++) {
//         printf("Main thread task iteration %d\n", i + 1);
//         sleep(1);
//     }
// }

int main() {
    pthread_t monitor_thread;

    // Start the file monitoring thread
    if (pthread_create(&monitor_thread, NULL, file_monitor, NULL) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    // Fork to launch the Python Tkinter GUI
    pid_t pid = fork();
    if (pid == -1) {
        perror("Failed to fork");
        return 1;
    } else if (pid == 0) {
        // Child process: run the Python script
        execlp("python3", "python3", PYTHON_SCRIPT, (char *)NULL);
        // If execlp fails:
        perror("Failed to exec python script");
        exit(1);
    } else {
        // Parent process: run other functionality
        
        // run_other_functionality();

        // Wait for the file monitoring thread to finish
        pthread_join(monitor_thread, NULL);
    }

    signal(SIGINT, handle_signal);   // Ctrl+C
    signal(SIGTERM, handle_signal);  // Termination


    return 0;
}

