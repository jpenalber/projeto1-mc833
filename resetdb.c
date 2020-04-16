#include <sqlite3.h>
#include <stdio.h>

int main( void ) {
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    char *sql = "DROP TABLE IF EXISTS Salas;"
                "CREATE TABLE Salas(sala_id INTEGER PRIMARY KEY AUTOINCREMENT, Tipo TEXT);" 
                "INSERT INTO Salas (Tipo) VALUES('3D');" 
                "INSERT INTO Salas (Tipo) VALUES('Padrao');" 
                "INSERT INTO Salas (Tipo) VALUES('IMAX');"
                "DROP TABLE IF EXISTS Filmes;"
                "CREATE TABLE Filmes(filme_id INTEGER PRIMARY KEY AUTOINCREMENT, Nome TEXT, Descricao TEXT);" 
                "INSERT INTO Filmes (Nome, Descricao) VALUES('Star Wars', 'Varios tiros e violencia no espaco');" 
                "INSERT INTO Filmes (Nome, Descricao) VALUES('Vingadores', 'Muitos herios para manter a conta');" 
                "INSERT INTO Filmes (Nome, Descricao) VALUES('Harry Potter', 'Star Wars mas com magia e bruxos');"
                "DROP TABLE IF EXISTS Exibicao;"
                "CREATE TABLE Exibicao(Id INTEGER PRIMARY KEY AUTOINCREMENT, sala_id INT, filme_id INT, FOREIGN KEY (sala_id) REFERENCES Salas(sala_id), FOREIGN KEY (filme_id) REFERENCES Filmes(filme_id));" 
                "INSERT INTO Exibicao (sala_id, filme_id) VALUES(1, 2);"
                "INSERT INTO Exibicao (sala_id, filme_id)  VALUES(2, 3);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    } 
    
    sqlite3_close(db);
    
    return 0;
}