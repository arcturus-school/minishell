#ifndef __UTILS_H__
#define __UTILS_H__

// @Author : ARCTURUS <ice99125@qq.com>
// @Created : 2022-12-21 16:31:03

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>

#define WELCOME                                                          \
    "+++++++++++++++++++++++++++++++++++++++++++++++++\n"                \
    "+                                               +\n"                \
    "+            \033[35mwelcome to mini-shell.\033[0m             +\n" \
    "+        ctrl+c or input \"exit\" to quit.        +\n"              \
    "+                                               +\n"                \
    "+++++++++++++++++++++++++++++++++++++++++++++++++\n"

#define USERINFO "\033[31m%s@$%s:\033[0m"

#define DIRNAME "\033[34m%s$\033[0m"

#define TIP USERINFO DIRNAME

#define ERROR "\033[31m%s\033[0m"  // red

#define WARNING "\033[35m%s\033[0m"  // purple

#define SUCCESS "\033[36m%s\033[0m"  // green

#define true 1
#define false 0

// env.c

char* get_env(const char* key);

void show_all_env();

int unset_env(const char* key);

// set enviroment variable by additional
int put_env(const char* key, const char* value);

int set_env(const char* key, const char* value, int overwrite);

// printf.c

void show_error(int quit, char* fmt, ...);

void welcome_to_shell();

void prompt_msg();

// sys.c

// obtain current work directory.
char* get_cwd();

// obtain user's home directory
char* get_pwnam(const char* name);

char* get_kernel_name();

struct passwd* get_user_record();

char* get_user_name();

char* get_cur_home_dir();

typedef enum Direct {
    LEFT,
    RIGHT,
    BOTH,
} Direct;

// redirect.c

typedef enum {
    R_LEFT,    // <
    R_RIGHT,   // >
    R_DRIGHT,  // >>
} Redirect;

void set_redirect(Direct dir, char* path);

// str.c

// whether string is equal
int strequ(const char* src, const char* dst);

// whether string is a number
int isDigit(char* str);

// determines whether the src begins with dest
// if src does not begin with dest then return 0, otherwise, return 1
int startswith(const char* src, const char* dest);

int endswith(const char* src, const char* dest);

// @param all: whether replace all string in str
char* strrepl(const char* str, const char* search, const char* replace_with, int all);

char** strsplit(const char* source, const char* separator, int recycle);

// @param direct: 0: left, 1: right, 2: both
char* strtrim(const char* str, const char* flag, Direct direct, int recycle);

// concatenate string
char* strjoin(const char* src, const char* sub, const char* join_with);

// history.c

// write command into .history
void write_to_history(int close);

void add_cmd(char* cmd);

void init_history();

void clear_history();

FILE* open_history(char* mode);

void show_history(int num);

// signal.c

void interrupt_handler(int signum);

// shell variable
typedef struct var {
    char* key;
    char* value;
    int is_env;
    struct var* next;
} var;

// shellvar.c

void init_shell();

void add_var(char* key, char* value, int is_env);

void show_export();

var* search_var(char* key);

// alias.c

void add_alias(char* alias, char* real, char** args);

void init_alias();

void alias_handler(char*** c);

// free.c

void add_recycle(void* v);  // collect variables to be recycled

void free_recycle();

// daemon.c

void init_daemon();

#endif