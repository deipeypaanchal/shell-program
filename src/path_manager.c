#include <stdlib.h>
#include <string.h>
#include "path_manager.h"

void init_path_manager(PathManager *pm) {
    pm->paths = NULL;
    pm->count = 0;
}

void add_path(PathManager *pm, const char *new_path) {
    pm->paths = realloc(pm->paths, sizeof(char *) * (pm->count + 1));
    pm->paths[pm->count] = strdup(new_path);
    pm->count++;
}

void reset_path_manager(PathManager *pm) {
    for (int i = 0; i < pm->count; i++) {
        free(pm->paths[i]);
    }
    free(pm->paths);
    pm->paths = NULL;
    pm->count = 0;
}

void free_path_manager(PathManager *pm) {
    reset_path_manager(pm);
}