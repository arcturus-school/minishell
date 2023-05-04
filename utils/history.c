#include "include/utils.h"

FILE* fp;

int old_cmd_num = 0;  // easy to write new instructions quickly
int cmd_num     = 0;  // number of current commond

typedef struct cmdLink {
    char* cmd;
    int id;
    struct cmdLink* next;
    struct cmdLink* pre;
} cmdLink;

cmdLink *start = NULL, *end = NULL;

void init_history() {
    fp = open_history("a+");

    char *line = NULL, *p = NULL;

    while (!feof(fp)) {
        line = (char*)malloc(sizeof(char) * 1024);

        p = fgets(line, 1024, fp);

        if (p) {
            add_cmd(line);
            old_cmd_num++;
        }
    }
}

void add_cmd(char* cmd) {
    cmdLink* n = (cmdLink*)malloc(sizeof(cmdLink));

    n->cmd  = cmd;
    n->pre  = end;
    n->next = NULL;
    n->id   = cmd_num++;

    if (end != NULL) {
        end->next = n;
    }

    if (start == NULL) {
        start = n;
    }

    end = n;
}

void write_to_history(int close) {
    cmdLink* link = start;

    for (int i = 0; i < old_cmd_num; i++) {
        link = link->next;
    }

    while (link) {
        fprintf(fp, "%s", link->cmd);
        link = link->next;
    }

    fflush(fp);  // write into history immediately

    if (close) {
        fclose(fp);
    }
}

void show_history(int num) {
    if (num < 0) {
        cmdLink* link = start;

        while (link) {
            fprintf(stdout, "%d %s", link->id, link->cmd);
            link = link->next;
        }

    } else {
        int n = num < cmd_num ? num : cmd_num;

        cmdLink* link = end;

        for (int i = 0; i < n - 1; i++) {
            link = link->pre;
        }

        for (int i = 0; i < n; i++) {
            fprintf(stdout, "%d %s", link->id, link->cmd);
            link = link->next;
        }
    }
}

// clear history file content
void clear_file() {
    if (fp != NULL) {
        fclose(fp);
    }

    fp = open_history("w");
    fclose(fp);

    open_history("a+");
}

void clear_history() {
    cmdLink *cur = start, *next = NULL;

    while (cur) {
        next = cur->next;
        free(cur->cmd);
        free(cur);
        cur = next;
    }

    old_cmd_num = 0;
    cmd_num     = 0;
    start       = NULL;
    end         = NULL;

    clear_file();
}

FILE* open_history(char* mode) {
    FILE* fp = NULL;

    if ((fp = fopen(".history", mode)) == NULL) {
        show_error(true, "open file \"history\" failed.");
    }

    return fp;
}