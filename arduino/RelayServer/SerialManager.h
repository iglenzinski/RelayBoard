#include <stdint.h>
#include <Arduino.h>

#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

class SerialManager
{
  private:
  bool m_Debug;

  public:
  SerialManager(unsigned long BaudRate, bool Debug = false);
  void WriteString(char* Message);
  void WriteString(char Message);
  void WriteDebug(char* Message);
  uint8_t IsDataAvailable();
  uint8_t ReadByte();
};

#endif // SERIALMANAGER_H