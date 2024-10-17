#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "utils.h"

void print_error() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

int is_empty_line(char *line) {
    for (int i = 0; line[i]; i++) {
        if (!isspace((unsigned char)line[i])) {
            return 0;
        }
    }
    return 1;
}
