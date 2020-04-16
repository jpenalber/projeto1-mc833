#include "dbinterface.h"

sqlite3 *db;

int open_db(char *path) {
    
    int rc = sqlite3_open(path, &db);

    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return -1;
    }

}

int insertSala(char *tipo) {
    char *sql = "INSERT INTO Salas (Tipo) VALUES( ? );";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepere statment: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_bind_text(stmt, 1, tipo, strlen(tipo), 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind data: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    char *err_msg = 0;
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE ) {      
        fprintf(stderr, "insert statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(db));              
        return -1;
    }


    sqlite3_int64 id = sqlite3_last_insert_rowid(db);
    printf("sala ID: %lld", id);

    sqlite3_finalize(stmt);

    return (int) id;
}

int insertFilmeStruct(s_filme filme) {
    insertFilme(filme.nome, filme.descricao);
}

int insertFilme(char *nome, char *descricao) {
    char *sql = "INSERT INTO Filmes (Nome, Descricao) VALUES( ? , ? );";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepere statment: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_bind_text(stmt, 1, nome, strlen(nome), 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind data: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_bind_text(stmt, 2, descricao, strlen(descricao), 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind data: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    char *err_msg = 0;
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE ) {      
        fprintf(stderr, "insert statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(db));              
        return -1;
    }


    sqlite3_int64 id = sqlite3_last_insert_rowid(db);
    printf("Filme ID: %lld", id);

    sqlite3_finalize(stmt);

    return (int) id;
}