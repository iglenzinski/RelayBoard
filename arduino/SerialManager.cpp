#include "SerialManager.h"

SerialManager::SerialManager(unsigned long BaudRate)
{
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
  #ifdef DEBUG
    Serial.println(Message);
  #endif
}