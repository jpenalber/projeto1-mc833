#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "localhost"
#define SERVER_PORT 5000

int main(int argc, char *argv[]) {
    int sockfd = 0, n = 0;
    char recvBuff[1024] = {0};
    struct sockaddr_in serv_addr = {0};

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Could not create socket\n");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);

    puts("Connecting...");
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
       printf("Connect Failed\n");
       return 1;
    }

    while ((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
        puts("Reading connection");
        recvBuff[n] = 0;
        fputs(recvBuff, stdout);
    }

    return 0;
}
