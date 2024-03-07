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
#pragma once
#include "../common.h"
uint8_t calculate_crc8(volatile const uint8_t *data, uint8_t size);
bool check_crc8(volatile const uint8_t *data, uint8_t size);
