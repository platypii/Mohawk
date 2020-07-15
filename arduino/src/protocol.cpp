#include "mohawk.h"

/**
 * Pack point into compact LocationMessage
 */
LocationMessage pack(GeoPointV *point) {
  // three least significant bytes of tenths since epoch
  long long tenths = (point->millis + 50) / 100; // round to nearest tenth
  uint8_t time1 = tenths >> 16;
  uint8_t time2 = tenths >> 8;
  uint8_t time3 = tenths;
  LocationMessage msg = {
    'L',
    time1, time2, time3,
    (int)(point->lat * 1e6), // microdegrees
    (int)(point->lng * 1e6), // microdegrees
    (short)(point->alt * 10), // decimeters
    (short)(point->vN * 100), // cm/s
    (short)(point->vE * 100), // cm/s
    (short)(point->climb * 100) // cm/s
  };
  return msg;
}
