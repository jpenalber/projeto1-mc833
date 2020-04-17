#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

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

    struct packet packet = {
        .type = PT_INSERT_FILME,
        .data = "Frozen\0Aventura\0Aquele do boneco de neve\0"
    };
    write(sockfd, &packet, sizeof(packet));

    while (read(sockfd, &packet, sizeof(packet)) > 0) {
        printf("ID %d\n", ((int *)packet.data)[0]);
    }

    close(sockfd);

    return 0;
}
