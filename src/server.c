#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#include <common.h>
#include <dbinterface.h>

int findNextString(int start, char *buffer) {
    int i = start;
    while (buffer[i]) i++;
    return i+1;
}

void addStringToPacket(char *data, char *string) {
    int start = 0;
    while (data[start]) start = findNextString(start, data);
    strcpy(&data[start], string);
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
            int read_n = read(connfd, &packet, sizeof(packet));
            int data_size = read_n - sizeof(enum packet_type);

            switch (packet.type) {
                case PT_INSERT_FILME:
                    {
                        int gen = findNextString(0, packet.data);
                        int desc = findNextString(gen, packet.data);
                        int id = insertFilme(packet.data, &packet.data[gen], &packet.data[desc]);

                        packet.type = PT_INSERT_FILME_RESULT;
                        memcpy(packet.data, &id, sizeof(id));

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_REMOVE_FILME:
                    {
                        int id;
                        memcpy(&id, packet.data, sizeof(id));

                        //removeFilme(id);
                    }
                    break;
                case PT_LIST_TITULO_SALA:
                    {
                        packet.type = PT_LIST_TITULO_SALA_RESULT;

                        //getAllTituloSala((char *) packet.data);

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_LIST_GENERO:
                    {
                        char list[1023] = {0};
                        packet.type = PT_LIST_GENERO_RESULT;

                        //getAllByGenero((char *) packet.data, list);
                        memcpy(packet.data, list, sizeof(list));

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_FILM_NAME_FROM_ID:
                    {
                        int id;
                        memcpy(&id, packet.data, sizeof(id));

                        packet.type = PT_FILM_NAME_FROM_ID_RESULT;

                        //getNameByID(id, (char *) packet.data);

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_FILM_INFO_FROM_ID:
                    {
                        int id;
                        memcpy(&id, packet.data, sizeof(id));

                        packet.type = PT_FILM_INFO_FROM_ID_RESULT;

                        s_filme film = {0};
                        //getInfoByID(id, &film);
                        addStringToPacket(packet.data, film.nome);
                        addStringToPacket(packet.data, film.genero);
                        addStringToPacket(packet.data, film.descricao);

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_LIST_ALL:
                    {
                        packet.type = PT_LIST_ALL_RESULT;

                        s_filme films[20] = {0};
                        int n = 0;
                        //int n = getAllInfo(films);
                        for (int i = 0; i < n; i++) {
                            addStringToPacket(packet.data, films[i].nome);
                            addStringToPacket(packet.data, films[i].genero);
                            addStringToPacket(packet.data, films[i].descricao);
                        }

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
