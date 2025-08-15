#include <stdio.h>
#include <string.h>
void print_art(){
    printf(
            " ___                                  _   __ __                               \n"
            "| . \\ ___  ___ ___ _ _ _  ___  _ _  _| | |  \\  \\ ___ ._ _  ___  ___  ___  _ _ \n"
            "|  _/<_> |<_-<<_-<| | | |/ . \\| '_>/ . | |     |<_> || ' |<_> |/ . |/ ._>| '_>\n"
            "|_|  <___|/__//__/|__/_/ \\___/|_|  \\___| |_|_|_|<___||_|_|<___|\\_. |\\___.|_|  \n"
            "                                                               <___'          \n"
    );
    printf("Welcome to your interactive local password manager!\n\n");
}

void print_help(){
    printf(
            "Usage: ./passmgr [options]\n\n"
            "-h, --help          Display this help menu and exit\n"
            "-a                  Add a new password\n"
            "-l                  List a password\n"
            "-d                  Remove a password\n"
            "-m                  Edit a password\n"
            "\n"
    );
}

void read_line(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            // No newline → there are leftover chars in stdin → flush them
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}