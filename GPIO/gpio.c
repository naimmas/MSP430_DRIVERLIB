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

volatile static unsigned char * const GPIO_registerAddresses[] = {
    &P1IN,  //0x0
    &P1OUT, //0x1    
    &P1DIR, //0x2    
    &P1REN, //0x3    

    &P2IN,  //0x4
    &P2OUT, //0x5
    &P2DIR, //0x6
    &P2REN, //0x7

    &P3IN,  //0x8
    &P3OUT, //0x9
    &P3DIR, //0xA
    &P3REN, //0xB

    &P1IE,  //0xC    
    &P1IES, //0xD
    &P2IE,  //0xE
    &P2IES  //0xF
};

inline void GPIO_setModeInput(uint8_t port, uint8_t pin)
{
    GPIO_SET_IN(*GPIO_registerAddresses[(2)+(port<<2)], pin);
}
inline void GPIO_setModeOutput(uint8_t port, uint8_t pin)
{
    GPIO_SET_OUT(*GPIO_registerAddresses[(2)+(port<<2)], pin);
}

inline void GPIO_setModeInputWithPullUp(uint8_t port, uint8_t pin)
{
    GPIO_setModeInput(port, pin);
    BIT_SET(*GPIO_registerAddresses[(3)+(port<<2)], pin);
}
inline void GPIO_setModeInputWithPullDown(uint8_t port, uint8_t pin)
{
    GPIO_setModeInput(port, pin);
    BIT_CLR(*GPIO_registerAddresses[(3)+(port<<2)], pin);
}

inline void GPIO_setDigitalPinLow(uint8_t port, uint8_t pin)
{
    GPIO_WRITE_LOW(*GPIO_registerAddresses[(1)+(port<<2)], pin);
}
inline void GPIO_setDigitalPinHigh(uint8_t port, uint8_t pin)
{
    GPIO_WRITE_HIGH(*GPIO_registerAddresses[(1)+(port<<2)], pin);
}
inline void GPIO_writeOuputPort(uint8_t port, uint8_t value)
{
    *GPIO_registerAddresses[(1)+(port<<2)] = value;
}
inline void GPIO_toggleDigitalPin(uint8_t port, uint8_t pin)
{
    GPIO_TOGGLE(*GPIO_registerAddresses[(1)+(port<<2)], pin);
}
inline uint8_t GPIO_getDigitalPort(uint8_t port)
{
    return *GPIO_registerAddresses[(0)+(port<<2)];
}
inline bool GPIO_getDigitalPin(uint8_t port, uint8_t pin)
{
    return GPIO_READ(*GPIO_registerAddresses[(port<<2)], pin);
}

inline void GPIO_enableInterrupt(uint8_t port, uint8_t pin, bool edge_select)
{
    if(GPIO_PORT_3 == port) return;

    BIT_SET(*GPIO_registerAddresses[12+((port)<<1)], pin);
    BIT_CLR(*GPIO_registerAddresses[13+((port)<<1)], pin);
    *GPIO_registerAddresses[13+((port)<<1)] |= (edge_select << pin);
}

inline void GPIO_disableInterrupt(uint8_t port, uint8_t pin)
{
    if(GPIO_PORT_3 == port) return;

    BIT_CLR(*GPIO_registerAddresses[12+((port)<<1)], pin);
}