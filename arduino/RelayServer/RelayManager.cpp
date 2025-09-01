#include "RelayManager.h"

RelayManager::RelayManager(SerialManager* SerialManager, uint8_t RelayCount, uint8_t MaxRelayOn, uint16_t MinAddress, uint16_t MaxAddress)
{
  m_RelayCount = RelayCount;
  m_MaxRelayOn = MaxRelayOn;
  m_RelayOnCount = 0;
  m_pSerialManager = SerialManager;
  m_pRelayData = new RelayDataManager(SerialManager, MinAddress, MaxAddress, m_RelayCount);
  m_pVecRelays = new Relay*[m_RelayCount];
  
  m_pSerialManager->WriteDebug("RelayManager::RelayManager - Initialized");
}

uint8_t RelayManager::InitRelayData()
{
  return m_pRelayData->InitRelayData();
}

uint8_t RelayManager::AddRelay(uint8_t RelayNum, Relay *NewRelay)
{
  m_pSerialManager->WriteDebug("RelayManager::AddRelay - Starting Add Relay");
  if (RelayNum <= m_RelayCount)
  {
    m_pVecRelays[RelayNum - 1] = NewRelay;
    m_pSerialManager->WriteDebug("RelayManager::AddRelay - Relay Added");
  }  
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::AddRelay - Relay Out Of Range");
  }

  return 0;  
}

uint8_t RelayManager::AddRelay(uint8_t RelayNum, volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum, bool LowLevelTrigger)
{
  return AddRelay(RelayNum, new Relay(DDRReg, PortReg, PinNum, LowLevelTrigger));
}

uint8_t RelayManager::TurnRelayOn(uint8_t RelayNum)
{
  m_pSerialManager->WriteDebug("RelayManager::TurnRelayOn - Starting Turn Relay On");
  if (RelayNum <= m_RelayCount)
  {
    m_pVecRelays[RelayNum - 1]->RelayOn();
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOn - Relay Turned On");
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOn - Relay Out Of Range");
  }

  return 0;
}

uint8_t RelayManager::TurnRelayOff(uint8_t RelayNum)
{
  m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Starting Turn Relay Off");
  if (RelayNum <= m_RelayCount)
  {
    m_pVecRelays[RelayNum - 1]->RelayOff();
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Relay Turned Off");
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Relay Out Of Range");
  }

  return 0;
}

uint8_t RelayManager::ToggleRelay(uint8_t RelayNum)
{
  m_pSerialManager->WriteDebug("RelayManager::ToggleRelay - Starting Relay Toggle");
  if (RelayNum <= m_RelayCount)
  {
    m_pVecRelays[RelayNum - 1]->RelayToggle();
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Relay Toggled");
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Relay Out Of Range");
  }

  return 0;
}