#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <common.h>

#define SERVER_IP "localhost"

int main(int argc, char *argv[]) {
    int sockfd = 0;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("Could not create socket");
        return 1;
    }

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);

    puts("Connecting...");
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
       puts("Connect Failed");
       return 1;
    }

    while (1) {
        char type[40];
        scanf(" %s", type);

        struct packet packet = {0};
        if (!strcmp(type, "inserir")) {
            packet.type = PT_INSERT_FILME;
            scanf(" %[^\n]s", packet.data); //Nome
            int len = strlen(packet.data)+1;
            scanf(" %[^\n]s", &packet.data[len]); //Genero
            len += strlen(&packet.data[len])+1;
            scanf(" %[^\n]s", &packet.data[len]); //Descricao

            write(sockfd, &packet, sizeof(packet));
            read(sockfd, &packet, sizeof(packet));
            printf("ID %d\n", ((int *)packet.data)[0]);
        }
        else if (!strcmp(type, "remover")) {
            packet.type = PT_REMOVE_FILME;
            scanf(" %d", (int *) &packet.data);

            write(sockfd, &packet, sizeof(packet));
        }
        else if (!strcmp(type, "listar_titulo")) {
            packet.type = PT_LIST_TITULO_SALA;

            write(sockfd, &packet, sizeof(packet));
            read(sockfd, &packet, sizeof(packet));
            printf("%s\n", packet.data);
        }
        else if (!strcmp(type, "listar_genero")) {
            packet.type = PT_LIST_GENERO;
            scanf(" %[^\n]s", packet.data);

            write(sockfd, &packet, sizeof(packet));
            read(sockfd, &packet, sizeof(packet));
            printf("%s\n", packet.data);
        }
        else if (!strcmp(type, "nome")) {
            packet.type = PT_FILM_NAME_FROM_ID;
            scanf(" %d", (int *) &packet.data);

            write(sockfd, &packet, sizeof(packet));
            read(sockfd, &packet, sizeof(packet));
            printf("%s\n", packet.data);
        }
        else if (!strcmp(type, "info")) {
            packet.type = PT_FILM_INFO_FROM_ID;
            scanf(" %d", (int *) &packet.data);

            write(sockfd, &packet, sizeof(packet));
            read(sockfd, &packet, sizeof(packet));
            printf("%s\n", packet.data);
        }
        else if (!strcmp(type, "tudo")) {
            packet.type = PT_LIST_ALL;

            write(sockfd, &packet, sizeof(packet));
            read(sockfd, &packet, sizeof(packet));
            printf("%s\n", packet.data);
        }
        else {
            break;
        }
    }

    close(sockfd);

    return 0;
}
