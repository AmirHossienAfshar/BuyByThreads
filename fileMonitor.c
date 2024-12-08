#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *file_monitor(void *arg) {
    struct stat file_stat;
    time_t last_modified = 0;

    printf("Monitoring file changes in a separate thread...\n");

    while (1) {
        if (stat("shopping_data.txt", &file_stat) == 0) {
            if (file_stat.st_mtime != last_modified) {
                last_modified = file_stat.st_mtime;
                
                // Simulate processing the file when changes are detected
                FILE *file = fopen("shopping_data.txt", "r");
                if (file) {
                    char line[256];
                    printf("File content after change:\n");
                    while (fgets(line, sizeof(line), file)) {
                        printf("%s", line);
                    }
                    fclose(file);
                } else {
                    printf("Error: Could not open file.\n");
                }
            }
        } else {
            printf("File not found. Waiting for creation...\n");
        }
        sleep(1);  // Check every second
    }

    return NULL;
}

void run_other_functionality() {
    printf("Running other tasks in the main thread...\n");
    for (int i = 0; i < 100; i++) {
        printf("Main thread task iteration %d\n", i + 1);
        sleep(2);
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

    // Wait for the file monitoring thread to finish (if needed)
    pthread_join(monitor_thread, NULL);

    return 0;
}
