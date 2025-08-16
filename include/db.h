#include <sqlite3.h>
#define MAX_INPUT_LENGTH 100

int is_new_user();
int store_hash(const char *salt,const char *hash);
int retreive_hash(char* hex_hash, size_t buflen);
int retreive_salt(char* hex_salt, size_t buflen);
int add_password();
int list_password();
int delete_password();
int modify_entry();
int database_init();
