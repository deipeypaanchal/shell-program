#include <stdlib.h>
#include <string.h>
#include "parser.h"

char** parse_commands(char* line) {
    int count = 0;
    char **commands = malloc(MAX_CMDS * sizeof(char*));
    char *token = strtok(line, "&");

    while (token != NULL && count < MAX_CMDS) {
        commands[count++] = strdup(token);
        token = strtok(NULL, "&");
    }
    commands[count] = NULL;  // Null-terminate the array
    return commands;
}

void free_commands(char **commands) {
    for (int i = 0; commands[i] != NULL; i++) {
        free(commands[i]);
    }
    free(commands);
}

char** parse_arguments(char* command) {
    int count = 0;
    char **args = malloc(MAX_ARGS * sizeof(char*));
    char *token = strtok(command, " \t");

    while (token != NULL && count < MAX_ARGS) {
        args[count++] = strdup(token);
        token = strtok(NULL, " \t");
    }
    args[count] = NULL;  // Null-terminate the array
    return args;
}

void free_arguments(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        free(args[i]);
    }
    free(args);
}