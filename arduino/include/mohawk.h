#ifndef _MOHAWK_H
#define _MOHAWK_H

#include <stdint.h>
#include <stddef.h>

#define PIN_LED 8
#define PIN_GPS_TX 7 // white GPS output
#define PIN_GPS_RX 6 // green GPS input

#define to_radians(degrees) (degrees * M_PI / 180.0)

struct GeoPointV {
  long long millis;
  double lat;
  double lng;
  double alt;
  double climb;
  double vN;
  double vE;
};

/**
 * Message with location and speed.
 * sizeof 20
 */
#pragma pack(1)
struct LocationMessage {
  char msg_type; // 'L'
  uint8_t time1;
  uint8_t time2;
  uint8_t time3; // three least significant bytes of tenths since epoch
  int lat; // microdegrees
  int lng; // microdegrees
  short alt; // decimeters
  short vN; // cm/s
  short vE; // cm/s
  short climb; // cm/s
};

extern GeoPointV *last_location;

// LED
void led_init();
void blink();
void beat(int count);

void bt_init();
void bt_send(uint8_t *data, size_t len);

void gps_init();
void gps_loop();
void ublox_init();

void parse_nmea(char *line);
void parse_ublox(char *line);

void update_location(GeoPointV *point);

LocationMessage pack(GeoPointV *point);

#endif
