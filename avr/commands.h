#include "../utils/bit.h"

#define AVR_CMD ((uint16_t)0)

#define AVR_DRS_DISABLED (AVR_CMD)
#define AVR_DRS_ENABLED (set_bit(AVR_CMD, 0))
#define AVR_DRS_ALLOWED (set_bit(AVR_CMD, 1))
#define AVR_DRS_ACTIVATED (set_bit(set_bit(AVR_CMD, 0), 1))
