#include <sqlite3.h>
#define MAX_INPUT_LENGTH 100

int is_new_user();
int store_hash(const char *stored_hash);
int retreive_hash_string(char *buf, size_t buflen);
int add_password(char* master_pass);
int list_password(char* master_pass);
int delete_password(char* master_pass);
int modify_entry(char* master_pass);
int passwords_db_init(char* master_pass);
int master_pass_db_init();
int sqlite3_key(sqlite3 *db, const void *key, int keylen);

