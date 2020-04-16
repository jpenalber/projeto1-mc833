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
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr = {0};

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    open_db("test.db");

    while(1) {
        puts("Accepting...");
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        puts("Accepted connection");

        struct packet recv_buffer = {0};
        int read_n = read(connfd, &recv_buffer, sizeof(recv_buffer));

        switch (recv_buffer.type) {
        case PT_INSERT_SALA:
            insertSala(recv_buffer.data);
            break;
        case PT_INSERT_FILME:
            {
                int desc = findNextString(0, recv_buffer.data);
                insertFilme(recv_buffer.data, &recv_buffer.data[desc]);
            }
            break;
        default:
            puts("Malformed packet");
        }

        close(connfd);
        sleep(1);
    }
}
