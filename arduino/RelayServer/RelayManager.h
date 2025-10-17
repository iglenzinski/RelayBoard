#include "Relay.h"
#include "RelayDataManager.h"
#include "SerialManager.h"

#ifndef RELAYMANAGER_H
#define RELAYMANAGER_H

#define STABLE_TIMEOUT 10000

class RelayManager
{
  private:
  uint8_t m_RelayCount;
  uint8_t m_MaxRelayOn;
  uint8_t m_RelayOnCount;
  SerialManager* m_pSerialManager;
  RelayDataManager* m_pRelayData;
  Relay** m_pVecRelays;

  uint32_t m_StabilizationTimeout;
  uint32_t m_LastChangeTime;
  bool m_PendingChangeFlag;

  public:
  RelayManager(SerialManager* SerialManager, uint8_t RelayCount, uint8_t MaxRelayOn, uint16_t MinAddress, uint16_t MaxAddress);
  uint8_t InitRelayData();
  uint8_t AddRelay(uint8_t RelayNum, Relay *NewRelay);
  uint8_t AddRelay(uint8_t RelayNum, volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum, bool LowLevelTrigger = false);
  uint8_t TurnRelayOn(uint8_t RelayNum);
  uint8_t TurnRelayOff(uint8_t RelayNum);
  uint8_t ToggleRelay(uint8_t RelayNum);
  uint8_t TurnAllRelaysOff();
  uint8_t SetRelayName(uint8_t RelayNum, char* Name);
  char* GetRelayName(uint8_t RelayNum);
  uint8_t SetRelayOrder(uint8_t RelayNum, uint8_t Order);
  uint8_t GetRelayOrder(uint8_t RelayNum);
  uint8_t SavePendingChanges();
  bool IsStateStable();
};

#endif // RELAYMANAGER_H