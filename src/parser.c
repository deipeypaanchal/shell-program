#include "rush.h"

int is_empty_line(char *line) {
    for (int i = 0; line[i]; i++) {
        if (!isspace((unsigned char)line[i])) {
            return 0;
        }
    }
    return 1;
}

Command **parse_input(char *input, int *command_count) {
    char **tokens = tokenize(input);
    Command **commands = malloc(MAX_COMMANDS * sizeof(Command *));
    *command_count = 0;

    Command *current_cmd = create_command();
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], "&") == 0) {
            commands[(*command_count)++] = current_cmd;
            current_cmd = create_command();
        } else if (strcmp(tokens[i], ">") == 0) {
            current_cmd->is_redirect = 1;
            current_cmd->output_file = strdup(tokens[++i]);
        } else {
            current_cmd->args[current_cmd->arg_count++] = strdup(tokens[i]);
        }
    }
    commands[(*command_count)++] = current_cmd;
    free(tokens);
    return commands;
}

char **tokenize(char *line) {
    char **tokens = malloc(MAX_TOKENS * sizeof(char *));
    int idx = 0;
    char *token;
    char *rest = line;

    while ((token = strsep(&rest, " \t\n")) != NULL) {
        if (strlen(token) > 0) {
            tokens[idx++] = strdup(token);
        }
    }
    tokens[idx] = NULL;
    return tokens;
}

Command *create_command() {
    Command *cmd = malloc(sizeof(Command));
    cmd->args = malloc(MAX_ARGS * sizeof(char *));
    cmd->arg_count = 0;
    cmd->is_redirect = 0;
    cmd->output_file = NULL;
    return cmd;
}

void free_commands(Command **commands, int command_count) {
    for (int i = 0; i < command_count; i++) {
        for (int j = 0; j < commands[i]->arg_count; j++) {
            free(commands[i]->args[j]);
        }
        free(commands[i]->args);
        if (commands[i]->output_file) {
            free(commands[i]->output_file);
        }
        free(commands[i]);
    }
    free(commands);
}