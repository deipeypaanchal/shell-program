// src/main.c

/**
 * @file main.c
 * @brief Implementation of the main loop for the rush shell.
 * 
 * This file contains the main function, which handles user input,
 * parses commands, manages built-in commands, and coordinates the
 * execution of external commands, including parallel execution and
 * output redirection.
 */

#define _POSIX_C_SOURCE 200809L // Required for getline()
#include "rush.h"               // Include the rush header file
#include "utils.h"              // Include the utility functions
#include "commands.h"           // Include the command functions
#include "path.h"               // Include the path functions
#include "redirection.h"        // Include the redirection functions
#include <stdlib.h>             // Standard library
#include <unistd.h>             // Standard symbolic constants and types
#include <sys/wait.h>           // Wait for process to change state
#include <stdio.h>              // Standard input/output definitions
#include <string.h>             // String handling functions
#include <sys/stat.h>           // File information

// Global array to store search paths
char *paths[MAX_PATHS] = {NULL};

// Function to execute parallel commands
int main(int argc, char *argv[]) {
    // Suppress unused parameter warning
    (void)argv;

    // Check for invalid arguments
    if (argc > 1) {
        // Print an error message
        print_error();
        // Exit the shell
        exit(1);
    }

    // Initialize the search paths
    paths[0] = strdup("/bin");
    // Check if memory allocation failed
    if (paths[0] == NULL) {
        // Print an error message
        print_error();
        // Exit the shell
        exit(1);
    }
    // Initialize the rest of the paths to NULL
    for (int i = 1; i < MAX_PATHS; i++) {
        // Set the path to NULL
        paths[i] = NULL;
    }

    // Variables for reading input
    char *line = NULL;
    // Initialize the length of the input line
    size_t len = 0;

    while (1) {
        // Print the prompt
        printf("rush> ");
        // Flush the output buffer
        fflush(stdout);

        // Read input from the user
        ssize_t nread = getline(&line, &len, stdin);
        // Check if reading failed
        if (nread == -1) {
            // Free allocated memory before exiting
            free(line);
            // Free all allocated paths before exiting
            for (int i = 0; i < MAX_PATHS; i++) {
                // Check if the path is not NULL
                if (paths[i]) free(paths[i]);
            }
            // Exit the shell
            exit(0);
        }

        // Trim leading and trailing whitespace
        trim_whitespace(line);

        // Skip empty commands
        if (strlen(line) == 0) {
            // Skip further processing in main loop
            continue;
        }

        // Check if the command contains '&' for parallel execution
        if (strchr(line, '&') != NULL) {
            // Execute parallel commands
            run_parallel_commands(line);
            // Skip further processing in main loop
            continue; 
        }

        // Parse command and arguments
        char *args[MAX_ARGS];
        // Parse the input line
        parse_input(line, args);

        // Handle built-in commands
        if (args[0] == NULL) {
            // Skip further processing in main loop
            continue;
            // Check for built-in commands
        } else if (strcmp(args[0], "exit") == 0) {
            // Check for additional arguments
            if (args[1] != NULL) {
                // Print an error message
                print_error();
                // Skip further processing in main loop
                continue;
            } else {
                // Free allocated memory before exiting
                free(line);
                // Free all allocated paths before exiting
                for (int i = 0; i < MAX_PATHS; i++) {
                    // Check if the path is not NULL
                    if (paths[i]) free(paths[i]);
                }
                // Exit the shell
                exit(0);
            }
            // Skip further processing in main loop
        } else if (strcmp(args[0], "cd") == 0) {
            // Change directory
            change_directory(args);
            // Skip further processing in main loop
        } else if (strcmp(args[0], "path") == 0) {
            // Set the search paths
            set_path(args);
        } else {
            // External command
            pid_t pid = fork();
            // Check if fork was successful
            if (pid == 0) {
                // Child process
                if (handle_redirection(args) == 0) {
                    // Execute the command
                    exec_command(args);
                }
                // If exec_command fails, exit child
                exit(1);
                // If fork fails
            } else if (pid > 0) {
                // Parent process
                waitpid(pid, NULL, 0);
            } else {
                // Fork failed
                print_error();
            }
        }
    }

    // Free allocated memory before exiting
    free(line);
    // Free all allocated paths before exiting
    for (int i = 0; i < MAX_PATHS; i++) {
        // Check if the path is not NULL
        if (paths[i]) free(paths[i]);
    }
    // Exit the shell
    return 0;
}

