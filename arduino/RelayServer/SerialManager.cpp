#include "SerialManager.h"

SerialManager::SerialManager(unsigned long BaudRate, bool Debug)
{
  m_Debug = Debug;
  Serial.begin(BaudRate);
}

void SerialManager::WriteString(char* Message)
{
  Serial.println(Message);
}

void SerialManager::WriteString(char Message)
{
  Serial.println(Message);
}

void SerialManager::WriteDebug(char* Message)
{
  if (m_Debug)
  {
    Serial.println(Message);
  }
}

bool SerialManager::IsDataAvailable()
{
  if (Serial.available() > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

uint8_t SerialManager::ReadByte()
{
  return Serial.read();
}