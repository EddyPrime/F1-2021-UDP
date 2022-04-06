#include <stdint.h>

#define millisToMins(millis) millis / 60000
#define minsToMillis(mins) mins * 60000

#define millisToSecs(millis) millis / 1000
#define secsToMillis(secs) secs * 1000

struct lap_time {
    uint8_t mins;
    uint8_t secs;
    uint16_t millis;
} __attribute__((packed));

struct lap_time millisToMinSecMillis(uint32_t millis) {
    struct lap_time lt;

    lt.mins = millisToMins(millis);
    millis -= minsToMillis(lt.mins);

    lt.secs = millisToSecs(millis);
    millis -= secsToMillis(lt.secs);

    lt.millis = millis;

    return lt;
}