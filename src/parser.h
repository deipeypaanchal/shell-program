#ifndef PARSER_H
#define PARSER_H

typedef struct {
    char **argv;
    int argc;
    int output_redirect;
    char *output_file;
} Command;

typedef struct {
    Command *commands;
    int count;
} CommandList;

void init_command_list(CommandList *cmd_list);
void free_command_list(CommandList *cmd_list);
void parse_input(char *input, CommandList *cmd_list);
void parse_command(char *command_str, Command *cmd);

#endif