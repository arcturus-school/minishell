#include "include/utils.h"

void set_redirect(Direct dir, char* path) {
    if (path != NULL) {
        if (dir == R_RIGHT || dir == R_DRIGHT) {  // > or >>
            int flag;

            if (dir == R_RIGHT) {
                flag = O_WRONLY | O_TRUNC | O_CREAT;
            } else {
                flag = O_WRONLY | O_APPEND | O_CREAT;
            }

            int wport = open(path, flag, 0644);
            dup2(wport, STDOUT_FILENO);
            close(wport);
        } else if (dir == R_LEFT) {  // <
            int rport = open(path, O_RDONLY);
            dup2(rport, STDIN_FILENO);
            close(rport);
        }
    }
}