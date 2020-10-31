#include <EEPROM.h>

#define ADDR_NAME 0

/**
 * Get device name from EEPROM
 */
std::string get_device_name() {
  return EEPROM.readString(ADDR_NAME);
}

/**
 * Save device name to EEPROM
 */
void set_device_name(char *name) {
  EEPROM.put(ADDR_NAME, name);
  EEPROM.commit();
}
