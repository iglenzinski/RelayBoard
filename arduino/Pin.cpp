#include "Pin.h"

BasePin::BasePin(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum)
{
  m_DDRReg = DDRReg;
  m_PortReg = PortReg;
  m_PinNum = PinNum;
}

OutputPin::OutputPin(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, uint8_t PinNum) : BasePin(DDRReg, PortReg, PinNum)
{
  *m_DDRReg |= (0x01 << m_PinNum);
  WritePinLow();
}

void OutputPin::WritePinHigh()
{
  *m_PortReg |= (0x01 << m_PinNum);
}

void OutputPin::WritePinLow()
{
  *m_PortReg &= ~(0x01 << m_PinNum);
}

InputPin::InputPin(volatile uint8_t *DDRReg, volatile uint8_t *PortReg, volatile uint8_t *PinReg, uint8_t PinNum) : BasePin(DDRReg, PortReg, PinNum)
{
  m_PinReg = PinReg;
  *m_DDRReg &= ~(0x01 << m_PinNum);
  PullupResistorOn();
}

void InputPin::PullupResistorOn()
{
  *m_PortReg |= (0x01 << m_PinNum);
}

void InputPin::PullupResistorOff()
{
  *m_PortReg &= ~(0x01 << m_PinNum);
}

uint8_t InputPin::ReadPin()
{
  return (*m_PinReg & (0x01 << m_PinNum)) >> m_PinNum;
}