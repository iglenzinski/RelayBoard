#include <EEPROM.h>
#include "SerialManager.h"

#ifndef RELAYDATAMANAGER_H
#define RELAYDATAMANAGER_H

#define DATA_VERSION 1
#define RELAY_NAME_LEN 100

#define DEFAULT_NAME ""
#define DEFAULT_STATE 0 //RelayState.RELAY_OFF

enum RelayState {
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
  uint8_t State;
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
  uint8_t RetrieveRelayDataSettings();
  uint8_t StoreRelayDataSettings();

  uint8_t RetrieveRelayData();
  
  public:
  RelayDataManager(SerialManager* SerialManager, uint16_t MinAddress, uint16_t MaxAddress, uint8_t RelayCount);
  uint8_t InitRelayData();
  uint8_t SetRelayName(uint8_t RelayNum, char* Name);
  char* GetRelayName(uint8_t RelayNum);
  uint8_t SetRelayOrder(uint8_t RelayNum, uint8_t Order);
  uint8_t GetRelayOrder(uint8_t RelayNum);
  uint8_t SetRelayState(uint8_t RelayNum, RelayState State);
  RelayState GetRelayState(uint8_t RelayNum);

  uint8_t StoreRelayData();
};

#endif // RELAYDATAMANAGER_H