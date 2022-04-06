#include "header.h"

#define CAR_DAMAGE_DATA_SIZE sizeof(struct CarDamageData)
#define PACKET_CAR_DAMAGE_DATA_SIZE sizeof(struct PacketCarDamageData)

struct CarDamageData
{
    float m_tyresWear[4];         // Tyre wear (percentage)
    uint8 m_tyresDamage[4];       // Tyre damage (percentage)
    uint8 m_brakesDamage[4];      // Brakes damage (percentage)
    uint8 m_frontLeftWingDamage;  // Front left wing damage (percentage)
    uint8 m_frontRightWingDamage; // Front right wing damage (percentage)
    uint8 m_rearWingDamage;       // Rear wing damage (percentage)
    uint8 m_floorDamage;          // Floor damage (percentage)
    uint8 m_diffuserDamage;       // Diffuser damage (percentage)
    uint8 m_sidepodDamage;        // Sidepod damage (percentage)
    uint8 m_drsFault;             // Indicator for DRS fault, 0 = OK, 1 = fault
    uint8 m_gearBoxDamage;        // Gear box damage (percentage)
    uint8 m_engineDamage;         // Engine damage (percentage)
    uint8 m_engineMGUHWear;       // Engine wear MGU-H (percentage)
    uint8 m_engineESWear;         // Engine wear ES (percentage)
    uint8 m_engineCEWear;         // Engine wear CE (percentage)
    uint8 m_engineICEWear;        // Engine wear ICE (percentage)
    uint8 m_engineMGUKWear;       // Engine wear MGU-K (percentage)
    uint8 m_engineTCWear;         // Engine wear TC (percentage)
} __attribute__((packed));

struct PacketCarDamageData
{
    struct PacketHeader m_header; // Header
    
    struct CarDamageData m_carDamageData[22];
} __attribute__((packed));