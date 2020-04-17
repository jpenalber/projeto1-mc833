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

            switch (packet.type) {
                case PT_INSERT_SALA:
                    {
                        int id = insertSala(packet.data);

                        packet.type = PT_INSERT_SALA_ID;
                        memcpy(packet.data, &id, sizeof(id));

                        write(connfd, &packet, sizeof(packet));
                    }
                    break;
                case PT_INSERT_FILME:
                    {
                        int gen = findNextString(0, packet.data);
                        int desc = findNextString(gen, packet.data);
                        int id = insertFilme(packet.data, &packet.data[gen], &packet.data[desc]);

                        packet.type = PT_INSERT_FILME_ID;
                        memcpy(packet.data, &id, sizeof(id));

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
