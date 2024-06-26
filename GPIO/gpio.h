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
#include "msp430.h"
#include "../common.h"

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#define GPIO_PORT_1 0U
#define GPIO_PORT_2 1U
#define GPIO_PORT_3 2U

#define GPIO_SET_IN(port, pin)      BIT_CLR(port, pin)
#define GPIO_SET_OUT(port,pin)      BIT_SET(port, pin)
#define GPIO_READ(port, pin)        BIT_CHK(port, pin)
#define GPIO_WRITE_HIGH(port, pin)  BIT_SET(port, pin)
#define GPIO_WRITE_LOW(port, pin)   BIT_CLR(port, pin)
#define GPIO_TOGGLE(port, pin)      BIT_TOG(port, pin)

void GPIO_setModeInput(uint8_t port, uint8_t pin);
void GPIO_setModeInputWithPullUp(uint8_t port, uint8_t pin);
void GPIO_setModeInputWithPullDown(uint8_t port, uint8_t pin);
void GPIO_setModeOutput(uint8_t port, uint8_t pin);
void GPIO_setPortOutput(uint8_t port);
void GPIO_setPortInput(uint8_t port); //TODO: test this function
void GPIO_setDigitalPinLow(uint8_t port, uint8_t pin);
void GPIO_setDigitalPinHigh(uint8_t port, uint8_t pin);
void GPIO_writeOuputPort(uint8_t port, uint8_t value);
void GPIO_toggleDigitalPin(uint8_t port, uint8_t pin);
uint8_t GPIO_getDigitalPort(uint8_t port); //TODO: test this function
bool GPIO_getDigitalPin(uint8_t port, uint8_t pin);
void GPIO_enableInterrupt(uint8_t port, uint8_t pin, bool edge_select);
void GPIO_disableInterrupt(uint8_t port, uint8_t pin);

#endif /* INC_GPIO_H_ */
