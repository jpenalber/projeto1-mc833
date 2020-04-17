#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include "filme.h"

int open_db(char *path);

int insertSala(char *tipo);

// Inset a Film and return ID
int insertFilmeStruct(s_filme filme);

// Inset a Film and return ID
int insertFilme(char *nome, char *descricao);
