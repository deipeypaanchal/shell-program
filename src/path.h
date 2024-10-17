#ifndef PATH_H
#define PATH_H

#define MAX_PATHS 10
#define MAX_CMD_LEN 256

void init_path();
void update_path(char **args);
char* find_command_path(char *cmd);

#endif