/**
 * @file spi.h
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include "../GPIO/gpio.h"
#include "../common.h"
#include "msp430.h"

typedef enum{
    SPI_AUX_CLK = UCSSEL_1,
    SPI_SM_CLK = UCSSEL_2
}SpiClkSource_t;

typedef enum{
    SPI_NO_INTERRUPT = 0,
    SPI_RX_INTERRUPT = UCB0RXIE,
    SPI_TX_INTERRUPT = UCB0TXIE,
    SPI_RXTX_INTERRUPT = UCB0RXIE | UCB0TXIE
}SpiInterruptMode_t;

typedef struct chipSelect_t
{
    uint8_t pin;
    uint8_t port;
}chipSelect_t;

void initSPI(chipSelect_t *cs_pin, SpiClkSource_t clk_src, uint16_t clk_prescaler);
void SPI_EnableInterrupt(SpiInterruptMode_t ie_mode);
void SPI_Enable(bool state);
uint8_t SPI_WriteByte(uint8_t data);
uint8_t SPI_ReadByte(uint8_t dummy_char);
void SPI_WriteBuffer(uint8_t* buf, uint16_t size);
void SPI_ReadBuffer(uint8_t* buf, uint16_t size, uint8_t dummy_char);
