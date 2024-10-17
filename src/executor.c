#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "executor.h"
#include "parser.h"
#include "path.h"
#include "error.h"

void execute_commands(char **commands) {
    pid_t pids[MAX_CMDS];
    int parallel_count = 0;

    for (int i = 0; commands[i] != NULL; i++) {
        char **args = parse_arguments(commands[i]);

        if (args[0] == NULL) {
            free_arguments(args);
            continue;
        }

        pids[parallel_count] = fork();
        if (pids[parallel_count] == 0) {
            // In child process
            if (strcmp(args[0], "exit") == 0) {
                handle_exit(args);
            } else if (strcmp(args[0], "cd") == 0) {
                handle_cd(args);
            } else if (strcmp(args[0], "path") == 0) {
                update_path(args);
            } else {
                handle_execution(args);  // Execute the command
            }
            exit(0);  // Exit child process after handling the command
        } else if (pids[parallel_count] < 0) {
            print_error();
        }

        parallel_count++;
        free_arguments(args);
    }

    // Wait for all parallel processes to complete
    for (int i = 0; i < parallel_count; i++) {
        waitpid(pids[i], NULL, 0);
    }
}

void handle_exit(char **args) {
    if (args[1] != NULL) {
        print_error();  // Exit command should not take any arguments
    } else {
        exit(0);  // Exit the shell if no arguments are provided
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
        execv(cmd_path, args);
        print_error();
        exit(1);
    } else {
        print_error();
    }

    if (redirect_file) {
        close(STDOUT_FILENO);
    }
}

char* check_redirection(char **args) {
    int redirect_count = 0;
    char* redirect_file = NULL;

    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            redirect_count++;
            if (args[i + 1] != NULL && args[i + 2] == NULL) {
                redirect_file = args[i + 1];
                args[i] = NULL;  // Nullify `>` and file name
            } else {
                print_error();  // Handle error case for bad redirection usage
                return NULL;
            }
        }
    }

    if (redirect_count > 1) {
        print_error();  // More than one redirection is an error
        return NULL;
    }

    return redirect_file;
}