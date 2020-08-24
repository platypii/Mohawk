#include <Arduino.h>
#include "mohawk.h"

#define BLINK_ON 10 // ms
#define BLINK_OFF 200 // ms

#define CHANNEL_LED 0

static void beatTask(void *params);

void led_init() {
  ledcSetup(CHANNEL_LED, 20000, 8);
  ledcAttachPin(PIN_LED, CHANNEL_LED);
}

/**
 * Blink once quickly
 */
void blink() {
  const int ms = 3;
  ledcWrite(CHANNEL_LED, 255); // LED on
  delay(ms);
  ledcWrite(CHANNEL_LED, 0); // LED off
}

/**
 * Fade in and out twice
 */
void beat() {
  xTaskCreatePinnedToCore(beatTask, "Beat", 10000, NULL, 1, NULL, 0 /* core */);
}

static void beatTask(void *params) {
  const int ms = 25;
  const int steps = 20;
  const int count = 2;
  for (int i = 0; i < count; i++) {
    // Up
    for (int t = 0; t < steps; t++) {
      ledcWrite(CHANNEL_LED, t * 255 / steps);
      delay(ms);
    }
    // Down
    for (int t = steps - 1; t >= 0; t--) {
      delay(ms);
      ledcWrite(CHANNEL_LED, t * 255 / steps);
    }
    if (i < count - 1) {
      delay(200);
    }
  }
  vTaskDelete(NULL);
}
