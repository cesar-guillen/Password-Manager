#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ui.h"
#include "db.h"
#include "crypto.h"
#define MIN_ENTROPY 55

int create_master_password(){
    char master_pass[AUTOGEN_PASS_LEN];
    double pass_entropy = 0;
    printf("First time user detected. Please enter your master password:\n");
    read_line(master_pass, sizeof(master_pass));
    pass_entropy = check_password_entropy(master_pass);

    while (pass_entropy < MIN_ENTROPY)
    {
        printf("Password is too weak. Try to increase the lenght or the complexity of the password:\n");
        read_line(master_pass, sizeof(master_pass));
        pass_entropy = check_password_entropy(master_pass);
    } 
    
    unsigned char salt[SALT_LEN];
    unsigned char hash[HASH_LEN];
    if (generate_salt(salt) == 1) return 1;
    if (generate_hash(salt, hash, master_pass) == 1) return 1;
    
    char hex_salt[SALT_LEN * 2 + 1];
    char hex_hash[HASH_LEN * 2 + 1];
    to_hex(salt, SALT_LEN, hex_salt);
    to_hex(hash, HASH_LEN, hex_hash);

    store_hash(hex_salt, hex_hash);;
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
    else if(strncmp(option, "-m", option_len) == 0) return modify_entry();
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