#include <sqlite3.h>
#define MAX_INPUT_LENGTH 100

int is_new_user();
int store_hash(const char *stored_hash);
int retreive_hash_string(char *buf, size_t buflen);
int add_password();
int list_password();
int delete_password();
int modify_entry();
int database_init();
