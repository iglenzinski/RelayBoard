#include "Relay.h"

Relay::Relay(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum, RelayState State) : OutputPin(DDRReg, PortReg, PinNum)
{
  m_RelayState = State;
}

uint8_t Relay::RelayOn()
{
  WritePinHigh();
}

uint8_t Relay::RelayOff()
{
  WritePinLow();
}