#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "utils.h"

void init_command_list(CommandList *cmd_list) {
    cmd_list->commands = NULL;
    cmd_list->count = 0;
}

void free_command_list(CommandList *cmd_list) {
    for (int i = 0; i < cmd_list->count; i++) {
        Command *cmd = &cmd_list->commands[i];
        for (int j = 0; j < cmd->argc; j++) {
            free(cmd->argv[j]);
        }
        free(cmd->argv);
        if (cmd->output_redirect) {
            free(cmd->output_file);
        }
    }
    free(cmd_list->commands);
}

void parse_input(char *input, CommandList *cmd_list) {
    char *rest = input;
    char *token;
    while ((token = strsep(&rest, "&")) != NULL) {
        if (is_empty_line(token)) {
            continue;
        }
        cmd_list->commands = realloc(cmd_list->commands, sizeof(Command) * (cmd_list->count + 1));
        Command *cmd = &cmd_list->commands[cmd_list->count];
        parse_command(token, cmd);
        cmd_list->count++;
    }
}

void parse_command(char *command_str, Command *cmd) {
    cmd->argv = NULL;
    cmd->argc = 0;
    cmd->output_redirect = 0;
    cmd->output_file = NULL;

    char *rest = command_str;
    char *token;
    int redirect_found = 0;

    while ((token = strsep(&rest, " \t")) != NULL) {
        if (strlen(token) == 0) {
            continue;
        }
        if (strcmp(token, ">") == 0) {
            if (redirect_found || cmd->output_redirect) {
                cmd->output_redirect = -1; // Error state
                return;
            }
            redirect_found = 1;
            continue;
        }
        if (redirect_found) {
            cmd->output_file = strdup(token);
            cmd->output_redirect = 1;
            redirect_found = 0;
            continue;
        }
        cmd->argv = realloc(cmd->argv, sizeof(char *) * (cmd->argc + 1));
        cmd->argv[cmd->argc] = strdup(token);
        cmd->argc++;
    }
    cmd->argv = realloc(cmd->argv, sizeof(char *) * (cmd->argc + 1));
    cmd->argv[cmd->argc] = NULL; // Null-terminate argv
}