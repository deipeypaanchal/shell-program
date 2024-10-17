#include <stdlib.h>
#include <string.h>
#include "path.h"
#include <Kernel/sys/unistd.h>

char* search_paths[MAX_PATHS] = { "/bin", NULL };

void init_path() {
    search_paths[0] = strdup("/bin");
    search_paths[1] = NULL;
}

void update_path(char **args) {
    // Free the old paths
    for (int i = 0; search_paths[i] != NULL; i++) {
        free(search_paths[i]);
    }

    // Add the new paths
    int count = 0;
    for (int i = 1; args[i] != NULL; i++) {
        search_paths[count++] = strdup(args[i]);
    }
    search_paths[count] = NULL;  // Null-terminate the path list
}

char* find_command_path(char *cmd) {
    for (int i = 0; search_paths[i] != NULL; i++) {
        char full_path[MAX_CMD_LEN];
        snprintf(full_path, sizeof(full_path), "%s/%s", search_paths[i], cmd);  // Construct full path

        if (access(full_path, X_OK) == 0) {  // Check if the file is executable
            return strdup(full_path);
        }
    }
    return NULL;  // Return NULL if no valid path is found
}