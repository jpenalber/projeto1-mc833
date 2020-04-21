#include <dbinterface.h>
#include <common.h>

sqlite3 *db;

void linked_list_push(linked_list *list, void *data);

void *linked_list_pop(linked_list *list);

int open_db(char *path) {
    int rc = sqlite3_open(path, &db);

    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return -1;
    }

    return 0;
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
    return insertFilme(filme.nome, filme.genero, filme.descricao);
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

char *getFilmeNomeByID(int filme_id) {
    char *sql = "SELECT nome FROM Filmes WHERE filme_id = ? LIMIT 1;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepere statment: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    rc = sqlite3_bind_int(stmt, 1, filme_id);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind data: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    char *nome = NULL;
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        nome = cpytext(sqlite3_column_text(stmt, 0));
        fprintf(stderr, "Filme Name: %s\n", nome);
    } else {
        fprintf(stderr, "Filme not found!\n");
    }

    sqlite3_finalize(stmt);

    return nome;
}

char *cpytext(const unsigned char *text) {
    char *new_text = calloc(strlen((const char *)text)+1, sizeof(char));
    strcpy(new_text, (const char *)text);
    return new_text;
}

int getFilmeByID(int filme_id, s_filme *filme) {
    char *sql = "SELECT * FROM Filmes WHERE filme_id = ? LIMIT 1;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepere statment: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_bind_int(stmt, 1, filme_id);
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

    if (id >= 0) {
        char *sql = "SELECT sala_id FROM Exibicao WHERE filme_id = ?;";

        int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Cannot prepere statment: %s\n", sqlite3_errmsg(db));
            return -1;
        }

        rc = sqlite3_bind_int(stmt, 1, filme_id);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Cannot bind data: %s\n", sqlite3_errmsg(db));
            return -1;
        }

        linked_list *list = calloc(1, sizeof(linked_list));
        int count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int *sala = calloc(1, sizeof(linked_list));
            *sala = (int) sqlite3_column_int64(stmt, 0);
            linked_list_push(list, sala);
            count++;
        }

        filme->num_salas = count;
        filme->salas = calloc(count, sizeof(int));
        for (int i = count; count >= 1 && list->next != NULL; i--) {
            int *p = (int *) linked_list_pop(list);
            filme->salas[i-1] = *p;
        }
        free(list);
        sqlite3_finalize(stmt);
    }

    

    return id;
}

void linked_list_push(linked_list *list, void *data) {
    linked_list *last = list->next;
    list->next = calloc(1, sizeof(linked_list));
    list->next->data = data;
    list->next->next = last;
}

void* linked_list_pop(linked_list *list) {
    void *data = list->next->data;
    linked_list *last = list->next;
    list->next = list->next->next;
    free(last);
    return data;
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

    linked_list *list = calloc(1, sizeof(linked_list));
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        s_filme *filme = calloc(1, sizeof(s_filme));
        filme->id = (int) sqlite3_column_int64(stmt, 0);
        filme->nome = cpytext(sqlite3_column_text(stmt, 1));
        filme->genero = cpytext(sqlite3_column_text(stmt, 2));
        filme->descricao = cpytext(sqlite3_column_text(stmt, 3));
        linked_list_push(list, filme);
        count++;
    }

    *(filmes) = calloc(count, sizeof(s_filme*));
    for (int i = count; count >= 1 && list->next != NULL; i--) {
        (*(filmes))[i-1] = (s_filme *) linked_list_pop(list);
    }
    free(list);

    sqlite3_finalize(stmt);

    return count;
}
