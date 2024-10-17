#ifndef RUSH_H
#define RUSH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_TOKENS 256
#define MAX_ARGS 64
#define MAX_COMMANDS 128
#define MAX_PATH_LEN 256

typedef struct Command {
    char **args;
    int arg_count;
    int is_redirect;
    char *output_file;
} Command;

typedef struct Node {
    char *path;
    struct Node *next;
} Node;

typedef struct PathList {
    Node *head;
    int length;
} PathList;

// Parser functions
int is_empty_line(char *line);
Command **parse_input(char *input, int *command_count);
char **tokenize(char *line);
Command *create_command();
void free_commands(Command **commands, int command_count);

// Executor functions
void execute_commands(Command **commands, int command_count, PathList *shell_path);
int handle_builtin(Command *cmd, PathList *shell_path);
char *find_executable(char *command, PathList *shell_path);

// Path manager functions
PathList *init_path();
void add_path(PathList *plist, char *path);
void clear_path(PathList *plist);
void update_path(PathList *plist, Command *cmd);

// Utility functions
void print_error();

#endif