#include <sqlite3.h>

int is_new_user();
int store_hash(const char *salt,const char *hash);
int add_password();
int list_password();
int delete_password();
int modify_entry();
int database_init();
