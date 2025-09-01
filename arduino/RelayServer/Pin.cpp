#include "Pin.h"

BasePin::BasePin(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum)
{
  m_pDDRReg = DDRReg;
  m_pPortReg = PortReg;
  m_PinNum = PinNum;
}

OutputPin::OutputPin(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum, bool InitHigh) : BasePin(DDRReg, PortReg, PinNum)
{
  if (InitHigh)
  {
    WritePinHigh();
  }
  else
  {
    WritePinLow();
  }
  *m_pDDRReg |= (0x01 << m_PinNum);
}

void OutputPin::WritePinHigh()
{
  *m_pPortReg |= (0x01 << m_PinNum);
}

void OutputPin::WritePinLow()
{
  *m_pPortReg &= ~(0x01 << m_PinNum);
}

InputPin::InputPin(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, volatile uint8_t *PinReg, uint8_t PinNum) : BasePin(DDRReg, PortReg, PinNum)
{
  m_pPinReg = PinReg;
  *m_pDDRReg &= ~(0x01 << m_PinNum);
  PullupResistorOn();
}

void InputPin::PullupResistorOn()
{
  *m_pPortReg |= (0x01 << m_PinNum);
}

void InputPin::PullupResistorOff()
{
  *m_pPortReg &= ~(0x01 << m_PinNum);
}

uint8_t InputPin::ReadPin()
{
  return (*m_pPinReg & (0x01 << m_PinNum)) >> m_PinNum;
}