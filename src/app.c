#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "app.h"

#if __linux__ || __APPLE__

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#elif _WIN32

#include <winsock2.h>

#endif

uint8 m_playerCarIndex, drsActivated;

struct PacketSessionData packetSessionData;
uint8 curr_m_sessionType, prev_m_sessionType;
uint8 curr_m_weather;

struct PacketEventData packetEventData;
uint8 drsEnabled, chequeredFlag;
uint32 m_buttonStatus;

struct PacketCarStatusData packetCarStatusData;
uint8 curr_m_drsAllowed, prev_m_drsAllowed;
int8 curr_m_vehicleFiaFlags, prev_m_vehicleFiaFlags;

char *color;

uint8 evenStringCodeCmp(uint8 *c1, uint8 *c2)
{
    uint8 res = c1[0] == c2[0];
    res &= c1[1] == c2[1];
    res &= c1[2] == c2[2];
    return res && (c1[3] == c2[3]);
}

int main()
{
    int sock, serial;
    size_t addr_len;
    ssize_t res;
    struct sockaddr_in server_addr, client_addr;

    char buf[PACKET_MAX_SIZE];
    struct PacketHeader packetHeader;

    uint16 cmd;
    char msg[2];

    cmd = AVR_DRS_ACTIVATED;
    msg = cmd;

    serial = serialport_init();
    while (1) {
        serialport_write(serial, msg, 16);
        sleep(2);
    }
    serialport_close(serial);

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

        res = recvfrom(sock, &buf, PACKET_MAX_SIZE, 0,
                       (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
        res = res; // FOR COMPILER HAPPINESS

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
            packetSessionData = *((struct PacketSessionData *)&buf);
            curr_m_sessionType = packetSessionData.m_sessionType;
            curr_m_weather = packetSessionData.m_weather;

            if (curr_m_sessionType != prev_m_sessionType)
            {
                prev_m_sessionType = curr_m_sessionType;
                if (curr_m_sessionType == R || curr_m_sessionType == R1 || curr_m_sessionType == R2)
                {
                    drsEnabled = 0;
                    break;
                }
                if (curr_m_weather == LIGHT_RAIN || curr_m_weather == HEAVY_RAIN || curr_m_weather == STORM)
                {
                    drsEnabled = 0;
                    break;
                }
                printf("DRS ENABLED BY DEFAULT\n");
                drsEnabled = 1;
            }
            break;
        case LAP_DATA:
            // printf("LAP_DATA packet\n");
            break;
        case EVENT:
            // printf("EVENT packet\n");
            packetEventData = *((struct PacketEventData *)&buf);

            if (evenStringCodeCmp(packetEventData.m_eventStringCode, SESSION_STARTED))
            {
                printf("SESSION STARTED\n");
            init:
                drsActivated = 0;
                chequeredFlag = 0;
                prev_m_drsAllowed = 255;
                prev_m_vehicleFiaFlags = INVALID_UNKNOWN;
                prev_m_sessionType = UNKNOWN;
                color = "None";
                break;
            }

            if (evenStringCodeCmp(packetEventData.m_eventStringCode, DRS_ENABLED))
            {
                printf("DRS ENABLED\n");
                drsEnabled = 1;
                break;
            }

            if (evenStringCodeCmp(packetEventData.m_eventStringCode, DRS_DISABLED))
            {
                printf("DRS DISABLED\n");
                drsEnabled = 0;
                drsActivated = 0;
                break;
            }

            if (evenStringCodeCmp(packetEventData.m_eventStringCode, CHEQUERED_FLAG))
            {
                printf("CHEQUERED FLAG\n");
                chequeredFlag = 1;
                break;
            }

            if (evenStringCodeCmp(packetEventData.m_eventStringCode, BUTTON_STATUS))
            {
                // printf("BUTTON STATUS\n");
                m_buttonStatus = packetEventData.m_eventDetails.Buttons.m_buttonStatus;

                /* NOTE: if drs is allowed, when it is activated then drsAllowed=0. */

                /*
                 *   If the vehicle breaks and the drs is active,
                 *   then it must be turned off.
                 */
                if (drsActivated && (m_buttonStatus & L2_OR_LT))
                {
                    drsActivated = 0;
                    printf("DRS DEACTIVATED DUE TO BREAK\n");
                }

                /*
                 *   If the drs is active and the driver pushes the drs button,
                 *   then it must be turned off.
                 */
                if (drsActivated && (m_buttonStatus & TRIANGLE_OR_Y))
                {
                    drsActivated = 0;
                    printf("DRS DEACTIVATED\n");
                }

                /*
                 *   If the drs is allowed but not active and the driver pushes the drs button,
                 *   then it must be turned on.
                 */
                if (!drsActivated && curr_m_drsAllowed && (m_buttonStatus & TRIANGLE_OR_Y))
                {
                    drsActivated = 1;
                    printf("DRS ACTIVATED\n");
                }
                break;
            }

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

            curr_m_vehicleFiaFlags = packetCarStatusData.m_carStatusData[m_playerCarIndex].m_vehicleFiaFlags;
            if (curr_m_vehicleFiaFlags != prev_m_vehicleFiaFlags)
            {
                prev_m_vehicleFiaFlags = curr_m_vehicleFiaFlags;
                switch (curr_m_vehicleFiaFlags)
                {
                case GREEN:
                    color = "Green";
                    goto flagColor;
                case BLUE:
                    color = "Blue";
                    goto flagColor;
                case YELLOW:
                    color = "Yellow";
                    goto flagColor;
                case RED:
                    color = "Red";
                    goto flagColor;
                default:
                    color = "None";
                    goto flagColor;
                }

            flagColor:
                printf("%s FLAG\n", color);
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
    }

    return 0;
}