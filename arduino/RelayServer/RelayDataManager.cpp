#include "RelayDataManager.h"

RelayDataManager::RelayDataManager(SerialManager* SerialManager, uint16_t MinAddress, uint16_t MaxAddress, uint8_t RelayCount)
{
  m_pSerialManager = SerialManager;
  m_MinEEPROMAddress = MinAddress;
  m_MaxEEPROMAddress = MaxAddress;
  m_RelayCount = RelayCount;
  m_DataInitialized = false;
  m_pSettingsData = new RelayDataSettings();
  m_pVecRelayData = new RelayData*[m_RelayCount];
  for (uint8_t RelayNum = 0; RelayNum < m_RelayCount; RelayNum++)
  {
    m_pVecRelayData[RelayNum] = new RelayData();
  }

  m_pSerialManager->WriteDebug("RelayDataManager::RelayDataManager - Initialized");
}

uint8_t RelayDataManager::InitRelayData()
{
  m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Initializing Relay Data");
  if (GetStoredRelayDataSettings() != 0)
  {
    m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Failed Reading Relay Data Settings");
    return 1;
  }

  if(m_pSettingsData->DataVersion != DATA_VERSION || m_pSettingsData->RelayNameLen != RELAY_NAME_LEN || m_pSettingsData->RelayCount != m_RelayCount)
  {
    m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Data Not Compatable, Writing Defaults");
    if (SetRelayDataSettings((struct RelayDataSettings){DATA_VERSION, m_RelayCount, RELAY_NAME_LEN}) != 0)
    {
      m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Failed Writing Relay Data Settings");
      return 1;
    }

    for (uint8_t RelayNum = 0; RelayNum < m_RelayCount; RelayNum++)
    {
      if (SetRelayData(RelayNum, ((struct RelayData){DEFAULT_NAME, RelayNum, DEFAULT_VALUE})) != 0)
      {
        m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Failed Writing Relay Data");
        return 1;
      }
    } 
  }
  else
  {
    for (int RelayNum = 0; RelayNum < m_RelayCount; RelayNum++)
    {
      if (GetStoredRelayData(RelayNum) != 0)
      {
        m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Failed Getting Relay Data");
        return 1;
      }
    }
  }
  m_pSerialManager->WriteDebug("RelayDataManager::InitRelayData - Relay Data Initialized");
}

uint8_t RelayDataManager::SetRelayDataSettings(RelayDataSettings Settings)
{
  m_pSerialManager->WriteDebug("RelayDataManager::SetRelayDataSettings - Setting Relay Data Settings");
  uint16_t Addr = m_MinEEPROMAddress;
  if ((Addr >= m_MinEEPROMAddress) && ((Addr + sizeof(struct RelayDataSettings)) <= m_MaxEEPROMAddress))
  {
    EEPROM.put(Addr, Settings);
    if(GetStoredRelayDataSettings() != 0 || m_pSettingsData->DataVersion != Settings.DataVersion || m_pSettingsData->RelayCount != Settings.RelayCount || m_pSettingsData->RelayNameLen != Settings.RelayNameLen)
    {
      m_pSerialManager->WriteDebug("RelayDataManager::SetRelayDataSettings - ERROR! Read Back Failed!");
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::SetRelayDataSettings - ERROR! Address Out Of Range!");
    return 1;
  }
}

uint8_t RelayDataManager::GetStoredRelayDataSettings()
{
  m_pSerialManager->WriteDebug("RelayDataManager::GetStoredRelayDataSettings - Getting Relay Data Settings");
  uint16_t Addr = m_MinEEPROMAddress;
  if ((Addr >= m_MinEEPROMAddress) && ((Addr + sizeof(struct RelayDataSettings)) <= m_MaxEEPROMAddress))
  {
    EEPROM.get(Addr, *m_pSettingsData);
    return 0;
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::GetStoredRelayDataSettings - ERROR! Address Out Of Range!");
    return 1;
  }
}

uint8_t RelayDataManager::SetRelayData(uint8_t RelayNum, RelayData Data)
{
  m_pSerialManager->WriteDebug("RelayDataManager::SetRelayData - Setting Relay Data");
  uint16_t Addr = m_MinEEPROMAddress + sizeof(struct RelayDataSettings) + (RelayNum * sizeof(struct RelayData));
  if ((Addr >= m_MinEEPROMAddress) && ((Addr + sizeof(struct RelayData)) <= m_MaxEEPROMAddress))
  {
    EEPROM.put(Addr, Data);    
    if(GetStoredRelayData(RelayNum) != 0 || strcmp(m_pVecRelayData[RelayNum]->Name, Data.Name) != 0 || m_pVecRelayData[RelayNum]->Order != Data.Order || m_pVecRelayData[RelayNum]->Value != Data.Value)
    {
      m_pSerialManager->WriteDebug("RelayDataManager::SetRelayData - ERROR! Read Back Failed!");
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::SetRelayData - ERROR! Address Out Of Range!");
    return 1;
  }
}

uint8_t RelayDataManager::GetStoredRelayData(uint8_t RelayNum)
{
  m_pSerialManager->WriteDebug("RelayDataManager::GetStoredRelayData - Getting Relay Data");
  uint16_t Addr = m_MinEEPROMAddress + sizeof(struct RelayDataSettings) + (RelayNum * sizeof(struct RelayData));
  if ((Addr >= m_MinEEPROMAddress) && ((Addr + sizeof(struct RelayData)) <= m_MaxEEPROMAddress))
  {
    EEPROM.get(Addr, *m_pVecRelayData[RelayNum]);
    return 0;
  }
  else
  {
    m_pSerialManager->WriteDebug("RelayDataManager::GetStoredRelayData - ERROR! Address Out Of Range!");
    return 1;
  }
}

