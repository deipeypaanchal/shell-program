// src/path.h

#ifndef PATH_H
#define PATH_H
#include "rush.h"

/**
 * @brief Changes the current working directory of the shell.
 * 
 * Validates the number of arguments and attempts to change the directory.
 * If unsuccessful, prints an error.
 * 
 * @param args The array of arguments for the cd command.
 */
void change_directory(char **args);

/**
 * @brief Sets the search paths for the shell.
 * 
 * Frees existing paths and duplicates new paths from the arguments.
 * If memory allocation fails, prints an error.
 * 
 * @param args The array of arguments for the path command.
 */
void set_path(char **args);

#endif // PATH_H

