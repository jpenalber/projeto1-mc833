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
                "CREATE TABLE Salas(sala_id INTEGER PRIMARY KEY, Tipo TEXT);" 
                "INSERT INTO Salas VALUES(1, '3D');" 
                "INSERT INTO Salas VALUES(2, 'Padrao');" 
                "INSERT INTO Salas VALUES(3, 'IMAX');"
                "DROP TABLE IF EXISTS Filmes;"
                "CREATE TABLE Filmes(filme_id INTEGER PRIMARY KEY, Nome TEXT, Descricao TEXT);" 
                "INSERT INTO Filmes VALUES(1, 'Star Wars', 'Varios tiros e violencia no espaco');" 
                "INSERT INTO Filmes VALUES(2, 'Vingadores', 'Muitos herios para manter a conta');" 
                "INSERT INTO Filmes VALUES(3, 'Harry Potter', 'Star Wars mas com magia e bruxos');"
                "DROP TABLE IF EXISTS Exibicao;"
                "CREATE TABLE Exibicao(Id INTEGER PRIMARY KEY, sala_id INT, filme_id INT, FOREIGN KEY (sala_id) REFERENCES Salas(sala_id), FOREIGN KEY (filme_id) REFERENCES Filmes(filme_id));" 
                "INSERT INTO Exibicao VALUES(1, 1, 2);"
                "INSERT INTO Exibicao VALUES(2, 2, 3);";
                // "INSERT INTO Cars VALUES(3, 'Skoda', 9000);" 
                // "INSERT INTO Cars VALUES(4, 'Volvo', 29000);" 
                // "INSERT INTO Cars VALUES(5, 'Bentley', 350000);" 
                // "INSERT INTO Cars VALUES(6, 'Citroen', 21000);" 
                // "INSERT INTO Cars VALUES(7, 'Hummer', 41400);" 
                // "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";

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