#include "include/minishell.h"

void run() {
    init_shell();
    init_history();
    init_alias();

    welcome_to_shell();

    signal(SIGINT, interrupt_handler);

    char* input;  // user input

    while (true) {
        prompt_msg();

        size_t buffer_size = 0;
        getline(&input, &buffer_size, stdin);

        add_cmd(input);

        Command* list = parse(input);

        int i = 0;
        while (list) {
            exec_command(list);
            list = list->next;
        }

        free_recycle();
    }
}
