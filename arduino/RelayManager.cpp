#include "RelayManager.h"

RelayManager::RelayManager(SerialManager* SerialManager, uint8_t MaxRelayOn)
{
  m_pSerialManager = SerialManager;
  m_MaxRelayOn = MaxRelayOn;
  m_RelayCount = 0;
  m_RelayOnCount = 0;
  m_pSerialManager->WriteDebug("RelayManager::RelayManager - Initialized");
}

uint8_t RelayManager::AddRelay(Relay *NewRelay)
{
  m_pSerialManager->WriteDebug("RelayManager::AddRelay - Starting Add Relay");
  if (m_RelayCount <= MAX_RELAY_COUNT)
  {
    m_pVecRelays[m_RelayCount] = NewRelay;
    m_RelayCount++;
    m_pSerialManager->WriteDebug("RelayManager::AddRelay - Relay Added");
  }  
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::AddRelay - Cannot Exceed Max Relay Number");
  }

  return 0;  
}

uint8_t RelayManager::AddRelay(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum, RelayState State)
{
  m_pSerialManager->WriteDebug("RelayManager::AddRelay - Starting Add Relay");
  if (m_RelayCount <= MAX_RELAY_COUNT)
  {
    m_pVecRelays[m_RelayCount] = new Relay(DDRReg, PortReg, PinNum, State);
    m_RelayCount++;
    m_pSerialManager->WriteDebug("RelayManager::AddRelay - Relay Added");
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::AddRelay - Cannot Exceed Max Relay Number");
  }

  return 0;  
}

uint8_t RelayManager::TurnRelayOn(uint8_t RelayNumber)
{
  m_pSerialManager->WriteDebug("RelayManager::TurnRelayOn - Starting Turn Relay On");
  if (RelayNumber <= m_RelayCount)
  {
    m_pVecRelays[RelayNumber - 1]->RelayOn();
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOn - Relay Turned On");
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOn - Relay Out Of Range");
  }

  return 0;
}

uint8_t RelayManager::TurnRelayOff(uint8_t RelayNumber)
{
  m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Starting Turn Relay Off");
  if (RelayNumber <= m_RelayCount)
  {
    m_pVecRelays[RelayNumber - 1]->RelayOff();
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Relay Turned Off");
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Relay Out Of Range");
  }

  return 0;
}