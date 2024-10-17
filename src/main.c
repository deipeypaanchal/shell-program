#include "rush.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        print_error();
        exit(1);
    }

    // Initialize the shell path
    PathList *shell_path = init_path();

    // Start the shell loop
    while (1) {
        char *input_line = NULL;
        size_t buffer_size = 0;

        printf("rush> ");
        fflush(stdout);
        getline(&input_line, &buffer_size, stdin);

        // Process the input line
        if (is_empty_line(input_line)) {
            free(input_line);
            continue;
        }

        // Parse the input line into commands
        int command_count = 0;
        Command **commands = parse_input(input_line, &command_count);
        free(input_line);

        // Execute the commands
        execute_commands(commands, command_count, shell_path);

        // Clean up
        free_commands(commands, command_count);
    }

    return 0;
}