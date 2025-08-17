#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "ui.h"
#include "db.h"
#include "crypto.h"

int create_master_password(){
    char master_pass[MAX_INPUT_LENGTH];
    double pass_entropy = 0;
    printf("First time user detected. Please enter your master password:\n");
    read_line(master_pass, MAX_INPUT_LENGTH);
    pass_entropy = check_password_entropy(master_pass);

    while (pass_entropy < MIN_ENTROPY)
    {
        printf("Password is too weak. Try to increase the length or the complexity of the password:\n");
        read_line(master_pass, sizeof(master_pass));
        pass_entropy = check_password_entropy(master_pass);
    } 
    
    unsigned char salt[SALT_LEN];
    unsigned char hash[HASH_LEN];
    if (generate_salt(salt) == 1) return 1;
    if (generate_hash(salt, hash, master_pass, PBKDF2_ITERATIONS) == 1) return 1;

    wipe_mem(master_pass, sizeof(master_pass));

    char hex_salt[HEX_SALT_LEN];
    char hex_hash[HEX_HASH_LEN];
    to_hex(salt, SALT_LEN, hex_salt);
    to_hex(hash, HASH_LEN, hex_hash);

    char stored_hash[1024];
    snprintf(stored_hash, sizeof(stored_hash),
             "pbkdf2_sha512$%d$%s$%s",
             PBKDF2_ITERATIONS, hex_salt, hex_hash);

    return store_hash(stored_hash);
}

int verify_master_passwd(char *master_pass){
    printf("Please enter your master password to continue:\n");
    read_line(master_pass, MAX_INPUT_LENGTH);

    char stored_hash[1024];
    if (retreive_hash_string(stored_hash, sizeof(stored_hash)) == 1) return 1;

    // Expected format: pbkdf2_sha512$iterations$salt$hash
    char *algo = strtok(stored_hash, "$");
    char *iter_str = strtok(NULL, "$");
    char *hex_salt = strtok(NULL, "$");
    char *hex_hash = strtok(NULL, "$");

    if (!algo || !iter_str || !hex_salt || !hex_hash) {
        fprintf(stderr, "Stored hash format invalid\n");
        return 1;
    }

    size_t iterations = (size_t)atoi(iter_str);

    unsigned char salt[SALT_LEN];
    if (from_hex(hex_salt, salt, SALT_LEN) < 0) {
        printf("Error decoding salt\n");
        return 1;
    }

    unsigned char expected_hash[HASH_LEN];
    if (from_hex(hex_hash, expected_hash, HASH_LEN) < 0) {
        printf("Error decoding stored hash\n");
        return 1;
    }

    unsigned char candidate_hash[HASH_LEN];
    if (generate_hash(salt, candidate_hash, master_pass, iterations) == 1) return 1;

    if (memcmp(expected_hash, candidate_hash, HASH_LEN) == 0) {
        printf("Master password is valid!\n");
        return 0;
    } else {
        usleep(300 * 1000); // slow down brute force
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
    char master_pass[MAX_INPUT_LENGTH];
    int password_not_verified = 1;
    while (password_not_verified)
    {
        password_not_verified = verify_master_passwd(master_pass);
    }
    if (passwords_db_init(master_pass) != 0) return 1;
    
    if(strncmp(option, "-a", option_len) == 0) return add_password(master_pass);
    else if(strncmp(option, "-l", option_len) == 0) return list_password(master_pass);
    else if(strncmp(option, "-d", option_len) == 0) return delete_password(master_pass);
    else if(strncmp(option, "-m", option_len) == 0) return modify_entry(master_pass);
    else
    {
        printf("%s is not recognized as a command\n", option);
        print_help();
        return 0;
    }
}


int main(int argc, char *argv[]){
    print_art();
    if (master_pass_db_init() != 0) return 1;
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