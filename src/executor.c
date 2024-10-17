#include "rush.h"

void execute_commands(Command **commands, int command_count, PathList *shell_path) {
    pid_t pids[MAX_COMMANDS];

    for (int i = 0; i < command_count; i++) {
        Command *cmd = commands[i];

        if (cmd->arg_count == 0) {
            continue;
        }

        // Handle built-in commands
        if (handle_builtin(cmd, shell_path)) {
            continue;
        }

        // Execute external commands
        char *executable = find_executable(cmd->args[0], shell_path);
        if (!executable) {
            print_error();
            continue;
        }

        pids[i] = fork();
        if (pids[i] == 0) {
            // Child process
            if (cmd->is_redirect) {
                int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd < 0) {
                    print_error();
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            execv(executable, cmd->args);
            print_error();
            exit(1);
        } else if (pids[i] < 0) {
            print_error();
        }
        free(executable);
    }

    // Wait for all child processes
    for (int i = 0; i < command_count; i++) {
        waitpid(pids[i], NULL, 0);
    }
}

int handle_builtin(Command *cmd, PathList *shell_path) {
    if (strcmp(cmd->args[0], "exit") == 0) {
        if (cmd->arg_count > 1) {
            print_error();
        } else {
            exit(0);
        }
        return 1;
    } else if (strcmp(cmd->args[0], "cd") == 0) {
        if (cmd->arg_count != 2) {
            print_error();
        } else if (chdir(cmd->args[1]) != 0) {
            print_error();
        }
        return 1;
    } else if (strcmp(cmd->args[0], "path") == 0) {
        update_path(shell_path, cmd);
        return 1;
    }
    return 0;
}

char *find_executable(char *command, PathList *shell_path) {
    Node *current = shell_path->head;
    while (current) {
        char full_path[MAX_PATH_LEN];
        snprintf(full_path, sizeof(full_path), "%s/%s", current->path, command);
        if (access(full_path, X_OK) == 0) {
            return strdup(full_path);
        }
        current = current->next;
    }
    return NULL;
}