#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trigger_tk(const char *message) {
    char command[512];
    snprintf(command, sizeof(command), "python3 tkinter_app.py \"%s\"", message);
    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to execute the Tkinter app.\n");
    }
}

int main() {
    char message[10];
    for (int i = 0; i< 3; i++)
    {
        scanf("%10s", message);
        trigger_tk(message);
    }
    return 0;
}
