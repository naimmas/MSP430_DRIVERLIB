/**
 * @file spi.c
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-11-22
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "spi.h"
uint8_t _cs_port;
uint8_t _cs_pin;
void initSPI(chipSelect_t *cs_pin, SpiClkSource_t clk_src, uint16_t clk_prescaler)
{
    SPC_BIT_SET(P1SEL, BIT5 | BIT6 | BIT7);
    SPC_BIT_SET(P1SEL2, BIT5 | BIT6 | BIT7);
    GPIO_setModeOutput(cs_pin->port, cs_pin->pin);
    GPIO_setDigitalPinHigh(cs_pin->port, cs_pin->pin);
    _cs_port = cs_pin->port;
    _cs_pin = cs_pin->pin;
    SPC_BIT_SET(UCB0CTL1, UCSWRST);

    SPC_BIT_SET(UCB0CTL0, UCMST | UCSYNC | UCCKPH | UCMSB);
    UCB0CTL1 |= clk_src;
    UCB0BR0 = GET_LOW_BYTE(clk_prescaler);
    UCB0BR1 = GET_HIGH_BYTE(clk_prescaler);

    SPC_BIT_CLR(UCB0CTL1, UCSWRST);
}
void SPI_EnableInterrupt(SpiInterruptMode_t ie_mode)
{
    __disable_interrupt();
    SPC_BIT_CLR(IE2, UCB0TXIE | UCB0RXIE);
    SPC_BIT_SET(IE2, ie_mode);
    __enable_interrupt();
}

uint8_t SPI_WriteByte(uint8_t data)
{
    GPIO_setDigitalPinLow(_cs_port, _cs_pin);

    uint8_t ret_val = 0;
    UCB0TXBUF = data;
    while (!(IFG2 & UCB0TXIFG));
    while (UCB0STAT & UCBUSY);
    ret_val = UCB0RXBUF;

    GPIO_setDigitalPinHigh(_cs_port, _cs_pin);
    return ret_val;
}

uint8_t SPI_ReadByte(uint8_t dummy_char)
{
    return SPI_WriteByte(dummy_char);
}

void SPI_WriteBuffer(uint8_t *buf, uint16_t size)
{
    GPIO_setDigitalPinLow(_cs_port, _cs_pin);
    int16_t i = 0;
    volatile uint8_t tmpVar;

    for (i = size - 1; i >= 0; --i)
    {
        UCB0TXBUF = buf[i];

        // Wait for the TX buffer to be ready
        while (!SPC_BIT_CHK(IFG2, UCB0TXIFG))
            ;
    }

    // Wait for the transaction to complete
    while (SPC_BIT_CHK(UCB0STAT, UCBUSY))
        ;

    tmpVar = UCB0RXBUF;
    GPIO_setDigitalPinHigh(_cs_port, _cs_pin);
}

void SPI_ReadBuffer(uint8_t *buf, uint16_t size, uint8_t dummy_char)
{
    uint16_t i = 0;
    GPIO_setDigitalPinLow(_cs_port, _cs_pin);
    for (i = 0; i < size; i++)
    {
        // Write the dummy char to the TXBUF
        UCB0TXBUF = dummy_char;
        // Wait for the transaction to complete
        while (!SPC_BIT_CHK(IFG2, UCB0RXIFG))
            ;
        // Store the received value in the buffer
        buf[i] = UCB0RXBUF;
    }
    GPIO_setDigitalPinHigh(_cs_port, _cs_pin);
}
