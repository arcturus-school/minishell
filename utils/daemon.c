#include "include/utils.h"

void init_daemon() {
    pid_t pid;

    if ((pid = fork()) < 0) {
        show_error(true, "create a new process failed");
    } else if (pid != 0) {
        exit(EXIT_SUCCESS);
    }

    printf("pid=%d\n", getpid());

    setsid();

    for (int i = 0; i < NOFILE; close(i++))
        ;

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    umask(0);

    signal(SIGCHLD, SIG_IGN);
}