#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#include "app.h"
#include "switchs.h"

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

    uint8 m_playerCarIndex;

    struct PacketEventData packetEventData;
    uint8 drsEnabled = 0;
    uint8 chequeredFlag = 0;

    struct PacketCarStatusData packetCarStatusData;
    uint8 prev_m_drsAllowed = 255;
    uint8 curr_m_drsAllowed = prev_m_drsAllowed;
    int8 m_vehicleFiaFlags;

    while (1)
    {

        res = recvfrom(sock, &buf, 1472, 0,
                       (struct sockaddr *)&client_addr, &addr_len);

        // printf("\n(%s , %d) said : ", inet_ntoa(client_addr.sin_addr),
        //        ntohs(client_addr.sin_port));

        packetHeader = *((struct PacketHeader *)&buf);
        m_playerCarIndex = packetHeader.m_playerCarIndex;

        switch (packetHeader.m_packetId)
        {
        case MOTION:
            // printf("MOTION packet\n");
            break;
        case SESSION:
            // printf("SESSION packet\n");
            break;
        case LAP_DATA:
            // printf("LAP_DATA packet\n");
            break;
        case EVENT:
            // printf("EVENT packet\n");
            packetEventData = *((struct PacketEventData *)&buf);

            printf("[ %u, ", packetEventData.m_eventStringCode[0]);
            printf("%u, ", packetEventData.m_eventStringCode[1]);
            printf("%u, ", packetEventData.m_eventStringCode[2]);
            printf("%u ]\n", packetEventData.m_eventStringCode[3]);
            break;
        case PARTICIPANTS:
            // printf("PARTICIPANTS packet\n");
            break;
        case CAR_SETUP:
            // printf("CAR_SETUP packet\n");
            break;
        case CAR_TELEMETRY:
            // printf("CAR_TELEMETRY packet\n");
            break;
        case CAR_STATUS:
            // printf("CAR_STATUS packet\n");
            packetCarStatusData = *((struct PacketCarStatusData *)&buf);

            curr_m_drsAllowed = packetCarStatusData.m_carStatusData[m_playerCarIndex].m_drsAllowed;
            if (curr_m_drsAllowed != prev_m_drsAllowed)
            {
                prev_m_drsAllowed = curr_m_drsAllowed;
                printf("DRS ENABLED - ALLOwED { %u, %u }\n", drsEnabled, curr_m_drsAllowed);
            }

            m_vehicleFiaFlags = packetCarStatusData.m_carStatusData[m_playerCarIndex].m_vehicleFiaFlags;
            switch (m_vehicleFiaFlags)
            {
            case GREEN:
                printf("GREEN FLAG\n");
                break;
            case BLUE:
                printf("BLUE FLAG\n");
                break;
            case YELLOW:
                printf("YELLOW FLAG\n");
                break;
            case RED:
                printf("RED FLAG\n");
                break;
            default:
                printf("NO FLAG\n");
                break;
            }
            break;
        case FINAL_CLASSIFICATION:
            // printf("FINAL_CLASSIFICATION packet\n");
            break;
        case LOBBY_INFO:
            // printf("LOBBY_INFO packet\n");
            break;
        case CAR_DAMAGE:
            // printf("CAR_DAMAGE packet\n");
            break;
        case SESSION_HISTORY:
            // printf("SESSION_HISTORY packet\n");
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