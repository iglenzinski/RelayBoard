#include "RelayDataManager.h"

////////////////////////////////////////////////////////////////////////////
RelayDataManager::RelayDataManager(SerialManager* SerialManager, uint16_t MinAddress, uint16_t MaxAddress, uint8_t RelayCount)
{
  m_pSerialManager = SerialManager;
  m_MinEEPROMAddress = MinAddress;
  m_MaxEEPROMAddress = MaxAddress;
  m_RelayCount = RelayCount;
  m_DataInitialized = false;
  m_pSettingsData = new RelayDataSettings();
  m_pVecRelayData = new RelayData*[m_RelayCount];
  for (uint8_t RelayIndex = 0; RelayIndex < m_RelayCount; RelayIndex++)
  {
    m_pVecRelayData[RelayIndex] = new RelayData();
  }

  m_pSerialManager->WriteDebug("RelayDataManager::RelayDataManager - Initialized");
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayDataManager::InitRelayData()
{
  m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Initializing Relay Data");
  if (RetrieveRelayDataSettings() != 0)
  {
    m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Failed Reading Relay Data Settings");
    return 1;
  }

  if(m_pSettingsData->DataVersion != DATA_VERSION || m_pSettingsData->RelayNameLen != RELAY_NAME_LEN || m_pSettingsData->RelayCount != m_RelayCount)
  {
    m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Data Not Compatable, Writing Defaults");
    m_pSettingsData->DataVersion = DATA_VERSION;
    m_pSettingsData->RelayCount = m_RelayCount;
    m_pSettingsData->RelayNameLen = RELAY_NAME_LEN;
    if (StoreRelayDataSettings() != 0)
    {
      m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Failed Writing Relay Data Settings");
      return 1;
    }

    for (uint8_t RelayNum = 1; RelayNum <= m_RelayCount; RelayNum++)
    {
      if (SetRelayName(RelayNum, DEFAULT_NAME) || SetRelayOrder(RelayNum, RelayNum-1) || SetRelayState(RelayNum, (RelayState)DEFAULT_STATE))
      {
        m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Failed Setting Relay Data");
        return 1;
      }
    }

    if (StoreRelayData() != 0)
    {
      m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Failed Writing Relay Data");
      return 1;
    }
  }
  else
  {
    if(retrieveRelayData() != 0)
    {
      m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Failed Getting Relay Data");
      return 1;
    }
  }
  m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Relay Data Initialized");
  return 0;
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayDataManager::StoreRelayDataSettings()
{
  m_pSerialManager->WriteDebug("RelayDataManager::StoreRelayDataSettings - Setting Relay Data Settings");
  uint16_t Addr = m_MinEEPROMAddress;
  if ((Addr >= m_MinEEPROMAddress) && ((Addr + sizeof(struct RelayDataSettings)) <= m_MaxEEPROMAddress))
  {
    RelayDataSettings Settings = *m_pSettingsData;
    EEPROM.put(Addr, Settings);
    if(GetStoredRelayDataSettings() != 0 || m_pSettingsData->DataVersion != Settings.DataVersion || m_pSettingsData->RelayCount != Settings.RelayCount || m_pSettingsData->RelayNameLen != Settings.RelayNameLen)
    {
      m_pSerialManager->WriteDebug("RelayDataManager::StoreRelayDataSettings - ERROR! Read Back Failed!");
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::StoreRelayDataSettings - ERROR! Address Out Of Range!");
    return 1;
  }
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayDataManager::RetrieveRelayDataSettings()
{
  m_pSerialManager->WriteDebug("RelayDataManager::RetrieveRelayDataSettings - Getting Relay Data Settings");
  uint16_t Addr = m_MinEEPROMAddress;
  if ((Addr >= m_MinEEPROMAddress) && ((Addr + sizeof(struct RelayDataSettings)) <= m_MaxEEPROMAddress))
  {
    EEPROM.get(Addr, *m_pSettingsData);
    return 0;
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::RetrieveRelayDataSettings - ERROR! Address Out Of Range!");
    return 1;
  }
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayDataManager::StoreRelayData(uint8_t RelayNum, RelayData Data)
{
  for (uint8_t RelayIndex = 0; RelayIndex < m_RelayCount; RelayIndex++)
  {
    uint16_t Addr = m_MinEEPROMAddress + sizeof(struct RelayDataSettings) + (RelayIndex * sizeof(struct RelayData));
    if ((Addr >= m_MinEEPROMAddress) && ((Addr + sizeof(struct RelayData)) <= m_MaxEEPROMAddress))
    {
      EEPROM.put(Addr, *m_pVecRelayData[RelayIndex]);
      
      // TURNING OFF READBACK FOR NOW UNTIL IT CAN BE DONE PER RELAY AGAIN!
      // RelayData Data = *m_pVecRelayData[RelayIndex];
      // EEPROM.put(Addr, Data);
      // if(RetrieveRelayData() != 0 || strcmp(m_pVecRelayData[RelayIndex]->Name, Data.Name) != 0 || m_pVecRelayData[RelayIndex]->Order != Data.Order || m_pVecRelayData[RelayIndex]->State != Data.State)
      // {
      //   m_pSerialManager->WriteDebug("RelayDataManager::StoreRelayData - ERROR! Read Back Failed!");
      //   return 1;
      // }
    }
    else
    {
      m_pSerialManager->WriteDebug("RelayDataManager::StoreRelayData - ERROR! Address Out Of Range!");
      return 1;
    }
  }
  m_pSerialManager->WriteDebug("RelayDataManager::StoreRelayData - Relay Data Stored Successfully");
  return 0;
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayDataManager::RetrieveRelayData()
{
  m_pSerialManager->WriteDebug("RelayDataManager::RetrieveRelayData - Getting Relay Data");
  uint16_t Addr = m_MinEEPROMAddress + sizeof(struct RelayDataSettings) + (RelayNum * sizeof(struct RelayData));
  if ((Addr >= m_MinEEPROMAddress) && ((Addr + sizeof(struct RelayData)) <= m_MaxEEPROMAddress))
  {
    EEPROM.get(Addr, *m_pVecRelayData[RelayNum]);
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::RetrieveRelayData - ERROR! Address Out Of Range!");
    return 1;
  }
  m_pSerialManager->WriteDebug("RelayDataManager::RetrieveRelayData - Relay Data Retrieved Successfully");
  return 0;
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayDataManager::SetRelayName(uint8_t RelayNum, char* Name)
{
  m_pSerialManager->WriteDebug("RelayDataManager::SetRelayName - Setting Relay Name");
  if (RelayNum <= m_RelayCount)
  {
    strcpy(m_pVecRelayData[RelayNum - 1]->Name, Name);
    m_pSerialManager->WriteDebug("RelayDataManager::SetRelayName - New Name Set");
    return 0;
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::SetRelayName - Relay Out Of Range");
    return 1;
  }
}

////////////////////////////////////////////////////////////////////////////
char* RelayDataManager::GetRelayName(uint8_t RelayNum)
{
  m_pSerialManager->WriteDebug("RelayDataManager::GetRelayName - Getting Relay Name")
  if (RelayNum <= m_RelayCount)
  {
    return m_pVecRelayData[RelayNum - 1]->Name;
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::GetRelayName - Relay Out Of Range");
    return "";    
  }
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayDataManager::SetRelayOrder(uint8_t RelayNum, uint8_t Order)
{
  m_pSerialManager->WriteDebug("RelayDataManager::SetRelayOrder - Setting Relay Order");
  if (RelayNum <= m_RelayCount)
  {
    m_pVecRelayData[RelayNum - 1]->Order, Order;
    m_pSerialManager->WriteDebug("RelayDataManager::SetRelayOrder - New Order Set");
    return 0;
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::SetRelayOrder - Relay Out Of Range");
    return 1;
  }
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayDataManager::GetRelayOrder(uint8_t RelayNum)
{
  m_pSerialManager->WriteDebug("RelayDataManager::GetRelayOrder - Getting Relay Order")
  if (RelayNum <= m_RelayCount)
  {
    return m_pVecRelayData[RelayNum - 1]->Order;
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::GetRelayOrder - Relay Out Of Range");
    return 0xFF;    
  }
}

////////////////////////////////////////////////////////////////////////////
uint8_t RelayDataManager::SetRelayState(uint8_t RelayNum, RelayState State)
{
  m_pSerialManager->WriteDebug("RelayDataManager::SetRelayState - Setting Relay State");
  if (RelayNum <= m_RelayCount)
  {
    m_pVecRelayData[RelayNum - 1]->State = (uint8_t)State;
    m_pSerialManager->WriteDebug("RelayDataManager::SetRelayState - New State Set");
    return 0;
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::SetRelayState - Relay Out Of Range");
    return 1;
  }
}

////////////////////////////////////////////////////////////////////////////
RelayState RelayDataManager::GetRelayState(uint8_t RelayNum)
{
  m_pSerialManager->WriteDebug("RelayDataManager::GetRelayState - Getting Relay State")
  if (RelayNum <= m_RelayCount)
  {
    return (RelayState)m_pVecRelayData[RelayNum - 1]->State;
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::GetRelayState - Relay Out Of Range");
    return 0xFF;    
  }
}