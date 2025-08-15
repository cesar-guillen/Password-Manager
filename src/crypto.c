#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// taken from https://www.geeksforgeeks.org/dsa/random-password-generator-in-c/
// Function to randomly generates password
// of length N
void randomPasswordGeneration(int N, char *password) {
    int i = 0;
    int randomizer = 0;
    srand((unsigned int)(time(NULL)));

    char numbers[] = "0123456789";
    char letter[] = "abcdefghijklmnoqprstuvwyzx";
    char LETTER[] = "ABCDEFGHIJKLMNOQPRSTUYWVZX";
    char symbols[] = "!@#$^&*?";

    randomizer = rand() % 4;

    for (i = 0; i < N; i++) {
        if (randomizer == 1) {
            password[i] = numbers[rand() % 10];
        }
        else if (randomizer == 2) {
            password[i] = symbols[rand() % 8];
        }
        else if (randomizer == 3) {
            password[i] = LETTER[rand() % 26];
        }
        else {
            password[i] = letter[rand() % 26];
        }
        randomizer = rand() % 4;
    }

    password[N] = '\0'; 
}
