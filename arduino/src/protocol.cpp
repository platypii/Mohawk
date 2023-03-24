#include <limits.h>
#include "mohawk.h"

/**
 * Convert double to short (16 bits).
 * Out of range returns SHRT_MAX 0x7fff.
 */
static short pack16(double in) {
  if (SHRT_MIN <= in && in < SHRT_MAX) {
    return (short)(in);
  } else {
    return SHRT_MAX;
  }
}

/**
 * Pack point into compact LocationMessage
 */
LocationMessage pack(GeoPointV *point) {
  // three least significant bytes of tenths since epoch
  long long millis = mktime(&point->date) * 1000LL + point->millis;
  long long tenths = (millis + 50) / 100; // round to nearest tenth
  uint8_t time1 = tenths >> 16;
  uint8_t time2 = tenths >> 8;
  uint8_t time3 = tenths;
  LocationMessage msg = {
    .msg_type = 'L',
    time1, time2, time3,
    .lat = (int)(point->lat * 1e6), // microdegrees
    .lng = (int)(point->lng * 1e6), // microdegrees
    .alt = pack16(point->alt * 10 - 31768), // decimeters -100..6453.5m MSL
    .vN = pack16(point->vN * 100), // cm/s
    .vE = pack16(point->vE * 100), // cm/s
    .climb = pack16(point->climb * 100) // cm/s
  };
  return msg;
}
