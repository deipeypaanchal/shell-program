#include <stdio.h>   // For snprintf()
#include <unistd.h>  // For access() and X_OK
#include <stdlib.h>
#include <string.h>
#include "path.h"

char* search_paths[MAX_PATHS] = { "/bin", NULL };

void init_path() {
    search_paths[0] = strdup("/bin");
    search_paths[1] = NULL;
}

void update_path(char **args) {
    for (int i = 0; search_paths[i] != NULL; i++) {
        free(search_paths[i]);
    }
    int count = 0;
    for (int i = 1; args[i] != NULL; i++) {
        search_paths[count++] = strdup(args[i]);
    }
    search_paths[count] = NULL;
}

char* find_command_path(char *cmd) {
    for (int i = 0; search_paths[i] != NULL; i++) {
        char full_path[MAX_CMD_LEN];
        snprintf(full_path, sizeof(full_path), "%s/%s", search_paths[i], cmd);  // Use snprintf
        
        if (access(full_path, X_OK) == 0) {  // Use access() to check for executable
            return strdup(full_path);
        }
    }
    return NULL;
}