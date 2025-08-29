#include "Constants.h"
#include <Arduino.h>
#include "avr/io.h"

#ifndef ERROR_H
#define ERROR_H


#define ERROR_INVALID_COMMAND 10
#define ERROR_INVALID_SIZE 11

#define ERROR_INIT_SERIAL 1
#define ERROR_INIT_EEPROM 2
#define ERROR_INIT_RESERVED_3 3
#define ERROR_INIT_RESERVED_4 4
#define ERROR_INIT_RESERVED_5 5
#define ERROR_INIT_RESERVED_6 6
#define ERROR_INIT_RESERVED_7 7
#define ERROR_INIT_RESERVED_8 8
#define ERROR_INIT_RESERVED_9 9

class ErrorModule {
public:
  ErrorModule();
  void BlinkInitErrorLoop(int ErrorCode);
};

#endif // ERROR_H