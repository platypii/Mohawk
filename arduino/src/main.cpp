#include <Arduino.h>
#include <time.h>
#include "mohawk.h"

static long last_blink = -1000;

void setup() {
  Serial.begin(115200);
  Serial.println("Mohawk");
  gps_init();
  bt_init(); // 700ms
  led_init();
  beat();
}

void loop() {
  gps_loop();
}

/**
 * Called when GPS location is updated
 */
void update_location(GeoPointV *point) {
  // Log the new point
  log_date(&point->date, point->millis);
  // Latitude,longitude
  Serial.printf(" %f,%f", point->lat, point->lng);
  // Altitude
  if (isnan(point->alt)) Serial.print(" nan");
  else Serial.printf(" %.1fm", point->alt);
  // Climb rate
  if (isnan(point->climb)) Serial.print(" nan");
  else Serial.printf(" %.1fm/s", point->climb);
  // Ground speed
  const double ground_speed = hypot(point->vE, point->vN);
  if (isnan(ground_speed)) Serial.print(" nan");
  else Serial.printf(" %.1fm/s", ground_speed);
  Serial.println();

  // Update location
  if (last_location) free(last_location);
  last_location = point;
  last_fix_millis = millis();

  // Pack point
  LocationMessage msg = pack(point);
  bt_send((uint8_t*) &msg, sizeof(msg));

  // Blink at most once per second
  if (millis() - last_blink >= 950) {
    last_blink = millis();
    blink();
  }
}
