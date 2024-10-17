#ifndef PATH_MANAGER_H
#define PATH_MANAGER_H

typedef struct {
    char **paths;
    int count;
} PathManager;

void init_path_manager(PathManager *pm);
void add_path(PathManager *pm, const char *new_path);
void reset_path_manager(PathManager *pm);
void free_path_manager(PathManager *pm);

#endif
