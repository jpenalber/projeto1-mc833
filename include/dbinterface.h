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

int removeFilme(int filme_id);

int deleteSalaFromID(int sala_id);

int insertExibicao(int sala_id, int filme_id);

int deleteExibicao(int sala_id, int filme_id);

int deleteExibicaoFromFilme(int filme_id);

int deleteExibicaoFromSala(int sala_id);

int oneIntBindStatment(char *sql, int bind);

int getFilmeIDByNome(char *nome);

int getInfoByID(int filme_id, s_filme *filme);

char *getNameByID(int filme_id);

int getAllByGenero(char *genero, s_filme ***filmes);

int getAllInfo(s_filme ***filmes);

int getAllTituloSala(s_filme ***filmes);

char *cpytext(char *text);
