#include <Arduino.h>
#include <time.h>
#include "mohawk.h"

/**
 * Print date as ISO8601 string
 * 2023-03-22T16:20:00.000Z
 */
void log_date(struct tm *date, int millis) {
  char str[21];
  strftime(str, sizeof(str), "%Y-%m-%dT%H:%M:%S", date);
  Serial.print(str);
  Serial.printf(".%.3dZ", millis);
}

/**
 * Log millis() or UTC time if available
 */
void log_millis() {
  const long milli = millis();
  if (last_location) {
    // Get current millis since epoch
    const long long epoch = mktime(&last_location->date) * 1000LL
      + last_location->millis + milli - last_fix_millis;
    const time_t now = epoch / 1000;
    const int remainder = epoch % 1000;
    // Convert back to date and log it
    struct tm *date = gmtime(&now);
    log_date(date, remainder);
    Serial.print(' ');
  } else {
    Serial.printf("[%.1fs] ", milli);
  }
}
