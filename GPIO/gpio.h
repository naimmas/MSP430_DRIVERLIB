/**
 * @file gpio.h
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief MSP430 GPIO modul kutuphanesi
 * @version 0.1
 * @date 2023-09-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "../common.h"

#ifndef INC_GPIO_H_
#define INC_GPIO_H_
#ifdef __cplusplus
extern "C" {
#endif
#define GPIO_PORT_1 0
#define GPIO_PORT_2 1
#define GPIO_PORT_3 2

#define GPIO_SET_IN(port, pin)      BIT_CLR(port, pin)
#define GPIO_SET_OUT(port,pin)      BIT_SET(port, pin)
#define GPIO_READ(port, pin)        BIT_CHK(port, pin)
#define GPIO_WRITE_HIGH(port, pin)  BIT_SET(port, pin)
#define GPIO_WRITE_LOW(port, pin)   BIT_CLR(port, pin)
#define GPIO_RES_PDN(port, pin)  { \
            GPIO_SET_IN(port,pin); \
            BIT_SET(port, pin); \
            BIT_CLR(port, pin);}
#define GPIO_RES_PUP(port, pin)  { \
            GPIO_SET_IN(port,pin); \
            BIT_SET(port, pin); \
            BIT_SET(port, pin);}

volatile unsigned char * const GPIO_registerAddresses[] = {
    &P1OUT,
    &P1DIR,
    &P1REN,
    &P2OUT,
    &P2DIR,
    &P2REN,
    &P3OUT,
    &P3DIR,
    &P3REN
};

inline void GPIO_setModeInput(uint8_t port, uint8_t pin)
{
    GPIO_SET_IN(*GPIO_registerAddresses[(port+1)+(port<<1)], pin);
}
inline void GPIO_setModeInputWithPullUp(uint8_t port, uint8_t pin)
{
    GPIO_RES_PUP(*GPIO_registerAddresses[(port+1)+(port<<1)], pin);
}
inline void GPIO_setModeInputWithPullDown(uint8_t port, uint8_t pin)
{
    GPIO_RES_PDN(*GPIO_registerAddresses[(port+1)+(port<<1)], pin);
}
inline void GPIO_setModeOutput(uint8_t port, uint8_t pin)
{
    GPIO_SET_OUT(*GPIO_registerAddresses[(port+1)+(port<<1)], pin);
}
inline void GPIO_setDigitalPinLow(uint8_t port, uint8_t pin)
{
    GPIO_WRITE_LOW(*GPIO_registerAddresses[(port)+(port<<1)], pin);
}
inline void GPIO_setDigitalPinHigh(uint8_t port, uint8_t pin)
{
    GPIO_WRITE_HIGH(*GPIO_registerAddresses[(port)+(port<<1)], pin);
}

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* INC_GPIO_H_ */
