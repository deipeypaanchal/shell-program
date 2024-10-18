// src/redirection.h

#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "rush.h"

/**
 * @brief Handles output redirection for a command.
 * 
 * Detects the '>' operator, ensures correct syntax, redirects stdout
 * to the specified file, and modifies the args array accordingly.
 * 
 * @param args The array of arguments for the command.
 * @return int Returns 0 on success, -1 on error.
 */
int handle_redirection(char **args);

#endif // REDIRECTION_H

