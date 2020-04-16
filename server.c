#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr = {0};

    char sendBuff[1025] = {0};
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    while(1) {
        puts("Accepting...");
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        puts("Accepted connection");

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));

        close(connfd);
        sleep(1);
    }
}
