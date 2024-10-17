#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "executor.h"
#include "utils.h"
#include "path_manager.h"

void execute_commands(CommandList *cmd_list, PathManager *path_manager) {
    pid_t *pids = malloc(sizeof(pid_t) * cmd_list->count);

    for (int i = 0; i < cmd_list->count; i++) {
        Command *cmd = &cmd_list->commands[i];

        if (cmd->argc == 0) {
            continue;
        }

        // Handle built-in commands
        if (handle_builtin(cmd, path_manager)) {
            continue;
        }

        // Validate redirection
        if (cmd->output_redirect == -1 || (cmd->output_redirect && cmd->output_file == NULL)) {
            print_error();
            continue;
        }

        // Find executable in path
        char *exec_path = find_executable(cmd->argv[0], path_manager);
        if (exec_path == NULL) {
            print_error();
            continue;
        }

        // Fork and execute
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            if (cmd->output_redirect) {
                int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd < 0) {
                    print_error();
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            execv(exec_path, cmd->argv);
            // If execv returns, an error occurred
            print_error();
            exit(1);
        } else if (pid < 0) {
            // Fork failed
            print_error();
        } else {
            // Parent process
            pids[i] = pid;
        }
        free(exec_path);
    }

    // Wait for all child processes
    for (int i = 0; i < cmd_list->count; i++) {
        if (pids[i] > 0) {
            waitpid(pids[i], NULL, 0);
        }
    }
    free(pids);
}

int handle_builtin(Command *cmd, PathManager *path_manager) {
    if (strcmp(cmd->argv[0], "exit") == 0) {
        if (cmd->argc != 1) {
            print_error();
            return 1;
        }
        exit(0);
    } else if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argc != 2) {
            print_error();
            return 1;
        }
        if (chdir(cmd->argv[1]) != 0) {
            print_error();
        }
        return 1;
    } else if (strcmp(cmd->argv[0], "path") == 0) {
        reset_path_manager(path_manager);
        for (int i = 1; i < cmd->argc; i++) {
            add_path(path_manager, cmd->argv[i]);
        }
        return 1;
    }
    return 0;
}

char *find_executable(char *cmd_name, PathManager *path_manager) {
    for (int i = 0; i < path_manager->count; i++) {
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", path_manager->paths[i], cmd_name);
        if (access(full_path, X_OK) == 0) {
            return strdup(full_path);
        }
    }
    return NULL;
}