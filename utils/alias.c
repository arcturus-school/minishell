#include "include/utils.h"

typedef struct Alias {
    char* alias;  // alias of command
    char* real;   // actual command
    char** args;  // default arguments
    struct Alias* next;
} Alias;

Alias* ap = NULL;

void add_alias(char* alias, char* real, char** args) {
    Alias* a = (Alias*)malloc(sizeof(Alias));

    a->alias = alias;
    a->real  = real;
    a->args  = args;
    a->next  = ap;

    ap = a;
}

Alias* search_alias(char* cmd) {
    Alias* n = ap;

    while (n) {
        if (strequ(cmd, n->alias)) {
            return n;
        }

        n = n->next;
    }

    return NULL;
}

int max_default_args_len = 3;

char* ll[]   = { "-alF", "--color=auto", NULL };
char* l[]    = { "-CF", "--color=auto", NULL };
char* la[]   = { "-A", "--color=auto", NULL };
char* ls[]   = { "--color=auto", NULL };
char* mv[]   = { "-i", NULL };
char* rm[]   = { "-i", NULL };
char* grep[] = { "--color=auto", NULL };
char* mk[]   = { "-pv", NULL };

void init_alias() {
    add_alias("ll", "ls", ll);
    add_alias("l", "ls", l);
    add_alias("la", "ls", la);
    add_alias("ls", "ls", ls);
    add_alias("mv", "mv", mv);
    add_alias("rm", "rm", rm);
    add_alias("grep", "grep", grep);
    add_alias("mkdir", "mkdir", mk);
}

// array length exclude NULL
int array_len(char** c) {
    int i = 0;

    while (c[i]) {
        i++;
    }

    return i;
}

void alias_handler(char*** c) {
    Alias* a = search_alias((*c)[0]);

    if (NULL != a) {
        int len = array_len(*c) + max_default_args_len;

        char** t = (char**)malloc(sizeof(char*) * len);
        add_recycle(t);

        t[0] = a->real;

        int i = 0;
        while (a->args[i]) {
            t[i + 1] = a->args[i];
            i++;
        }

        int j = i + 1;

        i = 1;

        while ((*c)[i]) {
            t[j++] = (*c)[i++];
        }

        t[j] = NULL;

        (*c) = t;
    }
}