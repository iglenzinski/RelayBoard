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
  void WriteByte(uint8_t Byte);
  void WriteBytes(uint8_t Byte);
  void WriteDebug(char* Message);
  void WriteDebug(uint32_t Message);
  bool IsDataAvailable(uint8_t Bytes);
  uint8_t ReadByte();
  uint8_t ReadBytes();
};

#endif // SERIALMANAGER_H