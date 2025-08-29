#include "Constants.h"
#include "Errors.h"
#include "Pin.h"

#ifndef RELAY_H
#define RELAY_H

enum RelayState
{
  NORMAL_ON         = 0b00000001,
  NORMAL_OFF        = 0b00000010,
  NORMAL_START_ON   = 0b00000100,
  NORMAL_START_OFF  = 0b00001000,
  BUTTON_TOGGLE     = 0b00010000,
  DISABLED          = 0b00100000
};

class Relay : OutputPin
{
  private:
  RelayState m_RelayState;

  public:
  Relay(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum, RelayState State);
  uint8_t RelayOn();
  uint8_t RelayOff();
};

#endif // RELAY_H