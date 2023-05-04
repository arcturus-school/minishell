#include "include/minishell.h"

void change_dir(char* path) {
    char* cwd = get_cwd();
    char* p   = path;

    if (chdir(p) != 0) {
        printf("cd: %s: ", p);

        if (access(p, F_OK) == -1) {
            printf("no such file or directory.\n");
        } else if (access(p, R_OK) == -1) {
            printf("permission denied.\n");
        }
    } else {
        // be useful to "cd -"
        set_env("OLDPWD", cwd, 1);
    }
}

void cmd_cd(char** args) {
    if (!args[0]) {
        char* path = get_cur_home_dir();

        // return to home directory
        change_dir(path);
    } else {
        if (args[1] != NULL) {
            printf("cd: too many arguments.\n");
        } else {
            if (strequ(args[0], "~") || strequ(args[0], "--")) {
                // cd ~ or cd --
                char* path = get_cur_home_dir();

                // return to home directory
                change_dir(path);
            } else if (strequ(args[0], "-")) {
                // cd -
                char* old = get_env("OLDPWD");

                if (old != NULL) {
                    change_dir(old);
                }
            } else {
                change_dir(args[0]);
            }
        }
    }
}