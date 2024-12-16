#ifndef CONFIRMATION_H
#define CONFIRMATION_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


#define PIPE_NAME "/tmp/tk_to_c_pipe"

// int listen_for_confirmation() {
//     int fd;
//     char buffer[256];

//     // Open the named pipe for reading
//     printf("Listening for confirmation...\n");
//     fd = open(PIPE_NAME, O_RDONLY);
//     if (fd == -1) {
//         perror("Failed to open pipe");
//         return -1;
//     }

//     // Continuously read messages from the pipe
//     while (1) {
//         ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
//         if (bytes_read > 0) {
//             buffer[bytes_read] = '\0'; // Null-terminate the string
//             return atoi(buffer);
//         }
//     }

//     close(fd);
//     return 0;
// }

int listen_for_confirmation() {
    int fd;
    char buffer[256];

    printf("Listening for confirmation...\n");
    fd = open(PIPE_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open pipe");
        return -1; // Return an error code
    }

    while (1) {
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            close(fd);
            return atoi(buffer);
        }
    }

    close(fd);
    return -1;
}


void trigger_tk(const char *message) {
    pid_t pid = fork();  // Fork a new process
    if (pid == -1) {
        perror("Failed to fork process");
        return;
    }

    if (pid == 0) {  // Child process
        execlp("python3", "python3", "tkinter_app.py", message, NULL);
        perror("Failed to execute Python script");
        exit(EXIT_FAILURE);
    }
}

// int confirm_function(char* shop_list) {
//     // Create the named pipe if it doesn't exist
//     int output = 0;

//     if (mkfifo(PIPE_NAME, 0666) == -1 && errno != EEXIST) {
//         perror("Failed to create pipe");
//         return 1;
//     }

//     // Fork a process to listen for confirmations
//     pid_t listener_pid = fork();
//     if (listener_pid == -1) {
//         perror("Failed to fork process");
//         return 1;
//     }

//     if (listener_pid == 0) {
//         // Child process: handle listening
//         output = listen_for_confirmation();
//         exit(0);
//     }

//     trigger_tk(shop_list);

//     return output;
// }

int confirm_function(char* shop_list) {
    int output = 0;

    if (mkfifo(PIPE_NAME, 0666) == -1 && errno != EEXIST) {
        perror("Failed to create pipe");
        return 1;
    }

    pid_t listener_pid = fork();
    if (listener_pid == -1) {
        perror("Failed to fork process");
        return 1;
    }

    if (listener_pid == 0) {
        // Child: handle listening
        output = listen_for_confirmation();
        // write output somewhere the parent can access, such as:
        // - return it as an exit status:
        exit(output);
    }

    // Parent process
    trigger_tk(shop_list);

    // Wait for the listener to finish and get its exit status
    int status;
    waitpid(listener_pid, &status, 0)ک
    if (WIFEXITED(status)) {
        // Retrieve child's exit status as the user's confirmation
        output = WEXITSTATUS(status);
    }

    return output;
}


#endif