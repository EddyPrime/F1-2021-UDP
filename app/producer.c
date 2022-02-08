#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>

#include "producer.h"

int main()
{
    int sock, sin_size;
    struct sockaddr_in server_addr;
    struct hostent *host;
    char recv_data[8];
    ssize_t res;

    struct PacketHeader packetHeader;
    struct PacketCarStatusData packetCarStatusData;
    struct CarStatusData carStatusData;

    host = (struct hostent *)gethostbyname((char *)SERVER_ADDRESS);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    // bzero(&(server_addr.sin_zero), 8);
    sin_size = sizeof(struct sockaddr);

    printf("sizeof(car_damage) = %ld\n", sizeof(struct PacketCarDamageData));
    printf("sizeof(car_setups) = %ld\n", sizeof(struct PacketCarSetupData));
    printf("sizeof(car_status) = %ld\n", sizeof(struct PacketCarStatusData));
    printf("sizeof(car_telemetrt) = %ld\n", sizeof(struct PacketCarTelemetryData));
    printf("sizeof(event) = %ld\n", sizeof(struct PacketEventData));
    printf("sizeof(final_classification) = %ld\n", sizeof(struct PacketFinalClassificationData));
    printf("sizeof(lap_data) = %ld\n", sizeof(struct PacketLapData));
    printf("sizeof(lobby_info) = %ld\n", sizeof(struct PacketLobbyInfoData));
    printf("sizeof(motion) = %ld\n", sizeof(struct PacketMotionData));
    printf("sizeof(participants) = %ld\n", sizeof(struct PacketParticipantsData));
    printf("sizeof(session_history) = %ld\n", sizeof(struct PacketSessionHistoryData));
    printf("sizeof(session) = %ld\n", sizeof(struct PacketSessionData));

    packetHeader = (struct PacketHeader){
        .m_packetFormat = 2021,
        .m_gameMajorVersion = 1,
        .m_gameMinorVersion = 00,
        .m_packetVersion = 1,
        .m_packetId = CAR_STATUS,
        .m_sessionUID = 0,
        .m_sessionTime = 0,
        .m_frameIdentifier = 0,
        .m_playerCarIndex = 0,
        .m_secondaryPlayerCarIndex = 255,
    };

    carStatusData = (struct CarStatusData){
        .m_drsAllowed = 1,
    };

    packetCarStatusData.m_header = packetHeader;
    packetCarStatusData.m_carStatusData[0] = carStatusData;

    while (1)
    {

        sendto(sock, &packetCarStatusData, sizeof(struct PacketCarStatusData), 0,
               (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

        res = recvfrom(sock, recv_data, 8, 0, (struct sockaddr *)&server_addr, &sin_size);
        recv_data[res] = '\0';
        printf("Received :%s\n", recv_data);
        sleep(5);
    }
}