#include "include/utils.h"

void prompt_msg() {
    char* uname = get_user_name();
    char* kname = get_kernel_name();
    char* cwd   = get_cwd();
    char* home  = get_cur_home_dir();

    if (startswith(cwd, home)) {
        char* t = strrepl(cwd, home, "~", 0);
        cwd = t;
    }

    printf(TIP, uname, kname, cwd);
    printf(" ");
}

void welcome_to_shell() {
    printf(WELCOME);
}

void show_error(int quit, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, ERROR, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, ", because %s.\n", strerror(errno));
    va_end(args);

    if (quit) {
        // immediately exit
        exit(EXIT_FAILURE);
    }
}
