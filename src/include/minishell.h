#ifndef __MINISHELL_H__
#define __MINISHELL_H__

#include "../../utils/include/utils.h"

typedef struct Command {
    char* cmd;
    char** args;
    int back;           // &
    char* raw;          // originated input
    Redirect redirect;  // redirect method
    char* path;         // redirect path
    struct Command* next;
    struct Command* p;  // pipe
} Command;

// minishell.c

void run();

// cd.c

void cmd_cd(char** args);

// exit.c

void cmd_exit();

// env.c

void cmd_env(char** args);

// echo.c

void cmd_echo(char** args);

// exec.c
Command* parse(char* input);

Command* newCommand(char* input);

void exec_command(Command* c);

int args_handler(Command* c);

// history.c

void cmd_history(char** args);

// export.c

void cmd_export(char** args);

#endif