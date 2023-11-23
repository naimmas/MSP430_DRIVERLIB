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

void initSPI(chipSelect_t *cs_pin, SpiTxBit_t tx_bit, SpiClkSource_t clk_src, uint16_t clk_prescaler, SpiCharLength_t ch_len)
{
    SPC_BIT_SET(P1SEL, BIT5 | BIT6 | BIT7);
    SPC_BIT_SET(P2SEL, BIT5 | BIT6 | BIT7);
    GPIO_setModeOutput(cs_pin->port, cs_pin->pin);
    GPIO_setDigitalPinHigh(cs_pin->port, cs_pin->pin);

    SPC_BIT_CLR(UCB0CTL1, UCSWRST);
    UCB0CTL0 |= tx_bit | ch_len;
    SPC_BIT_SET(UCB0CTL0, UCSYNC);
    UCB0CTL1 |= clk_src;
    UCB0BR0 = GET_LOW_BYTE(clk_prescaler);
    UCB0BR1 = GET_HIGH_BYTE(clk_prescaler);
    SPC_BIT_SET(UCB0CTL1, UCSWRST);
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
    uint8_t ret_val = 0;
    UCB0TXBUF = data;
    // while (!SPC_BIT_CHK(IFG2, UCB0TXIFG));
    while (SPC_BIT_CHK(UCB0STAT, UCBUSY))
        ;
    ret_val = UCB0RXBUF;
    return ret_val;
}

uint8_t SPI_ReadByte(uint8_t dummy_char)
{
    return SPI_WriteByte(dummy_char);
}

void SPI_WriteBuffer(uint8_t *buf, uint16_t size)
{
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
}

void SPI_ReadBuffer(uint8_t *buf, uint16_t size, uint8_t dummy_char)
{
    uint16_t i = 0;
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
}
