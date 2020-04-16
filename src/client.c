#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <common.h>

#define SERVER_IP "localhost"
#define SERVER_PORT 5000

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

    struct packet send_buffer = {
        .type = PT_INSERT_FILME,
        .data = "Frozen\0Aquele do boneco de neve\0"
    };
    write(sockfd, &send_buffer, sizeof(send_buffer));

    /*
    int n;
    while ((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
        puts("Reading connection");
        recvBuff[n] = 0;
        fputs(recvBuff, stdout);
    }
    */

    close(sockfd);

    return 0;
}
