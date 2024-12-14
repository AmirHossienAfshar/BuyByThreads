#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PIPE_NAME "/tmp/tk_to_c_pipe"

void process_confirmation(const char *message) {
    printf("C received confirmation: %s\n", message);
}

void listen_for_confirmation() {
    int fd;
    char buffer[256];

    // Open the named pipe for reading
    printf("Listening for confirmation...\n");
    fd = open(PIPE_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open pipe");
        return;
    }

    // Continuously read messages from the pipe
    while (1) {
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the string
            process_confirmation(buffer);
        }
    }

    close(fd);
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

int main() {
    // Create the named pipe if it doesn't exist
    if (mkfifo(PIPE_NAME, 0666) == -1 && errno != EEXIST) {
        perror("Failed to create pipe");
        return 1;
    }

    // Fork a process to listen for confirmations
    pid_t listener_pid = fork();
    if (listener_pid == -1) {
        perror("Failed to fork process");
        return 1;
    }

    if (listener_pid == 0) {
        // Child process: handle listening
        listen_for_confirmation();
        exit(0);
    }

    // main program logic
    char message[256];
    for (int i = 0; i < 3; i++) {
        printf("Enter a message: ");
        if (scanf("%255s", message) != 1) {
            fprintf(stderr, "Error: Failed to read input.\n");
            break;
        }
        trigger_tk(message);
    }

    return 0;
}
