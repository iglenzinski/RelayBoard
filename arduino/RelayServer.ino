#include "Constants.h"
#include "Errors.h"
#include "Relay.h"
#include "RelayData.h"
#include "RelayManager.h"
#include "RelayProtocol.h"
#include "Pin.h"
#include "SerialManager.h"


int NamesAddr = 0;
int value = EEPROM.read(NamesAddr);
SerialManager SerialMgr(115200);
RelayManager Manager(&SerialMgr, MAX_RELAY_ON_COUNT);
void setup() {
  SerialManager SerialMgr(115200);
  Manager.AddRelay(&DDRC, &PORTC, 0, RelayState::NORMAL_ON);
  Manager.AddRelay(&DDRC, &PORTC, 1, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRC, &PORTC, 2, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRC, &PORTC, 3, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRC, &PORTC, 4, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRC, &PORTC, 5, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRC, &PORTC, 6, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRC, &PORTC, 7, RelayState::NORMAL_ON);
  
  // Manager.AddRelay(&DDRA, &PORTA, 7, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRA, &PORTA, 6, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRA, &PORTA, 5, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRA, &PORTA, 4, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRA, &PORTA, 3, RelayState::NORMAL_ON);
  // Manager.AddRelay(&DDRA, &PORTA, 2, RelayState::NORMAL_ON);
  Manager.AddRelay(&DDRA, &PORTA, 1, RelayState::NORMAL_ON);
  Manager.AddRelay(&DDRA, &PORTA, 0, RelayState::NORMAL_ON);

  if ((value < 0x30) || (value > 0x39))
  {
    EEPROM.write(NamesAddr, 0x30);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  value = EEPROM.read(NamesAddr);  
  // SerialMgr.WriteString((char)value);

  if (value == 0x39)
  {
    EEPROM.write(NamesAddr, 0x30);
  }
  else
  {
    EEPROM.write(NamesAddr, value + 1);
  }
  Manager.TurnRelayOn(1);
  SerialMgr.WriteDebug("Relay On");
  // Rly.RelayOn();
  delay(5000);
  Manager.TurnRelayOff(1);
  SerialMgr.WriteDebug("Relay Off");
  // Rly.RelayOff();
  delay(5000);
}