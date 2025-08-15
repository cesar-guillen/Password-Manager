#include <stdio.h>
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
            "-l  <name>          List a password, if left blank all passwords will be displayed\n"
            "-d  <name>          Remove a password\n"
            "-m  <name>          Edit a password\n"
            "\n"
    );
}