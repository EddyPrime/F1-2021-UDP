#include "../packets/car_damage.h"
#include "../packets/car_setup.h"
#include "../packets/car_status.h"
#include "../packets/car_telemetry.h"
#include "../packets/event.h"
#include "../packets/final_classification.h"
#include "../packets/lap_data.h"
#include "../packets/lobby_info.h"
#include "../packets/motion.h"
#include "../packets/participants.h"
#include "../packets/session_history.h"
#include "../packets/session.h"

#include "../constants/_packet_ids.h"
#include "../constants/_events_string_codes.h"
#include "../constants/_fia_flags.h"
#include "../constants/_button_flags.h"
#include "../constants/_session_types.h"
#include "../constants/_weather.h"


#include "../config/config.h"

#define SERVER_ADDRESS "127.0.0.1"
#define PORT 20777

uint8 evenStringCodeCmp(uint8 *c1, uint8 *c2);