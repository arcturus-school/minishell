#include "include/utils.h"

extern char** environ;

char* get_env(const char* key) {
    char* val = getenv(key);

    if (NULL == val) {
        fprintf(stderr, "cannot find the environment variable \"%s\".\n", key);
    }

    return val;
}

int set_env(const char* key, const char* value, int overwrite) {
    int res = setenv(key, value, overwrite);

    if (-1 == res) {
        fprintf(stderr, "set enviroment variable \"%s\" failed.\n", key);
    }

    return res;
}

int put_env(const char* key, const char* value) {
    char *val = getenv(key), *e = NULL;

    if (NULL == val) {
        // environment variable is not exist
        return set_env(key, value, 0);
    }

    e = strjoin(val, value, ":");

    int flag = set_env(key, e, 1);

    return flag;
}

int unset_env(const char* key) {
    int res = unsetenv(key);

    if (res == -1) {
        fprintf(stderr, "unset enviroment variable \"%s\" failed.\n", key);
    }

    return res;
}

void show_all_env() {
    int i = 0;

    while (environ[i]) {
        printf("%s\n", environ[i++]);
    }
}
