// src/utils.c

#define _POSIX_C_SOURCE 200809L // Required for access() function
#include "rush.h"               // Include the rush header file
#include <stdio.h>              // Standard input/output definitions
#include <string.h>             // String handling functions
#include <sys/stat.h>           // File information
#include <unistd.h>             // Standard symbolic constants and types
#include <ctype.h>              // Character handling functions

/**
 * @brief Predefined error message.
 */
char error_message[30] = "An error has occurred\n";

/**
 * @brief Prints the predefined error message to stderr.
 */
void print_error(void) {
    write(STDERR_FILENO, error_message, strlen(error_message));
}

/**
 * @brief Removes leading and trailing whitespace from the input string.
 * 
 * This function modifies the string in place.
 * 
 * @param str The string to be trimmed.
 */

// Function to remove leading and trailing whitespace from a string
void trim_whitespace(char *str) {
    char *start = str;

    // Trim leading whitespace
    while (isspace((unsigned char)*start)) start++;

    // If all spaces or empty string
    if (*start == 0) {
        // Set the string to empty
        *str = 0;
        // Exit the function
        return;
    }

    // Trim trailing whitespace
    char *end = start + strlen(start) - 1;
    // Move the end pointer back to the last non-space character
    while (end > start && isspace((unsigned char)*end)) {
        // Set the character to null
        *end = '\0';
        // Move the pointer back
        end--;
    }

    // Shift the trimmed string back to the original buffer
    if (start != str) {
        // Move the string back to the start
        memmove(str, start, strlen(start) + 1);
    }
}

/**
 * @brief Splits the input line into arguments based on whitespace.
 * 
 * The arguments are stored in the provided args array, which should be
 * pre-allocated with at least MAX_ARGS elements.
 * 
 * @param line The input line to parse.
 * @param args The array to store parsed arguments.
 */
// Function to parse the input line into arguments
void parse_input(char *line, char **args) {
    // Initialize the argument index
    int i = 0;
    // Tokenize the input line based on whitespace
    char *token = strtok(line, " \t\n");
    // Loop through each token
    while (token != NULL && i < MAX_ARGS - 1) {
        // Store the token in the argument list
        args[i++] = token;
        // Move to the next token
        token = strtok(NULL, " \t\n");
    }
    // Null-terminate the argument list
    args[i] = NULL;
}

