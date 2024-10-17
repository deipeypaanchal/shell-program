#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Add this line to declare exit()
#include "executor.h"
#include "parser.h"
#include "path.h"
#include "error.h"

void execute_commands(char **commands) {
    for (int i = 0; commands[i] != NULL; i++) {
        char **args = parse_arguments(commands[i]);
        if (args[0] == NULL) {
            free_arguments(args);
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            exit(0);  // This line requires stdlib.h
        } else if (strcmp(args[0], "cd") == 0) {
            handle_cd(args);
        } else if (strcmp(args[0], "path") == 0) {
            update_path(args);
        } else {
            handle_execution(args);
        }

        free_arguments(args);
    }
}

void handle_cd(char **args) {
    if (args[1] == NULL || args[2] != NULL) {
        print_error();
        return;
    }
    if (chdir(args[1]) != 0) {
        print_error();
    }
}

void handle_execution(char **args) {
    char *redirect_file = check_redirection(args);
    if (redirect_file) {
        int fd = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd < 0) {
            print_error();
            return;
        }
        dup2(fd, STDOUT_FILENO);
    }

    char *cmd_path = find_command_path(args[0]);
    if (cmd_path != NULL) {
        pid_t pid = fork();
        if (pid == 0) {
            execv(cmd_path, args);
            print_error();
            exit(1);
        } else if (pid > 0) {
            wait(NULL);
        } else {
            print_error();
        }
    } else {
        print_error();
    }

    if (redirect_file) {
        close(STDOUT_FILENO);
    }
}

char* check_redirection(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            if (args[i + 1] != NULL && args[i + 2] == NULL) {
                args[i] = NULL;  // Remove redirection symbol
                return args[i + 1];
            } else {
                print_error();
                return NULL;
            }
        }
    }
    return NULL;
}