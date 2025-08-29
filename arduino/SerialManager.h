#include "Constants.h"
#include <stdint.h>
#include <Arduino.h>

#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

class SerialManager
{
  public:
  SerialManager(unsigned long BaudRate);
  void WriteString(char* Message);
  void WriteString(char Message);
  void WriteDebug(char* Message);
};

#endif // SERIALMANAGER_H