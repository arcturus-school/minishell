#include "include/minishell.h"

void cmd_echo(char** args) {
    int n = 1 /* newline */, options = 0;

    for (int i = 0; args[i] != NULL; i++) {
        if (strequ(args[i], "-n")) {
            n = 0;  // don't wrap
            options++;
        } else {
            break;
        }
    }

    int i     = options;
    char *msg = NULL, *c = NULL, *new = NULL;

    while (args[i] != NULL) {
        // echo "xxx" will become echo xxx
        c = strtrim(args[i], "\"", BOTH, true);

        char* p = strstr(c, "$");

        if (p != NULL) {
            var* v = search_var(p + 1);

            char* t;

            if (v != NULL) {
                t = (char*)malloc(sizeof(char) * (p - c + strlen(v->value) + 1));
                strncpy(t, c, p - c);
                t[p - c] = '\0';
                strcat(t, v->value);
            } else {
                // no variable
                t = (char*)malloc(sizeof(char) * (p - c + 1));
                strncpy(t, c, p - c);
                t[p - c] = '\0';
            }

            add_recycle(t);

            new = strjoin(msg, t, " ");

        } else {
            new = strjoin(msg, c, " ");
        }

        msg = new;
        i++;
    }

    if (msg != NULL) {
        if (n) {
            // echo "xxx"
            printf("%s\n", msg);
        } else {
            // echo -n "xxx" or echo "xxx"
            printf("%s", msg);
        }
    }
}