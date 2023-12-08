/**
 * @file crc8.h
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../common.h"

extern const uint8_t crc8_table[256];

uint8_t calculate_crc8(const uint8_t *data, uint16_t length);