#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#include "app.h"

int main(int argc, char **argv)
{
    ssize_t res;
    struct PacketHeader packetHeader;
    int sockfd;
    struct sockaddr_in servaddr;

    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket()");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
    {
        perror("connect()");
        exit(1);
    }

    res = recvfrom(sockfd, &packetHeader, sizeof(struct PacketHeader), 0,(struct sockaddr*)NULL, NULL);
    if (res < 0) {
        perror("recvfrom()");
        exit(1);
    }
    
    printf("retrieved %ld bytes\n", res);
    close(sockfd);
    exit(0);

}