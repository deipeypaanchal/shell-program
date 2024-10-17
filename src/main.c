#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "executor.h"
#include "path_manager.h"
#include "utils.h"

#define PROMPT "rush> "

int main(int argc, char *argv[]) {
    if (argc != 1) {
        print_error();
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;

    // Initialize path manager with default path
    PathManager path_manager;
    init_path_manager(&path_manager);
    add_path(&path_manager, "/bin");

    while (1) {
        printf("%s", PROMPT);
        fflush(stdout);

        ssize_t nread = getline(&line, &len, stdin);
        if (nread == -1) {
            break; // EOF reached
        }

        // Remove trailing newline
        if (line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }

        if (is_empty_line(line)) {
            continue;
        }

        // Parse the input line into commands
        CommandList cmd_list;
        init_command_list(&cmd_list);
        parse_input(line, &cmd_list);

        // Execute the commands
        execute_commands(&cmd_list, &path_manager);

        // Clean up
        free_command_list(&cmd_list);
    }

    free(line);
    free_path_manager(&path_manager);
    return 0;
}