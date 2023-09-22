/**
 * @file uart.h
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief
 *
 *
 * @version 0.1
 * @date 2023-09-22
 *
 * @copyright Copyright (c) 2023
 *
 */


#ifndef INC_UART_H_
#define INC_UART_H_

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

#ifdef __cplusplus
extern "C"
{
#endif
#include "../common.h"

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: USCI_A_UART_init().
//
//*****************************************************************************
#define UART_NO_PARITY                                              0x00
#define UART_ODD_PARITY                                             0x01
#define UART_EVEN_PARITY                                            0x02

typedef struct USCI_A_UART_initParam {
    uint8_t selectClockSource;
    uint16_t clockPrescalar;
    uint8_t firstModReg;
    uint8_t secondModReg;
    uint8_t parity;
    uint8_t msborLsbFirst;
    uint8_t numberofStopBits;
    uint8_t uartMode;
    uint8_t overSampling;
} UART_initParam;

#define UART_CLOCKSOURCE_SMCLK                             (BIT6)
#define UART_CLOCKSOURCE_ACLK                              (BIT7)

extern bool UART_init(UART_initParam* param);
extern inline void UART_enable();
extern inline void UART_transmitData(uint8_t data);
#endif /* INC_UART_H_ */
