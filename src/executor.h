#ifndef EXECUTOR_H
#define EXECUTOR_H

void execute_commands(char **commands);
void handle_cd(char **args);
void handle_execution(char **args);
char* check_redirection(char **args);

#endif