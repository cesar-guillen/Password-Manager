#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "db.h"
#define MAX_PASSW_LEN 30
#define HELP 0
#define ADD 1
#define LIST 2
#define DELETE 3
#define MODIFY 4


int create_master_password(){
    printf("no passowrds\n");
    return 0;
}

int determine_option(char *option){
    if((((strncmp(option, "-h", 3) == 0) || (strncmp(option, "--help", 7) == 0)))) {
        print_help();
        return 0;
    }
    else if(strncmp(option, "-a", 3) == 0) {
        print_help();
        return 1;
    }
    else if(strncmp(option, "-l", 3) == 0) {
        print_help();
        return 2;
    }
    else if(strncmp(option, "-d", 3) == 0) {
        print_help();
        return 3;
    }
    else if(strncmp(option, "-m", 3) == 0) {
        print_help();
        return 4;
    }
    else
    {
        printf("%s is not recognized as a command\n", option);
        return -1;
    }
}


int main(int argc, char *argv[]){
    print_art();
    if (database_init() != 0) return 1;
    if(argc == 1) {
        print_help();
        return 0;
    }

    if (is_new_user()) create_master_password();
    if(argc > 1){
        switch (determine_option(argv[1]))
        {
        case HELP:
            print_help();
            break;
        case ADD:
            break;
        case LIST:
            break;
        case DELETE:
            break;
        case MODIFY:
            break;
        default:
            print_help();
            break;
        }
    }
    return 0;
}