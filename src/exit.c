#include "include/minishell.h"

// kills the shell gracefully
void cmd_exit() {
    write_to_history(true);
    exit(EXIT_SUCCESS);
}