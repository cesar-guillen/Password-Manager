#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include "ui.h"
#include "crypto.h"
#include "db.h"

static int check_status(sqlite3 * db, int conn){
    if (conn != SQLITE_OK )
    {
        fprintf(stderr, "Error: %s \n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    return 0;
}

static int service_exists(char *service){
    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_qry = "SELECT COUNT(*) FROM passwords WHERE service = ?";
    sqlite3_stmt *stmt;

    conn = sqlite3_prepare_v2(db, sql_qry, -1, &stmt, NULL);
    if (check_status(db, conn) == 1) return 1;

    sqlite3_bind_text(stmt, 1, service, -1, SQLITE_STATIC);

    int exits = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exits = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return exits;
}

int is_new_user() {
    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_qry = "SELECT COUNT(*) FROM master";
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

int retreive_hash_string(char *buf, size_t buflen){
    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_all = "SELECT master_password FROM master";
    sqlite3_stmt *stmt;
    conn = sqlite3_prepare_v2(db, sql_all, -1, &stmt, NULL);
    if (check_status(db, conn) == 1) return 1;
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *col = sqlite3_column_text(stmt, 0);
        strncpy(buf, (const char *)col, buflen - 1);
        buf[buflen - 1] = '\0';
    } else {
        printf("Master password not found\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int store_hash(const char *stored_hash){
    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_qry =
        "INSERT INTO master (master_password, last_changed) "
        "VALUES (?, DATETIME('now'))";
    sqlite3_stmt *stmt;

    conn = sqlite3_prepare_v2(db, sql_qry, -1, &stmt, NULL);
    if (check_status(db, conn) == 1) return 1;

    sqlite3_bind_text(stmt, 1, stored_hash, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert data: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Master password saved successfully!\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int add_password(){
    char service[MAX_INPUT_LENGTH];
    char username[MAX_INPUT_LENGTH];
    char password[MAX_INPUT_LENGTH];

    do
    {
        printf("(Required) Service name: ");
        read_line(service, sizeof(service));
    } while (strlen(service) == 0);

    
    printf("Username: ");
    read_line(username, sizeof(username));

    printf("Password (Leave blank to autogenerate): ");
    read_line(password, sizeof(password));
    if (strlen(password) == 0)
    {
        randomPasswordGeneration(AUTOGEN_PASS_LEN, password);
        printf("Auto generating password: %s\n", password);
    }
    
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

    if (!service_exists(service) && strlen(service) > 0)
    {
        printf("No password found for service '%s'.\n", service);
        return 0;
    }

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

    printf("\n%-18s %-18s %-28s %-20s\n", "Service", "Username", "Password", "Last Changed (UTC)");
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

int delete_password() {
    char service[MAX_INPUT_LENGTH];
    printf("Enter the service name of the password you would like to delete\n");
    printf("Service: ");
    read_line(service, sizeof(service));
    if (!service_exists(service))
    {
        printf("No password found for service '%s'.\n", service);
        return 0;
    }
    
    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_qry = "DELETE FROM passwords WHERE service = ?;";
    sqlite3_stmt *stmt;

    conn = sqlite3_prepare_v2(db, sql_qry, -1, &stmt, NULL);
    if (check_status(db, conn) == 1) return 1;

    sqlite3_bind_text(stmt, 1, service, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) fprintf(stderr, "Failed to delete data: %s\n", sqlite3_errmsg(db));
    if (sqlite3_changes(db) > 0) printf("Password deleted successfully!\n");


    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

static int edit_password(char *service){
    char new_password[MAX_INPUT_LENGTH];
    printf("Enter new password (Leave blank to autogenerate): ");
    read_line(new_password, sizeof(new_password));
    if (strlen(new_password) == 0)
    {
        randomPasswordGeneration(AUTOGEN_PASS_LEN, new_password);
        printf("Auto generating password: %s\n", new_password);
    }

    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_qry = "UPDATE passwords SET password = ?, last_changed = DATETIME('now') WHERE service = ?;";
    sqlite3_stmt *stmt;

    conn = sqlite3_prepare_v2(db, sql_qry, -1, &stmt, NULL);
    if (check_status(db, conn) == 1) return 1;
    sqlite3_bind_text(stmt, 1, new_password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, service, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) fprintf(stderr, "Failed to update data: %s\n", sqlite3_errmsg(db));
    if (sqlite3_changes(db) > 0) printf("Password modified successfully!\n");


    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

static int edit_service(char *service){
    char new_service[MAX_INPUT_LENGTH];

    do
    {
        printf("Enter new service name: ");
        read_line(new_service, sizeof(new_service));
    } while (strlen(new_service) == 0);

    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_qry = "UPDATE passwords SET service = ?, last_changed = DATETIME('now') WHERE service = ?;";
    sqlite3_stmt *stmt;

    conn = sqlite3_prepare_v2(db, sql_qry, -1, &stmt, NULL);
    if (check_status(db, conn) == 1) return 1;
    sqlite3_bind_text(stmt, 1, new_service, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, service, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) fprintf(stderr, "Failed to update data: %s\n", sqlite3_errmsg(db));
    if (sqlite3_changes(db) > 0) printf("Service modified successfully!\n");


    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

static int edit_username(char *service){
    char new_username[MAX_INPUT_LENGTH];
    printf("Enter new username: ");
    read_line(new_username, sizeof(new_username));

    sqlite3 *db;
    int conn = sqlite3_open("data/database.db", &db);
    if (check_status(db, conn) == 1) return 1;

    const char *sql_qry = "UPDATE passwords SET username = ?, last_changed = DATETIME('now') WHERE service = ?;";
    sqlite3_stmt *stmt;

    conn = sqlite3_prepare_v2(db, sql_qry, -1, &stmt, NULL);
    if (check_status(db, conn) == 1) return 1;
    sqlite3_bind_text(stmt, 1, new_username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, service, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) fprintf(stderr, "Failed to update data: %s\n", sqlite3_errmsg(db));
    if (sqlite3_changes(db) > 0) printf("username modified successfully!\n");


    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

static int edit_all(char *service){
    edit_username(service);
    edit_password(service);
    edit_service(service);
    return 0;
}

int modify_entry(){
    char service[MAX_INPUT_LENGTH];
    char option[2];
    printf("Enter the service name for the entry you would like to modify: ");
    read_line(service, sizeof(service));
    if (!service_exists(service))
    {
        printf("No password found for service '%s'.\n", service);
        return 0;
    }
    printf("Enter which parameter you would like to edit:\n"
            "[A] All     [S] Service     [U] Username     [P] Password\n"
            "Option: "
    );
    read_line(option, sizeof(option));
    if (strncmp(option,"A",strlen(option)) == 0) return edit_all(service);
    else if (strncmp(option,"S",strlen(option)) == 0) return edit_service(service);
    else if (strncmp(option,"U",strlen(option)) == 0) return edit_username(service);
    else if (strncmp(option,"P",strlen(option)) == 0) return edit_password(service);
    else printf("Option is not recognized\n");
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
    sql_qry = "CREATE TABLE IF NOT EXISTS master"
        "(id INTEGER PRIMARY KEY, master_password TEXT, last_changed TEXT);"; 
    conn = sqlite3_exec(db, sql_qry, 0, 0, &err_msg);
    if (check_status(db, conn) == 1) return 1;

    sqlite3_close(db);
    return 0;
}

