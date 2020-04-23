#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <filme.h>

int open_db(char *path);

int insertSala(char *tipo);

// Inset a Film and return ID
int insertFilmeStruct(s_filme filme);

// Inset a Film and return ID
int insertFilme(char *nome, char *genero, char *descricao);

int deleteFilmeFromID(int filme_id);

int deleteSalaFromID(int sala_id);

int insertExibicao(int sala_id, int filme_id);

int deleteExibicao(int sala_id, int filme_id);

int deleteExibicaoFromFilme(int filme_id);

int deleteExibicaoFromSala(int sala_id);

int oneIntBindStatment(char *sql, int bind);

int getFilmeIDByNome(char *nome);

int getFilmeByID(int filme_id, s_filme *filme);

char *getFilmeNomeByID(int filme_id);

int getFilmesByGenero(char *genero, s_filme ***filmes);

char *cpytext(const unsigned char *text);
