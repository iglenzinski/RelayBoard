#include "Constants.h"
#include "Errors.h"
#include "Relay.h"
#include "SerialManager.h"

#ifndef RELAYMANAGER_H
#define RELAYMANAGER_H

#define MAX_RELAY_COUNT 16

class RelayManager
{
  private:
  uint8_t m_RelayCount;
  uint8_t m_MaxRelayOn;
  uint8_t m_RelayOnCount;
  SerialManager* m_pSerialManager;
  Relay* m_pVecRelays[MAX_RELAY_COUNT];


  public:
  RelayManager(SerialManager* SerialManager, uint8_t MaxRelayOn);
  uint8_t AddRelay(Relay *NewRelay);
  uint8_t AddRelay(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum, RelayState State);
  uint8_t TurnRelayOn(uint8_t RelayNumber);
  uint8_t TurnRelayOff(uint8_t RelayNumber);
};

#endif // RELAYMANAGER_H