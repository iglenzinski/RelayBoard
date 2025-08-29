#include "Constants.h"
#include <stdint.h>

#ifndef PIN_H
#define PIN_H

class BasePin
{
  protected:
  volatile uint8_t *m_DDRReg;
  volatile uint8_t *m_PortReg;
  uint8_t m_PinNum;
  
  BasePin(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum);
};

class OutputPin : BasePin
{
  protected:
  OutputPin(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum);
  void WritePinHigh();
  void WritePinLow();
};

class InputPin : BasePin
{
  protected:
  volatile uint8_t *m_PinReg;
  
  InputPin(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, volatile uint8_t *PinReg, uint8_t PinNum);
  void PullupResistorOn();
  void PullupResistorOff();
  uint8_t ReadPin();
};

#endif // PIN_H