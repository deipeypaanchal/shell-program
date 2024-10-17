#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"
#include "path_manager.h"

void execute_commands(CommandList *cmd_list, PathManager *path_manager);
int handle_builtin(Command *cmd, PathManager *path_manager);
char *find_executable(char *cmd_name, PathManager *path_manager);

#endif