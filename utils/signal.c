#include "include/utils.h"

void interrupt_handler(int signum) {
    write_to_history(true);
    exit(EXIT_SUCCESS);
}