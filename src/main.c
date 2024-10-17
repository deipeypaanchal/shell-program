#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "executor.h"
#include "path.h"
#include "error.h"

int main() {
    char *line = NULL;
    size_t len = 0;
    
    init_path();  // Initialize the default path

    while (1) {
        printf("rush> ");
        fflush(stdout);

        if (getline(&line, &len, stdin) == -1) {
            break;  // Exit on EOF
        }

        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) {
            continue;  // Ignore empty lines
        }

        char **commands = parse_commands(line);  // Split into individual commands
        execute_commands(commands);  // Execute parsed commands

        free_commands(commands);
    }

    free(line);
    return 0;
}