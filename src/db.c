#include <stdio.h>
#include <sqlite3.h>
#include "ui.h"
#include "string.h"
#define MAX_INPUT_LENGTH 100

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
    return !count;
}

int add_password(){
    char service[MAX_INPUT_LENGTH];
    char username[MAX_INPUT_LENGTH];
    char password[MAX_INPUT_LENGTH];

    printf("Service name: ");
    read_line(service, sizeof(service));

    printf("Username: ");
    read_line(username, sizeof(username));

    printf("Password: ");
    read_line(password, sizeof(password));

    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_qry ="INSERT INTO passwords (service, username, password, last_changed) "
    "VALUES (?, ?, ?, DATETIME('now'))";
    sqlite3_stmt *stmt;

    conn = sqlite3_prepare_v2(db, sql_qry, -1, &stmt, NULL);
    if (check_status(db, conn) == 1) return 1;

    sqlite3_bind_text(stmt, 1, service, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert data: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Password saved successfully!\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int list_password() {
    char service[MAX_INPUT_LENGTH];
    printf("Enter the service name of the password (Leave blank to retrieve all)\n");
    printf("Service: ");
    read_line(service, sizeof(service));

    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_all = "SELECT service, username, password, last_changed FROM passwords";
    const char *sql_one = "SELECT service, username, password, last_changed FROM passwords WHERE service = ?";

    sqlite3_stmt *stmt;

    if (strlen(service) == 0) {
        conn = sqlite3_prepare_v2(db, sql_all, -1, &stmt, NULL);
    } else {
        conn = sqlite3_prepare_v2(db, sql_one, -1, &stmt, NULL);
        if (conn == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, service, -1, SQLITE_STATIC);
        }
    }
    if (check_status(db, conn) == 1) return 1;

    printf("\n%-18s %-18s %-28s %-20s\n", "Service", "Username", "Password", "Last Changed");
    printf("---------------------------------------------------------------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *svc = sqlite3_column_text(stmt, 0);
        const unsigned char *usr = sqlite3_column_text(stmt, 1);
        const unsigned char *pwd = sqlite3_column_text(stmt, 2);
        const unsigned char *changed = sqlite3_column_text(stmt, 3);

        printf("%-18s %-18s %-28s %-20s\n",
               svc ? (const char *)svc : "",
               usr ? (const char *)usr : "",
               pwd ? (const char *)pwd : "",
               changed ? (const char *)changed : "");
        printf("---------------------------------------------------------------------------------------\n");
        
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int delete_password(){
    char service[MAX_INPUT_LENGTH];
    printf("Enter the service name of the password you would like to delete\n");
    read_line(service, sizeof(service));

    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_qry ="DELETE FROM passwords WHERE service = ?;"
    "VALUES (?)";
    sqlite3_stmt *stmt;

    conn = sqlite3_prepare_v2(db, sql_qry, -1, &stmt, NULL);
    if (check_status(db, conn) == 1) return 1;
    sqlite3_bind_text(stmt, 1, service, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to delete data: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Password deleted successfully!\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;

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
        "(id INTEGER PRIMARY KEY, service TEXT, username TEXT, password TEXT, last_changed TEXT);"; 
    conn = sqlite3_exec(db, sql_qry, 0, 0, &err_msg);
    if (check_status(db, conn) == 1) return 1;

    sqlite3_close(db);
    return 0;
}

