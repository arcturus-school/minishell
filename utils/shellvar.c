#include "include/utils.h"

extern char** environ;

var* node = NULL;

void init_shell() {
    int i  = 0;
    var* p = NULL;

    while (environ[i]) {
        char** t = strsplit(environ[i], "=", false);
        add_var(t[0], t[1], true);
        i++;
    }
}

void add_var(char* key, char* value, int is_env) {
    var* p = (var*)malloc(sizeof(var));

    p->is_env = is_env;
    p->next   = node;
    p->key    = key;
    p->value  = value;
    node      = p;
}

void show_export() {
    var* p      = node;
    char* value = NULL;

    while (p) {
        if (p->is_env) {
            value = p->value == NULL ? "" : p->value;
            printf("declare -x %s=\"%s\"\n", p->key, value);
        }

        p = p->next;
    }
}

var* search_var(char* key) {
    var* p = node;

    while (p) {
        if (strequ(key, p->key)) {
            return p;
        }

        p = p->next;
    }

    return NULL;
}