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

/**
 * @brief Setting UART device settings to default
 * @note UART baud rate calcualtions for 16Mhz clock & 9600bps speed
 * @param self UARTs device structures address
 */
void defaultBasicClock(UartDevice_t *self){
    self->selectClockSource = SM_CLK;
    //* For 16Mhz clock with 9600bps
    self->clockPrescalar = 104;
    self->firstModReg = 3;
    self->secondModReg = 0;
    self->overSampling = 1;
    self->parity = NO_PARITY;
    self->msborLsbFirst = LSB_FIRST;
    self->numberofStopBits = STOP_BIT_1;
    self->uartMode = UART_NORMAL_MODE;
    self->uartCharLength = DATA_8_BIT;
}

OperationStatus_t initUART(UartDevice_t *self)
{

    // USCI modulunu devre disi birakmak
    __uart_disable();
    P1SEL = BIT1 + BIT2;  // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2; // P1.1 = RXD, P1.2=TXD
    SPC_BIT_CLR(UCA0CTL0, UCMSB);
    SPC_BIT_SET(UCA0CTL0, self->msborLsbFirst);

    SPC_BIT_CLR(UCA0CTL0, UC7BIT);
    SPC_BIT_SET(UCA0CTL0, self->uartCharLength);

    SPC_BIT_CLR(UCA0CTL0, UCSPB);
    SPC_BIT_SET(UCA0CTL0, self->numberofStopBits);

    SPC_BIT_CLR(UCA0CTL0, UCMODE0 | UCMODE1);
    SPC_BIT_SET(UCA0CTL0, self->uartMode);

    SPC_BIT_CLR(UCA0CTL0, UCSYNC);

    SPC_BIT_CLR(UCA0CTL1, UCSSEL0 | UCSSEL1);
    SPC_BIT_SET(UCA0CTL1, self->selectClockSource);

    SPC_BIT_CLR(UCA0CTL1, UCRXEIE + UCBRKIE + UCDORM + UCTXADDR + UCTXBRK);

    UCA0BR0 = GET_LOW_BYTE(self->clockPrescalar);
    UCA0BR1 = GET_HIGH_BYTE(self->clockPrescalar);

    UCA0MCTL = ((self->firstModReg << 4) +
                (self->secondModReg << 1) +
                (self->overSampling));
    SPC_BIT_CLR(UCA0CTL0, UCPEN | UCPAR);
    SPC_BIT_SET(UCA0CTL0, self->parity);

    self->enable = __uart_enable;
    self->disable = __uart_disable;
    self->print = __uart_puts;
    self->read = __uart_receiveData;
    self->setLoopBack = __uart_loopbackEnable;
    self->setInterrupt = __uart_enable_interrupt;
    __uart_enable();
    return STATUS_SUCCESS;
}

static inline void __uart_enable()
{
    SPC_BIT_CLR(UCA0CTL1, UCSWRST);
}
static inline void __uart_disable()
{
    SPC_BIT_SET(UCA0CTL1, UCSWRST);
}
static inline void __uart_loopbackEnable(uint8_t enable)
{
    SPC_BIT_CLR(UCA0STAT, UCLISTEN);
    SPC_BIT_SET(UCA0STAT, enable << 7);
}
static void __uart_puts(const char *msg, const uint16_t dataSize)
{
    uint16_t sz = dataSize;
    while (sz)
    {
        __uart_transmitData(*msg++);
        sz--;
    }
}
static inline void __uart_transmitData(unsigned char data)
{
    if (SPC_BIT_CHK(IE2, UCA0TXIE))
    {
        while (SPC_BIT_CHK(IFG2, UCA0TXIFG))
            ;
    }
    while (SPC_BIT_CHK(UCA0STAT, UCBUSY))
        ;
    UCA0TXBUF = data;
}
static uint8_t __uart_receiveData()
{
    if (SPC_BIT_CHK(IE2, UCA0RXIE))
    {
        while (SPC_BIT_CHK(IFG2, UCA0RXIFG))
            ;
    }
    while (SPC_BIT_CHK(UCA0STAT, UCBUSY))
        ;
    return UCA0RXBUF;
}

static void __uart_enable_interrupt(UartInterruptMode_t interrupt_mode)
{
    __disable_interrupt();
    SPC_BIT_CLR(IE2, UCA0TXIE | UCA0RXIE);
    SPC_BIT_SET(IE2, interrupt_mode);
    __enable_interrupt();
}