#include <dbinterface.h>
#include <common.h>

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
    fprintf(stderr, "sala ID: %lld", id);

    sqlite3_finalize(stmt);

    return (int) id;
}

int insertFilmeStruct(s_filme filme) {
    insertFilme(filme.nome, filme.genero, filme.descricao);
}

int insertFilme(char *nome, char *genero, char *descricao) {
    char *sql = "INSERT INTO Filmes (Nome, Genero, Descricao) VALUES( ?, ?, ?);";
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

    rc = sqlite3_bind_text(stmt, 2, genero, strlen(genero), 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind data: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_bind_text(stmt, 3, descricao, strlen(descricao), 0);
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
    fprintf(stderr, "Filme ID: %lld\n", id);

    sqlite3_finalize(stmt);

    return (int) id;
}

int insertExibicao(int sala_id, int filme_id) {
    char *sql = "INSERT INTO Exibicao (sala_id, filme_id) VALUES( ? , ? );";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepere statment: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_bind_int(stmt, 1, sala_id);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind data: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_bind_int(stmt, 2, filme_id);
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
    fprintf(stderr, "Exibição ID: %lld\n", id);

    sqlite3_finalize(stmt);

    return (int) id;
}

int getFilmeIDByNome(char *nome) {
    char *sql = "SELECT filme_id FROM Filmes WHERE nome = ? LIMIT 1;";
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

    sqlite3_int64 id;
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        id = sqlite3_column_int64(stmt, 0);
        fprintf(stderr, "Filme ID: %lld\n", id);
    } else {
        fprintf(stderr, "Filme not found!\n");
        id = -1;
    }

    sqlite3_finalize(stmt);

    return (int) id;
}

char *cpytext(char *text) {
    char *new_text = calloc(strlen(text)+1, sizeof(char));
    strcpy(new_text, text);
    return new_text;
}

int getFilmeByNome(char *nome, s_filme *filme) {
    char *sql = "SELECT * FROM Filmes WHERE Nome = ? LIMIT 1;";
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

    rc = sqlite3_step(stmt);
    int id;
    if (rc == SQLITE_ROW) {
        filme->id = (int) sqlite3_column_int64(stmt, 0);
        filme->nome = cpytext(sqlite3_column_text(stmt, 1));
        filme->genero = cpytext(sqlite3_column_text(stmt, 2));
        filme->descricao = cpytext(sqlite3_column_text(stmt, 3));
        id = filme->id;
        fprintf(stderr, "Filme ID: %d\n", filme->id);
    } else {
        fprintf(stderr, "Filme not found!\n");
        id = -1;
    }

    sqlite3_finalize(stmt);

    return id;
}

int getFilmesByGenero(char *genero, s_filme ***filmes) {
    char *sql = "SELECT * FROM Filmes WHERE Genero = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepere statment: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_bind_text(stmt, 1, genero, strlen(genero), 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind data: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    int id;
    linked_list *list = calloc(1, sizeof(linked_list));
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        s_filme *filme = calloc(1, sizeof(s_filme));
        filme->id = (int) sqlite3_column_int64(stmt, 0);
        filme->nome = cpytext(sqlite3_column_text(stmt, 1));
        filme->genero = cpytext(sqlite3_column_text(stmt, 2));
        filme->descricao = cpytext(sqlite3_column_text(stmt, 3));
        id = filme->id;
        linked_list *last = list->next;
        list->next = calloc(1, sizeof(linked_list));
        list->next->data = filme;
        list->next->next = last;
        count++;
    }

    *(filmes) = calloc(count, sizeof(s_filme*));
    for (int i = count; count >= 1 && list->next != NULL; i--) {
        (*(filmes))[i-1] = (s_filme *)list->next->data;
        linked_list *last = list->next;
        list->next = list->next->next;
        free(last);
    }
    free(list);

    sqlite3_finalize(stmt);

    return count;
}
