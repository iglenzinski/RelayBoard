#include "ErrorManager.h"
#include "Relay.h"
#include "RelayDataManager.h"
#include "RelayManager.h"
#include "RelayProtocol.h"
#include "SerialManager.h"
#include <Arduino.h>

// #define DEBUG
// Configuration definition
#define ARDUINO_MEGA_16_RELAYS
// #define ARDUINO_MAGE_8_RELAYS


#ifdef ARDUINO_MEGA_16_RELAYS

#define SERIAL_BAUD 115200
#define RELAY_COUNT 16
#define RELAY_MAX_ON_COUNT 8
#define EEPROM_ADDR_MIN 0
#define EEPROM_ADDR_MAX 4095

const Relay *ConfigurationRelays[RELAY_COUNT] = 
{
  new Relay(&DDRC, &PORTC, 0, true),
  new Relay(&DDRC, &PORTC, 1, true),
  new Relay(&DDRC, &PORTC, 2, true),
  new Relay(&DDRC, &PORTC, 3, true),
  new Relay(&DDRC, &PORTC, 4, true),
  new Relay(&DDRC, &PORTC, 5, true),
  new Relay(&DDRC, &PORTC, 6, true),
  new Relay(&DDRC, &PORTC, 7, true),
  new Relay(&DDRA, &PORTA, 7, true),
  new Relay(&DDRA, &PORTA, 6, true),
  new Relay(&DDRA, &PORTA, 5, true),
  new Relay(&DDRA, &PORTA, 4, true),
  new Relay(&DDRA, &PORTA, 3, true),
  new Relay(&DDRA, &PORTA, 2, true),
  new Relay(&DDRA, &PORTA, 1, true),
  new Relay(&DDRA, &PORTA, 0, true)
};

#endif //ARDUINO_MEGA_16_REALYS


#ifdef ARDUINO_MEGA_8_RELAYS

#define SERIAL_BAUD 115200
#define RELAY_COUNT 8
#define RELAY_MAX_ON_COUNT 8
#define EEPROM_ADDR_MIN 0
#define EEPROM_ADDR_MAX 4095

const Relay *ConfigurationRelays[RELAY_COUNT] = 
{
  new Relay(&DDRC, &PORTC, 0, true),
  new Relay(&DDRC, &PORTC, 1, true),
  new Relay(&DDRC, &PORTC, 2, true),
  new Relay(&DDRC, &PORTC, 3, true),
  new Relay(&DDRC, &PORTC, 4, true),
  new Relay(&DDRC, &PORTC, 5, true),
  new Relay(&DDRC, &PORTC, 6, true),
  new Relay(&DDRC, &PORTC, 7, true)
};

#endif //ARDUINO_MEGA_8_REALYS

SerialManager *SerialMgr;
RelayManager *RelayMgr;

void setup() {
  bool SerialDebug = false;
  #ifdef DEBUG
    SerialDebug = true;
  #endif

  // Delay to wait for everything to stablize
  delay(5000);

  SerialMgr = new SerialManager(SERIAL_BAUD, SerialDebug);

  RelayMgr = new RelayManager(SerialMgr, RELAY_COUNT, RELAY_MAX_ON_COUNT, EEPROM_ADDR_MIN, EEPROM_ADDR_MAX);
  RelayMgr->InitRelayData();

  for (uint8_t RelayIdx = 0; RelayIdx < RELAY_COUNT; RelayIdx++)
  {
    // WARNING - RELAY MANAGER INDEX STARTS AT 1
    RelayMgr->AddRelay(RelayIdx + 1, ConfigurationRelays[RelayIdx]);
  }
}

void loop() {
  if(SerialMgr->IsDataAvailable())
  {
    uint8_t val = SerialMgr->ReadByte()-48;
    Serial.println(val);
    RelayMgr->ToggleRelay(val);
  }
}