#include <Arduino.h>
#include "mohawk.h"

static long last_blink = -1000;

void setup() {
  Serial.begin(115200);
  Serial.println("BASEline Mohawk");
  gps_init();
  bt_init();
  led_init();
  beat(2);
}

void loop() {
  gps_loop();
}

void update_location(GeoPointV *point) {
  Serial.printf(
    "GPS %.3fs %lld %f, %f, %.1f m, %.1f m/s, %.1f m/s\n",
    millis() * 1e-3, point->millis, point->lat, point->lng, point->alt, point->climb, hypot(point->vE, point->vN)
  );
  if (last_location) free(last_location);
  last_location = point;
  // Pack point
  LocationMessage msg = pack(point);
  bt_send((uint8_t*) &msg, sizeof(msg));
  // Blink at most once per second
  if (millis() - last_blink >= 950) {
    last_blink = millis();
    blink();
  }
}
