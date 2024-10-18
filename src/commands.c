// src/commands.c

#define _POSIX_C_SOURCE 200809L         // Required for access() function
#include "commands.h"                   // Include the header file
#include "redirection.h"                // Include the redirection header file
#include <stdlib.h>                     // Standard library  
#include <unistd.h>                     // Standard symbolic constants and types
#include <sys/wait.h>                   // Wait for process to change state
#include <stdio.h>                      // Standard input/output definitions
#include <string.h>                     // String handling functions
#include <sys/stat.h>                   // File information

/**
 * @brief Executes a command by searching through the defined paths.
 * 
 * Iterates through each path in the paths array, constructs the full
 * path for the command, and checks if it's executable. If found,
 * executes the command using execv. If the command is not found
 * in any path, prints an error and exits.
 * 
 * @param args The array of arguments for the command.
 * @return void
 * 
 */

// Function to execute a command by searching through the defined paths
void exec_command(char **args) {
    // Buffer to store the full path of the command
    char full_path[256];
    // Iterate through each path to find the executable
    for (int i = 0; paths[i] != NULL; i++) {
        // Construct the full path for the command
        snprintf(full_path, sizeof(full_path), "%s/%s", paths[i], args[0]);
        // Check if the command is executable
        if (access(full_path, X_OK) == 0) {
            // Execute the command
            execv(full_path, args);
            // If execv fails, continue to the next path
        }
    }
    print_error();  // If the command cannot be found in any path
    exit(1);
}

/**
 * @brief Handles the execution of parallel commands separated by '&'.
 * 
 * Splits the input line into individual commands, forks a child
 * process for each command, and executes them in parallel. The
 * parent process waits for all child processes to finish before
 * returning control to the user.
 * 
 * @param line The input line containing parallel commands.
 */

// Function to handle the execution of parallel commands
void run_parallel_commands(char *line) {
    // Array to store individual commands
    char *commands[MAX_ARGS];
    // Number of commands
    int num_cmds = 0;

    // Duplicate the line to avoid modifying the original
    char *line_copy = strdup(line);
    // Check if the duplication failed
    if (line_copy == NULL) {
        // Print an error message
        print_error();
        // Return control to the user
        return;
    }

    // Split the line by '&' to get individual commands
    char *token = strtok(line_copy, "&");
    // Loop through each command
    while (token != NULL && num_cmds < MAX_ARGS - 1) {
        // Trim whitespace
        trim_whitespace(token);
        // Skip empty commands
        if (strlen(token) > 0) {
            commands[num_cmds++] = token;
        }
        // Move to the next command
        token = strtok(NULL, "&");
    }

    // Null-terminate the commands array
    commands[num_cmds] = NULL;

    // Array to store child process IDs
    pid_t pids[num_cmds];
    // Loop through each command
    for (int i = 0; i < num_cmds; i++) {
        // Fork a child process for each command
        if ((pids[i] = fork()) == 0) {
            // Child process
            char *cmd_args[MAX_ARGS];
            // Parse the command into arguments
            parse_input(commands[i], cmd_args);
            // Handle redirection
            if (handle_redirection(cmd_args) == 0) {
                // Execute the command
                exec_command(cmd_args);
            }
            // If exec_command fails, exit child
            exit(1);
            
            // If fork fails
        } else if (pids[i] < 0) {
            // Print an error message
            print_error();
            // Free the duplicated line
            free(line_copy);
            // Return control to the user
            return;
        }
    }

    // Free the duplicated line
    free(line_copy);
    // Parent process waits for all child processes
    for (int i = 0; i < num_cmds; i++) {
        if (pids[i] > 0) {
            // Wait for child process to change state
            waitpid(pids[i], NULL, 0);
        }
    }
}

