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
    printf("New Line Detected: this is the new_user function: %s\n", line);

    int count;
    char **parsed = parse_line(line, &count);

//     // Format of line: user_name, id, threshold, is_repeated, good1, count1, good2, count2, ...

    // Extract user information
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


// // dummy new user to check...
// // void new_user(const char *line)
// // {
// //     int i = 0;
// //     printf("\n new user function is called. \n %d", i);
// // }


// void *file_monitor(void *arg) {
//     struct stat file_stat;
//     time_t last_modified = 0;
//     char *last_read_line = NULL; // Track the last processed line

//     printf("Monitoring file changes in a separate thread...\n");

//     while (1) {
//         if (stat(FILE_NAME, &file_stat) == 0) {
//             if (file_stat.st_mtime != last_modified) {
//                 last_modified = file_stat.st_mtime;

//                 // Fetch the last line from the file
//                 char *new_line = get_last_line();
//                 if (new_line) {
//                     // Compare new_line with last_read_line
//                     if (!last_read_line || strcmp(new_line, last_read_line) != 0) {
//                         printf("New Line Detected file monitor funciton: %s\n", new_line);

//                         // Free previous last_read_line and update it
//                         free(last_read_line);
//                         last_read_line = strdup(new_line);
//                         new_user(new_line); // Process the new line
//                     }
//                     free(new_line); // Free dynamically allocated memory from get_last_line
//                 }
//             }
//         } else {
//             printf("File not found. Waiting for creation...\n");
//         }
//         sleep(1);  // Check every second
//     }

//     // Clean up before exiting
//     free(last_read_line);
//     return NULL;
// }
// Mutex for protecting access to shared resources (e.g., last_read_line)
pthread_mutex_t line_mutex = PTHREAD_MUTEX_INITIALIZER;



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
    char *last_read_line = NULL; // Track the last processed line

    printf("Monitoring file changes in a separate thread...\n");

    while (1) {
        // Check the file's modification time
        if (stat(FILE_NAME, &file_stat) == 0) {
            if (file_stat.st_mtime != last_modified) {
                last_modified = file_stat.st_mtime;

                // Fetch the last line from the file
                char *new_line = get_last_line();
                if (new_line) {
                    pthread_mutex_lock(&line_mutex); // Lock before accessing shared resources
                    if (!last_read_line || strcmp(new_line, last_read_line) != 0) {
                        printf("[DEBUG] New line detected: %s\n", new_line);

                        // Free the previous last_read_line and update it
                        free(last_read_line);
                        last_read_line = strdup(new_line); // Update the last processed line

                        // Spawn a new thread to handle the new user
                        pthread_t thread;
                        char *line_copy = strdup(new_line); // Make a copy for the thread
                        if (pthread_create(&thread, NULL, new_user_thread, line_copy) != 0) {
                            perror("Failed to create new_user thread");
                            free(line_copy); // Free memory if thread creation fails
                        } else {
                            pthread_detach(thread); // Detach the thread to allow independent execution
                        }
                    }
                    pthread_mutex_unlock(&line_mutex); // Unlock after updating shared resources
                    free(new_line); // Free the memory allocated by get_last_line
                }
            }
        } else {
            printf("File not found. Waiting for creation...\n");
        }

        sleep(1); // Check every second
    }

    // Clean up before exiting
    free(last_read_line);
    return NULL;
}


int main() { // the newest... prolem was with the waitpid
    pid_t pid = fork();

    if (pid == -1) {
        perror("Failed to fork");
        exit(1);
    }

    if (pid == 0) {
        // Child process: Execute the Python script
        // execlp("python3", "python3", "shopping_app.py", (char *)NULL);
        execlp("python3", "python3", PYTHON_SCRIPT, (char *)NULL);
        perror("Failed to start Python script");
        exit(1);
    } else {
        // Parent process: Monitor the child
        printf("Started Python script with PID: %d\n", pid);

        // Start file monitoring in a separate thread
        pthread_t monitor_thread;
        if (pthread_create(&monitor_thread, NULL, file_monitor, NULL) != 0) {
            perror("Failed to create file monitor thread");
            exit(1);
        }

        // Wait for the child process (Python script) to terminate
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Python script exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Python script was terminated by signal %d\n", WTERMSIG(status));
        }

        // Cleanup and terminate the C program
        printf("Terminating C program as the Python script has exited.\n");
        pthread_cancel(monitor_thread);
        pthread_join(monitor_thread, NULL);
        exit(0);
    }
}

// void *file_monitor(void *arg) {
//     struct stat file_stat;
//     time_t last_modified = 0;
//     char last_read_line[256] = "";

//     printf("Monitoring file changes in a separate thread...\n");

//     while (1) {
//         if (stat(FILE_NAME, &file_stat) == 0) {
//             if (file_stat.st_mtime != last_modified) {
//                 last_modified = file_stat.st_mtime;

//                 char *new_line = get_last_line(); // Get the last line from the file

//                 if (new_line) {
//                     new_user(new_line);
//                 }
//             }
//         } else {
//             printf("File not found. Waiting for creation...\n");
//         }
//         sleep(1);  // Check every second
//     }
//     return NULL;
// }


// int main() {
//     pthread_t monitor_thread;

//     // Start the file monitoring thread
//     if (pthread_create(&monitor_thread, NULL, file_monitor, NULL) != 0) {
//         perror("Failed to create thread");
//         return 1;
//     }

//     // Fork to launch the Python Tkinter GUI
//     pid_t pid = fork();
//     if (pid == -1) {
//         perror("Failed to fork");
//         return 1;
//     } else if (pid == 0) {
//         // Child process: run the Python script
//         execlp("python3", "python3", PYTHON_SCRIPT, (char *)NULL);
//         // If execlp fails:
//         perror("Failed to exec python script");
//         exit(1);
//     } else {
//         // Parent process: run other functionality
        
//         // run_other_functionality();

//         // Wait for the file monitoring thread to finish
//         pthread_join(monitor_thread, NULL);
//     }

//     signal(SIGINT, handle_signal);   // Ctrl+C
//     signal(SIGTERM, handle_signal);  // Termination


//     return 0;
// }

    
// // File monitoring function
// void *file_monitor(void *arg) {
//     struct stat file_stat;
//     time_t last_modified = 0;
//     char last_read_line[256] = "";

//     printf("Monitoring file changes in a separate thread...\n");

//     while (1) {
//         if (stat(FILE_NAME, &file_stat) == 0) {
//             if (file_stat.st_mtime != last_modified) {
//                 last_modified = file_stat.st_mtime;
                
//                 char *new_line = get_last_line(); // Get the last line from the file

//   				if (new_line) {
  					
//   					// this must call a function that a new user is invoked in the text file.
//                     new_user(new_line);
                    
//                 }
//             }
//         } else {
//             printf("File not found. Waiting for creation...\n");
//         }
//         sleep(1);  // Check every second
//     }
//     return NULL;
// }