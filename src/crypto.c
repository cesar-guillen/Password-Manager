#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include "crypto.h"
char numbers[] = "0123456789";
char letter[] = "abcdefghijklmnoqprstuvwyzx";
char LETTER[] = "ABCDEFGHIJKLMNOQPRSTUYWVZX";
char symbols[] = "!@#$^&*?";

#define NUM_SET_SIZE strlen(numbers)
#define L_LETTERS_SET_SIZE strlen(letter)
#define U_LETTERS_SET_SIZE strlen(LETTER)
#define SYM_SET_SIZE strlen(symbols)


// taken from https://www.geeksforgeeks.org/dsa/random-password-generator-in-c/
// Function to randomly generates password
// of length N
void randomPasswordGeneration(int N, char *password) {
    int i = 0;
    int randomizer = 0;
    srand((unsigned int)(time(NULL)));

    randomizer = rand() % 4;

    for (i = 0; i < N; i++) {
        if (randomizer == 1) {
            password[i] = numbers[rand() % NUM_SET_SIZE];
        }
        else if (randomizer == 2) {
            password[i] = symbols[rand() % SYM_SET_SIZE];
        }
        else if (randomizer == 3) {
            password[i] = LETTER[rand() % U_LETTERS_SET_SIZE];
        }
        else {
            password[i] = letter[rand() % L_LETTERS_SET_SIZE];
        }
        randomizer = rand() % 4;
    }

    password[N] = '\0'; 
}

double check_password_entropy(const char *password) {
    int has_lower = 0, has_upper = 0, has_digit = 0, has_symbol = 0;
    int charset_size = 0;
    int length = strlen(password);

    // Check what character sets are present
    for (int i = 0; i < length; i++) {
        if (islower(password[i])) has_lower = 1;
        else if (isupper(password[i])) has_upper = 1;
        else if (isdigit(password[i])) has_digit = 1;
        else has_symbol = 1;  // everything else treated as symbol
    }

    if (has_lower) charset_size += L_LETTERS_SET_SIZE;
    if (has_upper) charset_size += U_LETTERS_SET_SIZE;
    if (has_digit) charset_size += NUM_SET_SIZE;
    if (has_symbol) charset_size += SYM_SET_SIZE; 

    if (charset_size == 0) return 0.0;

    // Entropy = length * log2(charset_size)
    return length * (log((double)charset_size) / log(2.0));
}

void to_hex(const unsigned char *in, size_t len, char *out) {
    for (size_t i = 0; i < len; i++) {
        sprintf(out + (i * 2), "%02x", in[i]);
    }
    out[len * 2] = '\0';
}

int generate_salt(unsigned char *salt){
    if (RAND_bytes(salt, sizeof(salt)) != 1) {
        fprintf(stderr, "Error generating random salt\n");
        return 1;
    }
    return 0;
}

int generate_hash(unsigned char *salt, unsigned char *hash, const char *master_pass){

    if (!PKCS5_PBKDF2_HMAC(master_pass, strlen(master_pass),
                           salt, SALT_LEN,
                           100000, // iterations
                           EVP_sha256(),
                           HASH_LEN, hash)) {
        fprintf(stderr, "Error in PBKDF2\n");
        return 1;
    }
    return 0;
}