/**
 * @file gpio.cpp
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "gpio.h"

void GPIO_setModeInput(uint8_t port, uint8_t pin)
{
    GPIO_SET_IN(*GPIO_registerAddresses[(port+1)+(port<<1)], pin);
}
void GPIO_setModeInputWithPullUp(uint8_t port, uint8_t pin)
{
    GPIO_RES_PUP(*GPIO_registerAddresses[(port+1)+(port<<1)], pin);
}
void GPIO_setModeInputWithPullDown(uint8_t port, uint8_t pin)
{
    GPIO_RES_PDN(*GPIO_registerAddresses[(port+1)+(port<<1)], pin);
}
void GPIO_setModeOutput(uint8_t port, uint8_t pin)
{
    GPIO_SET_OUT(*GPIO_registerAddresses[(port+1)+(port<<1)], pin);
}
void GPIO_setDigitalPinLow(uint8_t port, uint8_t pin)
{
    GPIO_WRITE_LOW(*GPIO_registerAddresses[(port)+(port<<1)], pin);
}
void GPIO_setDigitalPinHigh(uint8_t port, uint8_t pin)
{
    GPIO_WRITE_HIGH(*GPIO_registerAddresses[(port)+(port<<1)], pin);
}