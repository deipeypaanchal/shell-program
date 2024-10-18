// src/utils.h

#ifndef UTILS_H
#define UTILS_H

#include "rush.h"

/**
 * @brief Prints a predefined error message to stderr.
 */
void print_error(void);

/**
 * @brief Removes leading and trailing whitespace from the input string.
 * 
 * This function modifies the string in place.
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

#endif // UTILS_H

