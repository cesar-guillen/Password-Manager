#include <stdio.h>
#include <sqlite3.h>

static int check_status(sqlite3 * db, int conn){
    if (conn != SQLITE_OK )
    {
        fprintf(stderr, "Error: %s \n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    return 0;
}

int is_new_user() {
    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_qry = "SELECT COUNT(*) FROM passwords";
    sqlite3_stmt *stmt;

    conn = sqlite3_prepare_v2(db, sql_qry, -1, &stmt, NULL);
    if (check_status(db, conn) == 1) return 1;

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    if(count) return 1;
    return 0;
}

int add_password(){
    return 0;
}

int list_password(){
    return 0;
}

int delete_password(){
    return 0;
}

int modify_password(){
    return 0;
}

int database_init(){
    sqlite3 *db;
    char *err_msg = 0;

    int conn;
    conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;
    
    //table creation
    char *sql_qry = "CREATE TABLE IF NOT EXISTS passwords"
        "(id INTEGER PRIMARY KEY, service TEXT, username TEXT, password TEXT, url TEXT, last_changed TEXT);"; 
    conn = sqlite3_exec(db, sql_qry, 0, 0, &err_msg);
    if (check_status(db, conn) == 1) return 1;

    sqlite3_close(db);
    return 0;
}

