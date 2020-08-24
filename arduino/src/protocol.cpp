#include "mohawk.h"

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
    .alt = (short)(point->alt * 10), // decimeters
    .vN = (short)(point->vN * 100), // cm/s
    .vE = (short)(point->vE * 100), // cm/s
    .climb = (short)(point->climb * 100) // cm/s
  };
  return msg;
}
