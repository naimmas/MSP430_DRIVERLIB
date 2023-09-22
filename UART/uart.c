/**
 * @file uart.c
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief MSP430 UART modul kutuphanesi
 * @version 0.1
 * @date 2023-09-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "uart.h"
bool UART_init(UART_initParam* param)
{

    // USCI modulunu devre disi birakmak
    SPC_BIT_SET(UCA0CTL1, UCSWRST);

    switch (param->parity)
    {
        case UART_NO_PARITY:
            SPC_BIT_CLR(UCA0CTL0, UCPEN);
            break;
        case UART_ODD_PARITY:
            SPC_BIT_SET(UCA0CTL0, UCPEN);
            SPC_BIT_CLR(UCA0CTL0, UCPAR);
            break;
        case UART_EVEN_PARITY:
            SPC_BIT_SET(UCA0CTL0, UCPEN);
            SPC_BIT_SET(UCA0CTL0, UCPAR);
            break;
        default:
            return STATUS_FAIL;
    }
    SPC_BIT_CLR(UCA0CTL0, UCMSB);
    UCA0CTL0 |= param->msborLsbFirst;

    SPC_BIT_CLR(UCA0CTL0, UC7BIT);

    SPC_BIT_CLR(UCA0CTL0, UCSPB);
    UCA0CTL0 |= param->numberofStopBits;

    SPC_BIT_CLR(UCA0CTL0, UCMODE0);
    UCA0CTL0 |= param->uartMode;

    SPC_BIT_CLR(UCA0CTL0, UCSYNC);

    SPC_BIT_SET(UCA0CTL1, UCSSEL_3);
    SPC_BIT_CLR(UCA0CTL1, param->selectClockSource);

    SPC_BIT_CLR(UCA0CTL1, UCRXEIE + UCBRKIE + UCDORM + UCTXADDR + UCTXBRK);

    UCA0BR0 = GET_LOW_BYTE(param->clockPrescalar);
    UCA0BR1 = GET_HIGH_BYTE(param->clockPrescalar);

    UCA0MCTL = ((param->firstModReg << 4) +
                (param->secondModReg << 1) +
                (param->overSampling   ));
    return STATUS_SUCCESS;
}
inline void UART_enable()
{
    SPC_BIT_CLR(UCA0CTL1, UCSWRST);
}
inline void UART_disable()
{
    SPC_BIT_SET(UCA0CTL1, UCSWRST);
}
inline void UART_setDormant()
{
    SPC_BIT_SET(UCA0CTL1, UCDORM);
}
inline void UART_resetDormant()
{
    SPC_BIT_CLR(UCA0CTL1, UCDORM);
}
inline void UART_loopbackEnable()
{
    SPC_BIT_SET(UCA0STAT, UCLISTEN);
}
inline void UART_loopbackDisable()
{
    SPC_BIT_CLR(UCA0STAT, UCLISTEN);
}
inline void UART_transmitData(uint8_t data)
{
    if(SPC_BIT_CHK(IE2, UCA0TXIE))
    {
        while(SPC_BIT_CHK(IFG2, UCA0TXIFG));
    }
    while(SPC_BIT_CHK(UCA0STAT, UCBUSY));
    UCA0TXBUF = data;
}
uint8_t UART_receiveData()
{
    if(SPC_BIT_CHK(IE2, UCA0RXIE))
    {
        while(SPC_BIT_CHK(IFG2, UCA0RXIFG));
    }
    while(SPC_BIT_CHK(UCA0STAT, UCBUSY));
    return UCA0RXBUF;
}
