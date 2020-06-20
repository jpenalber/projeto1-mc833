#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <poll.h>

#include <common.h>
#include <filme.h>

// #define SERVER_IP "187.56.54.16"
#define SERVER_IP "localhost"

#define TIMEOUT 10000

void printFilms(struct staticFilme films[MAX_FILMS], int len) {
    for (int i = 0; i < len; i++) {
        printf("id: %d\n", films[i].id);
        printf("nome: %s\n", films[i].nome);
        printf("genero: %s\n", films[i].genero);
        printf("descricao: %s\n", films[i].descricao);
        printf("sala: %d\n", films[i].sala);
        printf("sala_tipo: %s\n", films[i].sala_tipo);
        // printf("salas: ");
        // for (int j = 0; j < films[i].num_salas; j++) {
        //     printf("%d ", films[i].salas[j]);
        // }
        puts("\n");
    }
}

int main(int argc, char *argv[]) {
    int sockfd = 0;


    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        puts("Could not create socket");
        return 1;
    }

    struct pollfd poll_set;
    poll_set.fd = sockfd;
    poll_set.events = POLLIN;

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);

    while (1) {
        printf("Operacoes:\n"
                "\tinserir\t\t insere filme\n"
                "\tremover\t\t remove filme por ID\n"
                "\tlistar_titulo\t lista todos os titulos de filme\n"
                "\tlistar_genero\t lista todos os filmes de um genero\n"
                "\tnome\t\t diz nome do filme pelo ID\n"
                "\tinfo\t\t diz todas informacoes do filme pelo ID\n"
                "\ttudo\t\t diz todas informacoes de todos os filmes\n"
                "\tsair\t\t sair do sistema\n\n"
                "comando: ");

        char type[40];
        scanf(" %s", type);

        struct packet packet = {0};
	sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        if (!strcmp(type, "sair")) {
            break;
        }else if (!strcmp(type, "inserir")) {
start_inserir:
            packet.type = PT_INSERT_FILME;

            struct staticFilme film;
            printf("Nome: ");
            scanf(" %[^\n]s", film.nome);
            printf("Genero: ");
            scanf(" %[^\n]s", film.genero);
            printf("Descricao: ");
            scanf(" %[^\n]s", film.descricao);
            printf("Sala: ");
            scanf(" %d", &film.sala);

            memcpy(packet.data, &film, sizeof(film));

            struct timespec begin;
            clock_gettime(CLOCK_BOOTTIME, &begin);

            sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

            int count;
            char buffer[sizeof(packet)];
            char *bufftmp = &buffer[0];
            size_t bufflen = sizeof(packet);

            if (poll(&poll_set, 1, TIMEOUT) > 0) {
                count = recvfrom(sockfd, bufftmp, bufflen-1, 0, NULL, NULL);
            } else {
                goto start_inserir;
            }

            struct timespec end;
            clock_gettime(CLOCK_BOOTTIME, &end);

            printf("\n%% %f\n", (end.tv_nsec-begin.tv_nsec)/1000000000.0f);

            memcpy(&packet, buffer, sizeof(buffer));

            printf("ID %d\n", ((int *)packet.data)[0]);
        }
        else if (!strcmp(type, "remover")) {
start_remover:
            packet.type = PT_REMOVE_FILME;

            printf("ID: ");
            scanf(" %d", (int *) &packet.data);

            struct timespec begin;
            clock_gettime(CLOCK_BOOTTIME, &begin);

            sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

            int count;
            char buffer[sizeof(packet)];
            char *bufftmp = &buffer[0];
            size_t bufflen = sizeof(packet);

            if (poll(&poll_set, 1, TIMEOUT) > 0) {
                count = recvfrom(sockfd, bufftmp, bufflen-1, 0, NULL, NULL);
            } else {
                goto start_remover;
            }

            struct timespec end;
            clock_gettime(CLOCK_BOOTTIME, &end);

            printf("\n%% %f\n", (end.tv_nsec-begin.tv_nsec)/1000000000.0f);
        }
        else if (!strcmp(type, "listar_titulo")) {
start_listar_titulo:
            packet.type = PT_LIST_TITULO_SALA;

            struct timespec begin;
            clock_gettime(CLOCK_BOOTTIME, &begin);

            sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

            int count;
            char buffer[sizeof(packet)];
            char *bufftmp = &buffer[0];
            size_t bufflen = sizeof(packet);

            struct staticFilme films[MAX_FILMS];

            int total = 0;
            while (1) {
                if (poll(&poll_set, 1, TIMEOUT) > 0) {
                    count = recvfrom(sockfd, bufftmp, bufflen-1, 0, NULL, NULL);
                } else {
                    goto start_listar_titulo;
                }
                total++;
                memcpy(&packet, buffer, sizeof(buffer));
                memcpy(&films[packet.id], packet.data, packet.len*sizeof(struct staticFilme));
                if (total == packet.total) break;
            }

            struct timespec end;
            clock_gettime(CLOCK_BOOTTIME, &end);

            printf("\n%% %f\n", (end.tv_nsec-begin.tv_nsec)/1000000000.0f);

            for (int i = 0; i < total; i++) {
                printf("nome: %s\n", films[i].nome);
                printf("sala: %d\n\n", films[i].sala);
            }
        }
        else if (!strcmp(type, "listar_genero")) {
start_listar_genero:
            packet.type = PT_LIST_GENERO;

            printf("Genero: ");
            scanf(" %[^\n]s", packet.data);

            struct timespec begin;
            clock_gettime(CLOCK_BOOTTIME, &begin);

            sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

            int count;
            char buffer[sizeof(packet)];
            char *bufftmp = &buffer[0];
            size_t bufflen = sizeof(packet);

            struct staticFilme films[MAX_FILMS];

            int total = 0;
            while (1) {
                if (poll(&poll_set, 1, TIMEOUT) > 0) {
                    count = recvfrom(sockfd, bufftmp, bufflen-1, 0, NULL, NULL);
                } else {
                    goto start_listar_genero;
                }
                total++;
                memcpy(&packet, buffer, sizeof(buffer));
                memcpy(&films[packet.id], packet.data, packet.len*sizeof(struct staticFilme));
                if (total == packet.total) break;
            }

            struct timespec end;
            clock_gettime(CLOCK_BOOTTIME, &end);

            printf("\n%% %f\n", (end.tv_nsec-begin.tv_nsec)/1000000000.0f);

            for (int i = 0; i < total; i++) {
                printf("id: %d\n", films[i].id);
                printf("nome: %s\n\n", films[i].nome);
            }
        }
        else if (!strcmp(type, "nome")) {
start_nome:
            packet.type = PT_FILM_NAME_FROM_ID;

            printf("ID: ");
            scanf(" %d", (int *) &packet.data);

            struct timespec begin;
            clock_gettime(CLOCK_BOOTTIME, &begin);

            sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

            int count;
            char buffer[sizeof(packet)];
            char *bufftmp = &buffer[0];
            size_t bufflen = sizeof(packet);

            if (poll(&poll_set, 1, TIMEOUT) > 0) {
                count = recvfrom(sockfd, bufftmp, bufflen-1, 0, NULL, NULL);
            } else {
                goto start_nome;
            }

            struct timespec end;
            clock_gettime(CLOCK_BOOTTIME, &end);

            printf("\n%% %f\n", (end.tv_nsec-begin.tv_nsec)/1000000000.0f);

            memcpy(&packet, buffer, sizeof(buffer));

            printf("%s\n", packet.data);
        }
        else if (!strcmp(type, "info")) {
start_info:
            packet.type = PT_FILM_INFO_FROM_ID;

            printf("ID: ");
            scanf(" %d", (int *) &packet.data);

            struct timespec begin;
            clock_gettime(CLOCK_BOOTTIME, &begin);

            sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

            int count;
            char buffer[sizeof(packet)];
            char *bufftmp = &buffer[0];
            size_t bufflen = sizeof(packet);

            if (poll(&poll_set, 1, TIMEOUT) > 0) {
                count = recvfrom(sockfd, bufftmp, bufflen-1, 0, NULL, NULL);
            } else {
                goto start_info;
            }

            struct timespec end;
            clock_gettime(CLOCK_BOOTTIME, &end);

            printf("\n%% %f\n", (end.tv_nsec-begin.tv_nsec)/1000000000.0f);

            memcpy(&packet, buffer, sizeof(buffer));

            struct staticFilme film;
            memcpy(&film, packet.data, sizeof(film));

            printFilms(&film, 1);
        }
        else if (!strcmp(type, "tudo")) {
start_tudo:
            packet.type = PT_LIST_ALL;

            struct timespec begin;
            clock_gettime(CLOCK_BOOTTIME, &begin);

            sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

            int count;
            char buffer[sizeof(packet)];
            char *bufftmp = &buffer[0];
            size_t bufflen = sizeof(packet);

            struct staticFilme films[MAX_FILMS];

            int total = 0;
            while (1) {
                if (poll(&poll_set, 1, TIMEOUT) > 0) {
                    count = recvfrom(sockfd, bufftmp, bufflen-1, 0, NULL, NULL);
                } else {
                    goto start_tudo;
                }
                total++;
                memcpy(&packet, buffer, sizeof(buffer));
                memcpy(&films[packet.id], packet.data, packet.len*sizeof(struct staticFilme));
                if (total == packet.total) break;
            }

            struct timespec end;
            clock_gettime(CLOCK_BOOTTIME, &end);

            printf("\n%% %f\n", (end.tv_nsec-begin.tv_nsec)/1000000000.0f);

            printFilms(films, total);
        }
        else {
            puts("Wrong command");
        }
        
    }
    close(sockfd);


    return 0;
}
