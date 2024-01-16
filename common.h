/**
 * @file common.h
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-10-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

#ifndef LOCAL_TEST
#include "msp430.h"
#endif
/**
 * @brief Define common frequencies for use with F_CPU
 *
 */
#define FRQ_32kHz 32768
#define FRQ_1MHz 1000000UL
#define FRQ_2MHz 2000000UL
#define FRQ_4MHz 4000000UL
#define FRQ_8MHz 8000000UL
#define FRQ_10MHz 10000000UL
#define FRQ_12MHz 12000000UL
#define FRQ_16MHz 16000000UL

/**
 * @brief Set n.bit in byte 0->1
 * @param byte: desired byte
 * @param nbit: number of bit want to set (from 0)
 * @return no return
 */
#define BIT_SET(byte, nbit) ((byte) |= (1 << (nbit)))
/**
 * @brief clear n.bit in byte 1->0
 * @param byte: desired byte
 * @param nbit: number of bit want to clear (from 0)
 * @return no return
 */
#define BIT_CLR(byte, nbit) ((byte) &= ~(1 << (nbit)))
/**
 * @brief Toggle n.bit in byte 0->1 or 1->0
 * @param byte: desired byte
 * @param nbit: number of bit want to toggle (from 0)
 * @return no return
 */
#define BIT_TOG(byte, nbit) ((byte) ^= (1 << (nbit)))

/**
 * @brief Check n.bit in byte if 0 or 1
 * @param byte: desired byte
 * @param nbit: number of bit want to check (from 0)
 * @return 0 or 1
 */
#define BIT_CHK(byte, nbit) (bool)((byte) & (1 << (nbit)))

/**
 * @brief Set n.bit in byte 0->1
 * @param byte: desired byte
 * @param bitName: bit name specified in MSP430 library (BIT0, BIT1, ...)
 * @return no return
 */
#define SPC_BIT_SET(byte, bitName) ((byte) |= (bitName))

/**
 * @brief Clear n.bit in byte 1->0
 * @param byte: desired byte
 * @param bitName: bit name specified in MSP430 library (BIT0, BIT1, ...)
 * @return no return
 */
#define SPC_BIT_CLR(byte, bitName) ((byte) &= ~(bitName))

/**
 * @brief Toggle n.bit in byte 0->1 or 1->0
 * @param byte: desired byte
 * @param bitName: bit name specified in MSP430 library (BIT0, BIT1, ...)
 * @return no return
 */
#define SPC_BIT_TOG(byte, bitName) ((byte) ^= (bitName))

/**
 * @brief Check n.bit in byte
 * @param byte: desired byte
 * @param bitName: bit name specified in MSP430 library (BIT0, BIT1, ...)
 * @return 0 or 1
 */
#define SPC_BIT_CHK(byte, bitName) (bool)((byte) & (bitName))

/**
 * @brief Get least significent byte in word
 */
#define GET_LOW_BYTE(word) (word & 0xff)

/**
 * @brief Get most significent byte in word
 */
#define GET_HIGH_BYTE(word) ((word >> 8) & 0xff)

/**
 * @brief Function operation status return values
 */
typedef enum OperationStatus_t
{
    STATUS_SUCCESS = 1,
    STATUS_FAILURE = 0
} OperationStatus_t;
