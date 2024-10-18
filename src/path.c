// src/path.c

#define _POSIX_C_SOURCE 200809L     // Required for access() function
#include "path.h"                   // Include the path header file
#include <stdlib.h>                 // Standard library
#include <stdio.h>                  // Standard input/output definitions
#include <string.h>                 // String handling functions
#include <sys/stat.h>               // File information
#include <unistd.h>                 // Symbolic constants and types

/**
 * @brief Changes the current working directory of the shell.
 * 
 * Validates that exactly one argument is provided. If the number of
 * arguments is incorrect or chdir fails, prints an error.
 * 
 * @param args The array of arguments for the cd command.
 */

// Function to change the current working directory of the shell
void change_directory(char **args) {
    // Check for the correct number of arguments
    if (args[1] == NULL || args[2] != NULL) {
        // Print an error message
        print_error();
        // Return control to the user
        return;
    }
    // Change the directory
    if (chdir(args[1]) != 0) {
        // Print an error message
        print_error();
    }
}

/**
 * @brief Sets the search paths for the shell.
 * 
 * Frees existing paths and duplicates new paths from the arguments.
 * If memory allocation fails, prints an error.
 * 
 * @param args The array of arguments for the path command.
 */

// Function to set the search paths for the shell
void set_path(char **args) {
    // Free existing paths
    for (int i = 0; i < MAX_PATHS; i++) {
        // Check if the path is not NULL
        if (paths[i]) {
            // Free the path
            free(paths[i]);
            // Set the path to NULL
            paths[i] = NULL;
        }
    }

    // Duplicate new paths from the arguments
    // Loop through each argument
    for (int i = 1; args[i] != NULL && i < MAX_PATHS; i++) {
        // Duplicate the path
        // Check if memory allocation failed
        paths[i - 1] = strdup(args[i]);
        // Check if memory allocation failed
        if (paths[i - 1] == NULL) {
            // Print an error message
            print_error();
            // Return control to the user
            break;
        }
    }
}

