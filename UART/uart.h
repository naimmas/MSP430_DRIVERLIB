/**
 * @file uart.h
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief MSP430 UART modul kutuphanesi
 * @version 0.1
 * @date 2023-09-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once

/*
*  | Address|  Acronym      |  Register Name                 |
*  |--------|---------------|--------------------------------|
*  | 60h    |  UCA0CTL0     |  USCI_A0 control 0             |
*  | 61h    |  UCA0CTL1     |  USCI_A0 control 1             |
*  | 62h    |  UCA0BR0      |  USCI_A0 baud-rate control 0   |
*  | 63h    |  UCA0BR1      |  USCI_A0 baud-rate control 1   |
*  | 64h    |  UCA0MCTL     |  USCI_A0 modulation control    |
*  | 65h    |  UCA0STAT     |  USCI_A0 status                |
*  | 66h    |  UCA0RXBUF    |  USCI_A0 receive buffer        |
*  | 67h    |  UCA0TXBUF    |  USCI_A0 transmit buffer       |
*  | 5Dh    |  UCA0ABCTL    |  USCI_A0 auto baud control     |
*  | 5Eh    |  UCA0IRTCTL   |  USCI_A0 IrDA transmit control |
*  | 5Fh    |  UCA0IRRCTL   |  USCI_A0 IrDA receive control  |
*  | 1h     |  IE2          |  SFR interrupt enable 2        |
*  | 3h     |  IFG2         |  SFR interrupt flag 2          |
*  |---------------------------------------------------------|
*/

/*
*    Initializing or Reconfiguring the USCI Module
*    The recommended USCI initialization or reconfiguration process is:
*    1. Set UCSWRST ( BIS.B #UCSWRST,&UCAxCTL1 )
*    2. Initialize all USCI registers with UCSWRST = 1 (including UCAxCTL1)
*    3. Configure ports.
*    4. Clear UCSWRST via software ( BIC.B #UCSWRST,&UCAxCTL1 )
*    5. Enable interrupts (optional) via UCAxRXIE and/or UCAxTXIE
*/

#include "../common.h"
#include "msp430.h"
//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: USCI_A_UART_init().
//
//*****************************************************************************



typedef enum{
    NO_PARITY = 0,
    ODD_PARITY = UCPEN,
    EVEN_PARITY = UCPEN | UCPAR
}UartParity_t;

typedef enum{
    LSB_FIRST = 0, 
    MSB_FIRST = UCMSB
}UartTxBit_t;

typedef enum{
    DATA_8_BIT = 0,
    DATA_7_BIT = UC7BIT
}UartCharLength_t;

typedef enum{
    STOP_BIT_1 = 0,
    STOP_BIT_2 = UCSPB
}UartStopBit_t;

typedef enum{
    NO_INTERRUPT = 0,
    RX_INTERRUPT = UCA0RXIE,
    TX_INTERRUPT = UCA0TXIE,
    RXTX_INTERRUPT = UCA0RXIE | UCA0TXIE
}UartInterruptMode_t;

typedef enum{
    UART_NORMAL_MODE = UCMODE_0,
    UART_IDLE_MODE = UCMODE_1,
    UART_ADRESS_MODE = UCMODE_2,
    UART_AUTO_BAUD_RATE_MODE = UCMODE_3
}UartMode_t;

typedef enum{
    EXT_CLK = UCSSEL_0,
    AUX_CLK = UCSSEL_1, 
    SM_CLK = UCSSEL_3
}UartClkSource_t;

typedef struct USCI_A_UART_device {
    UartClkSource_t selectClockSource;
    uint16_t clockPrescalar;
    uint8_t firstModReg;
    uint8_t secondModReg;
    UartParity_t parity;
    UartTxBit_t msborLsbFirst;
    UartStopBit_t numberofStopBits;
    UartMode_t uartMode;
    UartCharLength_t uartCharLength;
    uint8_t overSampling;

    void (*enable) ();
    void (*disable) ();
    void (*print) (const char* msg, const uint16_t dataSize);
    unsigned char (*read) ();
    void (*setLoopBack)(uint8_t enable);
    void (*setInterrupt)(UartInterruptMode_t interrupt_mode);
}UartDevice_t;

OperationStatus_t initUART(UartDevice_t* self);

static inline void __uart_enable();
static inline void __uart_disable();
static inline void __uart_loopbackEnable(uint8_t enable);
static void __uart_puts(const char* msg, const uint16_t dataSize);
static inline void __uart_transmitData(unsigned char data);
static uint8_t __uart_receiveData();
static void __uart_enable_interrupt(UartInterruptMode_t interrupt_mode);