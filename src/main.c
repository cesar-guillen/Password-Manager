#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ui.h"
#include "db.h"
#include "crypto.h"
#define MIN_ENTROPY 55

int create_master_password(){
    char master_pass[MAX_INPUT_LENGTH];
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

    char hex_salt[HEX_SALT_LEN];
    char hex_hash[HEX_HASH_LEN];
    to_hex(salt, SALT_LEN, hex_salt);
    to_hex(hash, HASH_LEN, hex_hash);

    return store_hash(hex_salt, hex_hash);
}

int verify_master_passwd(){
    char master_pass[MAX_INPUT_LENGTH];
    printf("Please enter your master password to continue:\n");
    read_line(master_pass, sizeof(master_pass));

    char hex_salt[HEX_SALT_LEN];
    char hex_hash[HEX_HASH_LEN];
    if (retreive_salt(hex_salt, HEX_SALT_LEN) == 1) return 1;
    if (retreive_hash(hex_hash, HEX_HASH_LEN) == 1) return 1;

    unsigned char salt[SALT_LEN];
    if (from_hex(hex_salt, salt, SALT_LEN) < 0){
        printf("Error unhexing salt\n");
        return 1;
    }

    unsigned char hash_current[HASH_LEN];
    if (generate_hash(salt, hash_current, master_pass) == 1) return 1;

    char hex_hash_current[HEX_HASH_LEN];
    to_hex(hash_current, HASH_LEN, hex_hash_current);
    if (strncmp(hex_hash, hex_hash_current, strlen(hex_hash)) == 0)
    {
        printf("Master password is valid!\n");
        return 0;
    }
    else
    {
        printf("Master password is not valid\n");
        return 1;
    }
}

int determine_option(char *option){
    size_t option_len = strlen(option);
    if((((strncmp(option, "-h", option_len) == 0) || (strncmp(option, "--help", option_len) == 0)))) {
        print_help();
        return 0;
    }
    if(verify_master_passwd() == 1) return 1;
    if(strncmp(option, "-a", option_len) == 0) return add_password();
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

    if (is_new_user()){
        if(create_master_password() == 1) return 1;
    } 
    if(argc > 1){
       return determine_option(argv[1]);
    }
    return 0;
}