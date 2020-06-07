typedef struct Filme {
    int id;
    char *nome;
    char *genero;
    char *descricao;
    int sala;
    char *sala_tipo;
    int *salas;
    int num_salas;
} s_filme;

struct staticFilme {
    int id;
    char nome[50];
    char genero[50];
    char descricao[200];
    int sala;
    char sala_tipo[50];
    int salas[50];
    int num_salas;
};
