// src/commands.h

#ifndef COMMANDS_H
#define COMMANDS_H

#include "rush.h"

/**
 * @brief Executes a command by searching through the defined paths.
 * 
 * If the command is not found in any of the paths, an error is printed
 * and the process exits with status 1.
 * 
 * @param args The array of arguments for the command.
 */
void exec_command(char **args);

/**
 * @brief Handles the execution of parallel commands separated by '&'.
 * 
 * Each command is executed in a separate child process. The parent
 * process waits for all child processes to complete before returning.
 * 
 * @param line The input line containing parallel commands.
 */
void run_parallel_commands(char *line);

#endif // COMMANDS_H

