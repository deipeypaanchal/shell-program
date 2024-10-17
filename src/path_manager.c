#include "rush.h"

PathList *init_path() {
    PathList *plist = malloc(sizeof(PathList));
    plist->head = NULL;
    plist->length = 0;
    add_path(plist, "/bin");
    return plist;
}

void add_path(PathList *plist, char *path) {
    Node *node = malloc(sizeof(Node));
    node->path = strdup(path);
    node->next = NULL;

    if (!plist->head) {
        plist->head = node;
    } else {
        Node *current = plist->head;
        while (current->next) {
            current = current->next;
        }
        current->next = node;
    }
    plist->length++;
}

void clear_path(PathList *plist) {
    Node *current = plist->head;
    while (current) {
        Node *next = current->next;
        free(current->path);
        free(current);
        current = next;
    }
    plist->head = NULL;
    plist->length = 0;
}

void update_path(PathList *plist, Command *cmd) {
    clear_path(plist);
    for (int i = 1; i < cmd->arg_count; i++) {
        add_path(plist, cmd->args[i]);
    }
}