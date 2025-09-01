#include "Relay.h"

Relay::Relay(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum, bool LowLevelTrigger) : OutputPin(DDRReg, PortReg, PinNum, LowLevelTrigger)
{
  m_RelayOn = false;
  m_LowLevelTrigger = LowLevelTrigger;
}

uint8_t Relay::RelayOn()
{  
  if (m_LowLevelTrigger)
  {
    WritePinLow();
  }
  else
  {
    WritePinHigh();
  }
  m_RelayOn = true;
}

uint8_t Relay::RelayOff()
{
  if (m_LowLevelTrigger)
  {
    WritePinHigh();
  }
  else
  {
    WritePinLow();
  }
  m_RelayOn = false;
}

uint8_t Relay::RelayToggle()
{
  if (m_RelayOn)
  {
    RelayOff();
  }
  else
  {
    RelayOn();
  }
}

uint8_t Relay::IsRelayOn()
{
  return m_RelayOn;
}