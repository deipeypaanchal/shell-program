// src/rush.h

#ifndef RUSH_H
#define RUSH_H

#include <stddef.h>

// Maximum number of arguments per command
#define MAX_ARGS 10

// Maximum number of paths in the search path
#define MAX_PATHS 10

// Maximum size for input lines
#define MAX_INPUT_SIZE 256

// Global array to store search paths
extern char *paths[MAX_PATHS];

// Error message to be displayed on stderr
extern char error_message[30];

/**
 * @brief Prints a predefined error message to stderr.
 */
void print_error(void);

/**
 * @brief Trims leading and trailing whitespace from a string.
 * 
 * @param str The string to be trimmed.
 */
void trim_whitespace(char *str);

/**
 * @brief Splits the input line into arguments based on whitespace.
 * 
 * The arguments are stored in the provided args array, which should be
 * pre-allocated with at least MAX_ARGS elements.
 * 
 * @param line The input line to parse.
 * @param args The array to store parsed arguments.
 */
void parse_input(char *line, char **args);

#endif // RUSH_H

