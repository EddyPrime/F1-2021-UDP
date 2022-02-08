#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#include "app.h"

int main()
{
    int sock;
    int addr_len;
    ssize_t res;
    struct sockaddr_in server_addr, client_addr;

    char buf[1472];
    struct PacketHeader packetHeader;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket()");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // bzero(&(server_addr.sin_zero),8);

    if (bind(sock, (struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1)
    {
        perror("bind()");
        exit(1);
    }

    addr_len = sizeof(struct sockaddr);

    printf("server waiting on port %d\n", PORT);
    fflush(stdout);

    while (1)
    {

        res = recvfrom(sock, &buf, 1472, 0,
                       (struct sockaddr *)&client_addr, &addr_len);

        printf("\n(%s , %d) said : ", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        packetHeader = *((struct PacketHeader *)&buf);
        printf("%d\n", packetHeader.m_packetId);
        printf("%d\n", packetHeader.m_secondaryPlayerCarIndex);

        if (packetHeader.m_packetId == CAR_STATUS)
        {
            struct PacketCarStatusData packetCarStatusData = *((struct PacketCarStatusData *)&buf);
            printf("%d\n", packetCarStatusData.m_carStatusData[0].m_drsAllowed);
        }

        switch (packetHeader.m_packetId)
        {
        case MOTION:
            printf("MOTION packet\n");
            break;
        case SESSION:
            printf("SESSION packet\n");
            break;
        case LAP_DATA:
            printf("LAP_DATA packet\n");
            break;
        case EVENT:
            printf("EVENT packet\n");
            break;
        case PARTICIPANTS:
            printf("PARTICIPANTS packet\n");
            break;
        case CAR_SETUP:
            printf("CAR_SETUP packet\n");
            break;
        case CAR_TELEMETRY:
            printf("CAR_TELEMETRY packet\n");
            break;
        case CAR_STATUS:
            printf("CAR_STATUS packet\n");
            break;
        case FINAL_CLASSIFICATION:
            printf("FINAL_CLASSIFICATION packet\n");
            break;
        case LOBBY_INFO:
            printf("LOBBY_INFO packet\n");
            break;
        case CAR_DAMAGE:
            printf("CAR_DAMAGE packet\n");
            break;
        case SESSION_HISTORY:
            printf("SESSION_HISTORY packet\n");
            break;
        }

        if (PRODUCER)
        {
            printf("\nsending ack\n");
            char *ack = "ack\0";
            sendto(sock, ack, 4, 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

            fflush(stdout);
        }
    }

    return 0;
}