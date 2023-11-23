/**
 * @file i2c.h
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-10-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#include "../common.h"
#define SDA_PIN BIT7 // P1.7
#define SCL_PIN BIT6 // P1.6

typedef enum
{
    I2C_DEVICE_SLAVE = 0,
    I2C_DEVICE_MASTER = 1
} I2cDeviceType_t;

typedef struct I2C_device_t I2C_device_t;
struct I2C_device_t
{
    I2cDeviceType_t device_type;
    uint16_t clock_prescaler;
    uint8_t device_address;
    volatile uint8_t is_transfer_cmplt;
    unsigned char *dataBuffer;
    volatile uint8_t dataSize;
    void (*transfer_data)(I2C_device_t *self, uint8_t useISR);
    uint8_t (*check_slave)(uint8_t slaveAddress);
    uint8_t (*check_line)();
};

OperationStatus_t initI2C(I2C_device_t *self);
static void(__i2c_transfer_data)(I2C_device_t *self, uint8_t useISR);
static uint8_t(__i2c_check_slave)(uint8_t slaveAddress);
static uint8_t(__i2c_check_line)();