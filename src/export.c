#include "include/minishell.h"

void cmd_export(char** args) {
    // export or export -p
    if (!args[0] || strequ(args[0], "-p")) {
        show_export();
    } else {
        // export NAME=arcturus DIR=/home
        int i = 0;

        while (args[i]) {
            char** t = strsplit(args[i], "=", false);

            var* v = search_var(t[0]);

            char* value = NULL;

            if (v != NULL) {
                value = t[1] == NULL ? v->value : t[1];

                v->value  = value;
                v->is_env = true;
            } else {
                value = t[1] == NULL ? "" : t[1];
                add_var(t[0], value, true);
            }

            set_env(t[0], value, true);  // export shell variable to environment variable

            i++;
        }
    }
}