#include "Pin.h"

#ifndef RELAY_H
#define RELAY_H

class Relay : OutputPin
{
  private:
  bool m_RelayOn;
  bool m_LowLevelTrigger;

  public:
  Relay(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum, bool LowLevelTrigger = false);
  uint8_t RelayOn();
  uint8_t RelayOff();
  uint8_t RelayToggle();
  uint8_t IsRelayOn();
};

#endif // RELAY_H