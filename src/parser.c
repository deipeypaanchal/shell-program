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
    if (!tokens) {
        return NULL;
    }

    Command **commands = malloc(MAX_COMMANDS * sizeof(Command *));
    *command_count = 0;

    Command *current_cmd = create_command();
    int redirect_count = 0;

    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], "&") == 0) {
            if (current_cmd->arg_count > 0 || current_cmd->is_redirect) {
                commands[(*command_count)++] = current_cmd;
            } else {
                // Skip empty commands
                free_command(current_cmd);
            }
            current_cmd = create_command();
            redirect_count = 0;
        } else if (strcmp(tokens[i], ">") == 0) {
            redirect_count++;
            if (redirect_count > 1 || tokens[i + 1] == NULL || (tokens[i + 2] != NULL && strcmp(tokens[i + 2], "&") != 0)) {
                // Error: Multiple redirections or extra arguments after redirection
                print_error();
                free_tokens(tokens);
                free_command(current_cmd);
                free_commands(commands, *command_count);
                *command_count = 0;
                return NULL;
            }
            current_cmd->is_redirect = 1;
            current_cmd->output_file = strdup(tokens[++i]);
        } else {
            current_cmd->args[current_cmd->arg_count++] = strdup(tokens[i]);
        }
    }
    if (current_cmd->arg_count > 0 || current_cmd->is_redirect) {
        commands[(*command_count)++] = current_cmd;
    } else {
        free_command(current_cmd);
    }
    free_tokens(tokens);
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
        free_command(commands[i]);
    }
    free(commands);
}

void free_command(Command *cmd) {
    for (int j = 0; j < cmd->arg_count; j++) {
        free(cmd->args[j]);
    }
    free(cmd->args);
    if (cmd->output_file) {
        free(cmd->output_file);
    }
    free(cmd);
}

void free_tokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}