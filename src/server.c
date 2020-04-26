#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#include <common.h>
#include <dbinterface.h>

int toStaticArray(struct staticFilme films[50], s_filme **list, int n) {
    int i;

    for (i = 0; i < n && i < 50; i++) {
        films[i].id = list[i]->id;
        if (list[i]->nome) strcpy(films[i].nome, list[i]->nome);
        else memset(films[i].nome, 0, sizeof(films[0].nome));
        if (list[i]->genero) strcpy(films[i].genero, list[i]->genero);
        else memset(films[i].genero, 0, sizeof(films[0].genero));
        if (list[i]->descricao) strcpy(films[i].descricao, list[i]->descricao);
        else memset(films[i].descricao, 0, sizeof(films[0].descricao));
        films[i].sala = list[i]->sala;
        if (list[i]->sala_tipo) strcpy(films[i].sala_tipo, list[i]->sala_tipo);
        else memset(films[i].sala_tipo, 0, sizeof(films[0].sala_tipo));
        memcpy(films[i].salas, list[i]->salas, list[i]->num_salas*sizeof(int));
        films[i].num_salas = list[i]->num_salas;
    }

    return i;
}

int main(int argc, char *argv[]) {
    int listenfd = 0;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("Could not create socket");
        return 1;
    }

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVER_PORT);

    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        puts("Could not bind socket");
        close(listenfd);
        return 1;
    }

    listen(listenfd, 10);

    open_db("test.db");

    while(1) {
        puts("Accepting...");
        int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        if (fork()) {
            // Parent
            close(connfd);
        }
        else {
            // Child
            struct packet packet = {0};
            read(connfd, &packet, sizeof(packet));

            switch (packet.type) {
                case PT_INSERT_FILME:
                    {
                        s_filme film;
                        struct staticFilme *pfilm = (struct staticFilme *) &packet.data;
                        film.nome = &pfilm->nome[0];
                        film.genero = &pfilm->genero[0];
                        film.descricao = &pfilm->descricao[0];
                        film.sala = pfilm->sala;
                        film.sala_tipo = &pfilm->sala_tipo[0];

                        int id = insertFilmeStruct(film);

                        packet.type = PT_INSERT_FILME_RESULT;
                        memcpy(packet.data, &id, sizeof(id));

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_REMOVE_FILME:
                    {
                        int id;
                        memcpy(&id, packet.data, sizeof(id));

                        removeFilme(id);
                    }
                    break;
                case PT_LIST_TITULO_SALA:
                    {
                        packet.type = PT_LIST_TITULO_SALA_RESULT;

                        s_filme **list;
                        int count = getAllTituloSala(&list);

                        struct staticFilme films[50];
                        count = toStaticArray(films, list, count);

                        memcpy(packet.data, films, sizeof(films));
                        packet.len = count;

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_LIST_GENERO:
                    {
                        packet.type = PT_LIST_GENERO_RESULT;

                        s_filme **list;
                        int count = getAllByGenero((char *) packet.data, &list);

                        struct staticFilme films[50];
                        count = toStaticArray(films, list, count);

                        memcpy(packet.data, films, sizeof(films));
                        packet.len = count;

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_FILM_NAME_FROM_ID:
                    {
                        int id;
                        memcpy(&id, packet.data, sizeof(id));

                        packet.type = PT_FILM_NAME_FROM_ID_RESULT;

                        char *name = getNameByID(id);
                        strcpy(packet.data, name);

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_FILM_INFO_FROM_ID:
                    {
                        int id;
                        memcpy(&id, packet.data, sizeof(id));

                        packet.type = PT_FILM_INFO_FROM_ID_RESULT;

                        s_filme *film = malloc(sizeof(s_filme));
                        getInfoByID(id, film);

                        struct staticFilme films[50];
                        toStaticArray(films, &film, 1);
                        free(film);

                        memcpy(packet.data, films, sizeof(struct staticFilme));
                        packet.len = 1;

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_LIST_ALL:
                    {
                        packet.type = PT_LIST_ALL_RESULT;

                        s_filme **list;
                        int count = getAllTituloSala(&list);

                        struct staticFilme films[50];
                        count = toStaticArray(films, list, count);

                        memcpy(packet.data, films, sizeof(films));
                        packet.len = count;

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                default:
                    puts("Malformed packet");
            }

            close(connfd);
            return 0;
        }
    }
}
