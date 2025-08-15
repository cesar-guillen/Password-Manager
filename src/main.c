#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "db.h"



int create_master_password(){
    printf("no passwords\n");
    return 0;
}

int determine_option(char *option){
    size_t option_len = strlen(option);
    if((((strncmp(option, "-h", option_len) == 0) || (strncmp(option, "--help", option_len) == 0)))) {
        print_help();
        return 0;
    }
    else if(strncmp(option, "-a", option_len) == 0) return add_password();
    else if(strncmp(option, "-l", option_len) == 0) return list_password();
    else if(strncmp(option, "-d", option_len) == 0) return delete_password();
    else if(strncmp(option, "-m", option_len) == 0) return modify_password();
    else
    {
        printf("%s is not recognized as a command\n", option);
        print_help();
        return 0;
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
       return determine_option(argv[1]);
    }
    return 0;
}