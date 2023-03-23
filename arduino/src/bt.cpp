#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "mohawk.h"

#define GPS_SERVICE        "ba5e0001-da9b-4622-b128-1e4f5022ab01"
#define GPS_CHARACTERISTIC "ba5e0002-ad0c-4fe2-af23-55995ce8eb02"

// Global bluetooth state
bool bt_connected = false;
static BLECharacteristic *gps_ch;

static void advertise();

class GpsServer : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      bt_connected = true;
      Serial.println("BT client connected");
    };
    void onDisconnect(BLEServer* pServer) {
      bt_connected = false;
      Serial.println("BT client disconnected");
      advertise();
    }
};

class GpsCharacteristic : public BLECharacteristicCallbacks {
    void onRead(BLECharacteristic *pCharacteristic) {
    };
    void onWrite(BLECharacteristic *pCharacteristic) {
      // Handle commands from phone here
      std::string value = pCharacteristic->getValue();
      Serial.printf("Unexpected BT msg %02x %d\n", value[0], value.length());
    }
};

void bt_init() {
  // Init BLE
  BLEDevice::init("Mohawk"); // Device name
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new GpsServer());
  BLEService *pService = pServer->createService(GPS_SERVICE);

  // GPS characteristic
  gps_ch = pService->createCharacteristic(
    GPS_CHARACTERISTIC,
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );
  BLEDescriptor *pDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2902));
  // Bluetooth Core Spec v4.1+:
  // The default value for the Client Characteristic Configuration descriptor value shall be 0x0000
  uint8_t descriptorValue[2] = {0, 0};
  pDescriptor->setValue(descriptorValue, 2);
  gps_ch->addDescriptor(pDescriptor);
  gps_ch->setCallbacks(new GpsCharacteristic());

  // Enable bonding, so that it can be discovered in BASEline
  BLESecurity *bSecurity = new BLESecurity();
  bSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
  bSecurity->setCapability(ESP_IO_CAP_NONE);

  pService->start();
  advertise();
}

static void advertise() {
  // BLEAdvertising *pAdvertising = pServer->getAdvertising(); // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(GPS_SERVICE);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void bt_send(uint8_t *data, size_t len) {
  gps_ch->setValue(data, len);
  gps_ch->notify();
}
