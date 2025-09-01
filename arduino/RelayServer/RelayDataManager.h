#include <EEPROM.h>
#include "SerialManager.h"

#ifndef RELAYDATAMANAGER_H
#define RELAYDATAMANAGER_H

#define DATA_VERSION 2
#define RELAY_NAME_LEN 100

#define DEFAULT_NAME ""
#define DEFAULT_VALUE 0 //StoredRelayState.RELAY_OFF

enum StoredRelayState {
  RELAY_OFF,
  RELAY_ON,
  RELAY_DISABLED
};

struct RelayDataSettings
{
  uint8_t DataVersion;
  uint8_t RelayCount;
  uint8_t RelayNameLen;
};

struct RelayData
{
  char Name[RELAY_NAME_LEN];
  uint8_t Order;
  uint8_t Value;
};

class RelayDataManager
{
  private:
  uint16_t m_MinEEPROMAddress;
  uint16_t m_MaxEEPROMAddress;
  uint8_t m_RelayCount;
  bool m_DataInitialized;
  SerialManager *m_pSerialManager;
  RelayDataSettings *m_pSettingsData;
  RelayData **m_pVecRelayData;
  uint8_t SetRelayDataSettings(RelayDataSettings Settings);
  uint8_t GetStoredRelayDataSettings();
  uint8_t SetRelayData(uint8_t RelayNum, RelayData Data);
  uint8_t GetStoredRelayData(uint8_t RelayNum);

  public:
  RelayDataManager(SerialManager* SerialManager, uint16_t MinAddress, uint16_t MaxAddress, uint8_t RelayCount);
  uint8_t InitRelayData();
};

#endif // RELAYDATAMANAGER_H