#include "RelayManager.h"

////////////////////////////////////////////////////////////////////////////
RelayManager::RelayManager(SerialManager* SerialManager, uint8_t RelayCount, uint8_t MaxRelayOn, uint16_t MinAddress, uint16_t MaxAddress)
{
  m_RelayCount = RelayCount;
  m_MaxRelayOn = MaxRelayOn;
  m_RelayOnCount = 0;
  m_pSerialManager = SerialManager;
  m_pRelayData = new RelayDataManager(SerialManager, MinAddress, MaxAddress, m_RelayCount);
  m_pVecRelays = new Relay*[m_RelayCount];

  m_StabilizationTimeout = STABLE_TIMEOUT;
  m_LastChangeTime = 0;
  m_PendingChangeFlag = false;
  
  m_pSerialManager->WriteDebug("RelayManager::RelayManager - Initialized");
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::InitRelayData()
{
  return m_pRelayData->InitRelayData();
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::AddRelay(uint8_t RelayNum, Relay *NewRelay)
{
  m_pSerialManager->WriteDebug("RelayManager::AddRelay - Starting Add Relay");
  if (RelayNum <= m_RelayCount)
  {
    if (m_pRelayData->GetRelayState(RelayNum) == RelayState::RELAY_ON)
    {
      NewRelay->RelayOn();
      m_RelayOnCount++;
    }
    else
    {
      NewRelay->RelayOff();
    }
    m_pVecRelays[RelayNum - 1] = NewRelay;
    m_pSerialManager->WriteDebug("RelayManager::AddRelay - Relay Added");
  }  
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::AddRelay - Relay Out Of Range");
  }

  return 0;  
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::AddRelay(uint8_t RelayNum, volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum, bool LowLevelTrigger)
{
  return AddRelay(RelayNum, new Relay(DDRReg, PortReg, PinNum, LowLevelTrigger));
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::TurnRelayOn(uint8_t RelayNum)
{
  m_pSerialManager->WriteDebug("RelayManager::TurnRelayOn - Starting Turn Relay On");
  if (RelayNum <= m_RelayCount)
  {
    if (m_RelayOnCount < m_MaxRelayOn)
    {
      m_pVecRelays[RelayNum - 1]->RelayOn();
      m_pSerialManager->WriteDebug("RelayManager::TurnRelayOn - Relay Turned On");
      m_RelayOnCount++;
      m_pRelayData->SetRelayState(RelayNum, RelayState::RELAY_ON);
      m_PendingChangeFlag = true;
      m_LastChangeTime = millis();
      return 0;
    }
    else
    {
      m_pSerialManager->WriteDebug("RelayManager::TurnRelayOn - Max Relays Already On");
      return 1;
    }
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOn - Relay Out Of Range");
    return 1;
  }
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::TurnRelayOff(uint8_t RelayNum)
{
  m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Starting Turn Relay Off");
  if (RelayNum <= m_RelayCount)
  {
    if (m_pVecRelays[RelayNum - 1]->IsRelayOn())
    {
      m_pVecRelays[RelayNum - 1]->RelayOff();
      m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Relay Turned Off");
      m_RelayOnCount--;
      m_pRelayData->SetRelayState(RelayNum, RelayState::RELAY_OFF);
      m_PendingChangeFlag = true;
      m_LastChangeTime = millis();
    }
    return 0;
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::TurnRelayOff - Relay Out Of Range");
    return 1;
  }
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::ToggleRelay(uint8_t RelayNum)
{
  m_pSerialManager->WriteDebug("RelayManager::ToggleRelay - Starting Relay Toggle");
  if (RelayNum <= m_RelayCount)
  {
    if (m_pVecRelays[RelayNum - 1]->IsRelayOn())
    {
      return TurnRelayOff(RelayNum);
    }
    else
    {
      return TurnRelayOn(RelayNum);
    }
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayManager::ToggleRelay - Relay Out Of Range");
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::TurnAllRelaysOff()
{
  for (int RelayNum = 0; RelayNum < m_RelayCount; RelayNum++)
  {
    TurnRelayOff(RelayNum);
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::SetRelayName(uint8_t RelayNum, char* Name)
{
  m_PendingChangeFlag = true;
  m_LastChangeTime = millis();
  return m_pRelayData->SetRelayName(RelayNum, Name);
}

////////////////////////////////////////////////////////////////////////////
char* RelayManager::GetRelayName(uint8_t RelayNum)
{
  return m_pRelayData->GetRelayName(RelayNum);
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::SetRelayOrder(uint8_t RelayNum, uint8_t Order)
{
  m_PendingChangeFlag = true;
  m_LastChangeTime = millis();
  return m_pRelayData->SetRelayOrder(RelayNum, Order);
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::GetRelayOrder(uint8_t RelayNum)
{
  return m_pRelayData->GetRelayOrder(RelayNum);
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayManager::SavePendingChanges()
{
  if (IsStateStable())
  {
    m_PendingChangeFlag = false;
    return m_pRelayData->StoreRelayData();
  }
  else
  {
    return 0;
  }
}

////////////////////////////////////////////////////////////////////////////
bool RelayManager::IsStateStable()
{
  if (!m_PendingChangeFlag)
  {
    return false;
  }
  else
  {
    uint32_t CurrentMillis = millis();
    //Checks if changes have become stable handeling the overflow/reset state for millis() function
    if (0xFFFFFFFF - m_LastChangeTime < m_StabilizationTimeout)
    {
      return (((m_StabilizationTimeout - (0xFFFFFFFF - m_LastChangeTime)) < CurrentMillis) && (CurrentMillis < m_StabilizationTimeout));
    }
    else
    {
      return (((m_LastChangeTime + m_StabilizationTimeout) < CurrentMillis) || (CurrentMillis < m_LastChangeTime));
    }
  }
}