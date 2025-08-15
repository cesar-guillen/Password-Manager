#include <stdio.h>
#include <string.h>
#define MAX_PASSW_LEN 30

void print_art(){
    printf(
            " ___                                  _   __ __                               \n"
            "| . \\ ___  ___ ___ _ _ _  ___  _ _  _| | |  \\  \\ ___ ._ _  ___  ___  ___  _ _ \n"
            "|  _/<_> |<_-<<_-<| | | |/ . \\| '_>/ . | |     |<_> || ' |<_> |/ . |/ ._>| '_>\n"
            "|_|  <___|/__//__/|__/_/ \\___/|_|  \\___| |_|_|_|<___||_|_|<___|\\_. |\\___.|_|  \n"
            "                                                               <___'          \n"
    );
}

void print_help(){
    printf("Welcome to your interactive local password manager!\n\n");
    printf(
            "Usage: ./passmgr [options]\n\n"
            "-h, --help          Display this help menu and exit\n"
            "-a, --add           Add a new password\n"
            "-l  <name>          List a password, if left blank all passwrds will be displayed\n"
            "-d  <name>          Remove a password\n"
            "-m  <name>          Edit a password\n"
    );
}

int main(int argc, char *argv[]){
    print_art();
    if(argc == 1 || (argc > 1 && ((strncmp(argv[1], "-h", 3) == 0) || (strncmp(argv[1], "--help", 7) == 0)))) {
        print_help();
        return 0;
    }
}