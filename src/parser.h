#ifndef PARSER_H
#define PARSER_H

#define MAX_CMDS 50
#define MAX_ARGS 50

char** parse_commands(char* line);
void free_commands(char **commands);
char** parse_arguments(char* command);
void free_arguments(char **args);

#endif