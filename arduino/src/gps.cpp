#include <Arduino.h>
#include "mohawk.h"
#include "ublox.h"

GeoPointV *last_location;
long last_fix_millis = -1;

static char buffer[256];
static int buffer_index = 0;

void gps_init() {
  ublox_init();
}

void gps_loop() {
  while (Serial2.available()) {
    // Read line
    while (Serial2.available() && buffer_index < 256) {
      const char ch = Serial2.read();
      buffer[buffer_index] = ch;

      if (buffer[0] != '$' && buffer[0] != 0xb5) {
        log_millis();
        Serial.printf("gps unexpected: %c %x\n", buffer[0], buffer[0]);
        buffer_index = 0;
        break;
      }

      // Check for termination
      if (buffer[0] == '$' && ch == '\n') {
        buffer[buffer_index - 1] = '\0'; // "\r\n"
        // Serial.printf("nmea %s\n", buffer);
        parse_nmea(buffer);
        buffer_index = 0;
        break;
      } else if (buffer[0] == 0xb5 && buffer_index == sizeof(NAV_PVT) + 3) { // sizeof(NAV_PVT) == 96
        // Serial.printf("ubx %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
        parse_ublox(buffer);
        buffer_index = 0;
        break;
      }
      buffer_index++;
      if (buffer_index >= 256) {
        log_millis();
        Serial.print("gps missing terminator\n");
        buffer_index = 0;
      }
    }
  }
}
