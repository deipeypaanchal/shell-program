// src/redirection.c

#define _POSIX_C_SOURCE 200809L // Required for access() function
#include "redirection.h"        // Include the redirection header file
#include <stdio.h>              // Standard input/output definitions
#include <string.h>             // String handling functions
#include <sys/stat.h>           // File information
#include <fcntl.h>              // File control options
#include <unistd.h>             // Symbolic constants and types

/**
 * @brief Handles output redirection for a command.
 * 
 * Detects the '>' operator, ensures correct syntax, redirects stdout
 * to the specified file, and modifies the args array accordingly.
 * 
 * @param args The array of arguments for the command.
 * @return int Returns 0 on success, -1 on error.
 */

// Function to handle output redirection for a command
int handle_redirection(char **args) {
    // Count the number of redirection operators and record the last index
    int redirect_count = 0;
    // Initialize the index to -1
    int redirect_index = -1;

    // First pass: Count redirection operators and record the last index
    for (int i = 0; args[i] != NULL; i++) {
        // Check for the redirection operator '>'
        if (strcmp(args[i], ">") == 0) {
            // Increment the count
            redirect_count++;
            // Record the index of the last redirection operator
            redirect_index = i;
        }
    }

    // Check for multiple redirection operators
    if (redirect_count > 1) {
        // Print an error message
        print_error();
        // Return an error
        return -1;
    }

    // Handle single redirection
    if (redirect_count == 1) {
        // Ensure there is a filename after '>'
        if (args[redirect_index + 1] == NULL) {
            // Print an error message
            print_error();
            // Return an error
            return -1;
        }

        // Ensure no additional arguments after the filename
        for (int i = redirect_index + 2; args[i] != NULL; i++) {
            // Print an error message
            print_error();
            // Return an error
            return -1;
        }

        // Open the file for writing (create if it doesn't exist, truncate if it does)
        int fd = open(args[redirect_index + 1], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
        // Check if the file could not be opened
        if (fd == -1) {
            // Print an error message
            print_error();
            // Return an error
            return -1;
        }

        // Redirect stdout to the file
        if (dup2(fd, STDOUT_FILENO) == -1) {
            // Print an error message
            print_error();
            // Close the file descriptor
            close(fd);
            // Return an error
            return -1;
        }
        // Close the file descriptor
        close(fd);

        // Terminate the args array at the redirection operator
        // Set the redirection operator to NULL
        args[redirect_index] = NULL;
    }

    // Return success
    return 0;
}

