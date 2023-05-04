#include "include/minishell.h"

Command* newCommand(char* input) {
    char** t = strsplit(input, " \t", true);  // split by \t or space

    alias_handler(&t);

    Command* c = (Command*)malloc(sizeof(Command));
    add_recycle(c);

    c->cmd  = t[0];
    c->args = t;
    c->raw  = input;
    c->next = NULL;
    c->path = NULL;
    c->p    = NULL;
    c->back = false;

    args_handler(c);

    return c;
}

char* builtin[] = { "export", "history", "echo", "env", NULL };

int is_builtin(char* cmd) {
    int i = 0;

    while (builtin[i]) {
        if (strequ(cmd, builtin[i++])) {
            return true;
        }
    }

    return false;
}

void exec_builtin(Command* c) {
    if (strequ(c->cmd, "export")) {
        cmd_export(c->args + 1);
    } else if (strequ(c->cmd, "history")) {
        cmd_history(c->args + 1);
    } else if (strequ(c->cmd, "echo")) {
        cmd_echo(c->args + 1);
    } else if (strequ(c->cmd, "env")) {
        cmd_env(c->args + 1);
    }

    exit(EXIT_SUCCESS);
}

void run_command(Command* c) {
    if (c->p == NULL) {
        if (c->back) {
            init_daemon();
        }

        set_redirect(c->redirect, c->path);

        if (is_builtin(c->cmd)) {
            exec_builtin(c);
        } else if (-1 == execvp(c->cmd, c->args)) {
            fprintf(stderr, "command execution failed.\n");
        }
    } else {
        int pipe_fd[2], status;

        if (pipe(pipe_fd) < 0) {
            show_error(true, "create pipe failed");
        }

        pid_t pid = fork();

        if (pid < 0) {
            show_error(true, "create a new process failed");
        } else if (0 == pid) {
            // child process
            set_redirect(c->redirect, c->path);

            // pipe
            close(pipe_fd[0]);
            if (-1 == dup2(pipe_fd[1], STDOUT_FILENO)) {
                show_error(false, "dup2 failed");
            }

            if (is_builtin(c->cmd)) {
                exec_builtin(c);
            } else if (-1 == execvp(c->cmd, c->args)) {
                fprintf(stderr, "command execution failed.\n");
            }
        } else {
            wait(&status);

            close(pipe_fd[1]);
            if (-1 == dup2(pipe_fd[0], STDIN_FILENO)) {
                show_error(false, "dup2 failed");
            }

            run_command(c->p);
        }
    }
}

void exec_command(Command* c) {
    if (strequ(c->cmd, "exit")) {
        if (!c->p) {
            // execute only no pipe
            cmd_exit();
        } else {
            c = c->p;
        }
    } else if (strequ(c->cmd, "cd")) {
        if (c->p) {
            // similar to "cd .. | xxx" will skip cd
            c = c->p;
        } else {
            cmd_cd(c->args + 1);
            return;
        }
    } else if (strequ(c->cmd, "history")) {
        // child process cannot mmodify parent process variable
        // so we can only modify without fork()
        if (c->args[1] != NULL && strequ(c->args[1], "-c")) {
            cmd_history(c->args + 1);
            return;
        }
    } else if (strequ(c->cmd, "export")) {
        if (c->args[1] != NULL && !strequ(c->args[1], "-p")) {
            cmd_export(c->args + 1);
            return;
        }
    } else if (strequ(c->cmd, "env")) {
        if (c->args[1] != NULL) {
            cmd_env(c->args + 1);
            return;
        }
    }

    pid_t pid = fork();

    if (pid < 0) {
        show_error(true, "create a new process failed");
    } else if (0 == pid) {
        // child process
        run_command(c);
    } else {
        // parent process
        wait(NULL);
    }
}

Command* reverse_link(Command* n) {
    Command *pre = NULL, *cur = n, *next = NULL;

    while (cur) {
        next      = cur->next;
        cur->next = pre;
        pre       = cur;
        cur       = next;
    }

    return pre;
}

Command* reverse_pipe(Command* n) {
    Command *pre = NULL, *cur = n, *next = NULL;

    while (cur) {
        next   = cur->p;
        cur->p = pre;
        pre    = cur;
        cur    = next;
    }

    return pre;
}

// parse user input into an Command array
Command* parse(char* input) {
    char** cmds = strsplit(input, ";\r\n", true);  // split by ';'
    char **temp = NULL, **t = NULL;

    Command *res = NULL, *pipe = NULL;

    for (int i = 0; NULL != cmds[i]; i++) {
        temp = strsplit(cmds[i], "|", true);

        for (int j = 0; NULL != temp[j]; j++) {
            if (strstr(temp[j], " ") == NULL && strstr(temp[j], "=") != NULL) {
                // NAME=123
                t = strsplit(temp[j], "=", false);
                add_var(t[0], t[1], false);

                if (pipe) {
                    pipe       = reverse_pipe(pipe);
                    pipe->next = res;
                    res        = pipe;
                    pipe       = NULL;
                }
            } else {
                Command* c = newCommand(temp[j]);

                c->p = pipe;
                pipe = c;
            }
        }

        if (pipe) {
            pipe       = reverse_pipe(pipe);
            pipe->next = res;
            res        = pipe;
            pipe       = NULL;
        }
    }

    return reverse_link(res);
}

int args_handler(Command* c) {
    for (int i = 0; NULL != c->args[i]; i++) {
        if (strequ(c->args[i], "&")) {
            c->back    = true;
            c->args[i] = NULL;
            break;
        } else if (strequ(c->args[i], ">>") || strequ(c->args[i], ">") || strequ(c->args[i], "<")) {
            // only support "xxx >> yy" not support "xxx >> yy >> zz"
            if (NULL == c->args[i + 1]) {
                fprintf(stderr, "syntax error after %s.\n", c->args[i]);
                break;
            }

            if (strequ(c->args[i], ">>")) {
                c->redirect = R_DRIGHT;
            } else if (strequ(c->args[i], ">")) {
                c->redirect = R_RIGHT;
            } else {
                c->redirect = R_LEFT;
            }

            c->path    = c->args[i + 1];
            c->args[i] = NULL;
        }
    }
}