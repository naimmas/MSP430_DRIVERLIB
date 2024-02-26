/**
 * @file i2c.c
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-10-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "i2c.h"
#include "msp430.h"

// TODO: Implement interrupt enable function
// TODO: Documentation
unsigned char byteCtr;

/**
 * @brief if the address is 0 then the module is disabled
 * 
 * @param self 
 * @return OperationStatus_t 
 */
OperationStatus_t initI2C(I2cDevice_t *self)
{
    SPC_BIT_SET(P1SEL, SDA_PIN + SCL_PIN);  // Assign I2C pins to USCI_B0
    SPC_BIT_SET(P1SEL2, SDA_PIN + SCL_PIN); // Assign I2C pins to USCI_B0
    __i2c_disable();
#ifdef I2C_DEVICE_MASTER
    if ((self->clock_prescaler & self->device_address) == 0)
        return STATUS_FAILURE;
    SPC_BIT_SET(UCB0CTL0, UCMST + UCMODE_3 + UCSYNC); // I2C Master, synchronous mode
    SPC_BIT_SET(UCB0CTL1, UCSSEL_2 + UCTR);        // Use SMCLK, set transmitter bit
    UCB0BR0 = GET_LOW_BYTE(self->clock_prescaler);    // set prescaler
    UCB0BR1 = GET_HIGH_BYTE(self->clock_prescaler);
    UCB0I2CSA = self->device_address; // set slave address
    SPC_BIT_SET(UCB0I2CIE, UCNACKIE);
    SPC_BIT_SET(IE2, UCB0RXIE); // Enable RX interrupt
    self->api->transfer_data = __i2c_transfer_data;
    self->api->check_slave = __i2c_check_slave;

#else
    SPC_BIT_SET(UCB0CTL0, UCMODE_3 + UCSYNC); // I2C Slave, synchronous mode
    UCB0I2COA = self->device_address + 0x8000;         // set own (slave) address, enable general call
    __i2c_enable();
    SPC_BIT_SET(IE2, UCB0RXIE + UCB0TXIE);    // Enable RX interrupt
    SPC_BIT_SET(UCB0I2CIE, UCSTTIE);          // Enable STT interrupt
#endif
    self->api = (I2cDevApi_t*) malloc(sizeof(I2cDevApi_t));
    self->api->check_line = __i2c_check_line;
    self->api->enable = __i2c_enable; // Enable I2C module
    self->api->disable = __i2c_disable; // Disable I2C module
    self->api->set_address = __i2c_set_address;
    self->api->enable_generalCall = __i2c_set_gc;
    if(self->device_address) __i2c_enable();
    return STATUS_SUCCESS;
}
#ifdef I2C_DEVICE_MASTER
static void(__i2c_transfer_data)(I2cDevice_t *self, uint8_t useISR)
{
    switch (self->device_type)
    {
    case I2C_DEVICE_MASTER:
        byteCtr = self->dataSize;
        break;

    case I2C_DEVICE_SLAVE:
        break;
    }
}
static uint8_t(__i2c_check_slave)(uint8_t slaveAddress)
{
    unsigned char ie2_bak, slaveadr_bak, ucb0i2cie, returnValue;
    ucb0i2cie = UCB0I2CIE;         // restore old UCB0I2CIE
    ie2_bak = IE2;                 // store IE2 register
    slaveadr_bak = UCB0I2CSA;      // store old slave address
    UCB0I2CIE &= ~UCNACKIE;        // no NACK interrupt
    UCB0I2CSA = slaveAddress;      // set slave address
    IE2 &= ~(UCB0TXIE + UCB0RXIE); // no RX or TX interrupts
    __disable_interrupt();
    UCB0CTL1 |= UCTR + UCTXSTT + UCTXSTP; // I2C TX, start condition
    while (UCB0CTL1 & UCTXSTP)
        ; // wait for STOP condition

    returnValue = !(UCB0STAT & UCNACKIFG);
    __enable_interrupt();
    IE2 = ie2_bak;            // restore IE2
    UCB0I2CSA = slaveadr_bak; // restore old slave address
    UCB0I2CIE = ucb0i2cie;    // restore old UCB0CTL1
    return returnValue;       // return whether or not
                              // a NACK occured
}
#endif
static inline void __i2c_set_address(I2cDevice_t* self, uint8_t new_address)
{
    self->device_address = new_address; 
    __i2c_disable();
    #ifdef I2C_DEVICE_MASTER
    UCB0I2CSA = address;
    #else
    UCB0I2COA = new_address;
    #endif
    __i2c_enable();  
}
static uint8_t(__i2c_check_line)()
{
    return (UCB0STAT & UCBBUSY);
}
static inline void __i2c_set_gc(uint8_t enable)
{
    if(enable)  SPC_BIT_SET(UCB0I2COA, UCGCEN);
    else        SPC_BIT_CLR(UCB0I2COA, UCGCEN);
}
static inline void __i2c_enable()
{
    SPC_BIT_CLR(UCB0CTL1, UCSWRST);
    SPC_BIT_SET(IE2, UCB0RXIE + UCB0TXIE);    // Enable RX interrupt
    SPC_BIT_SET(UCB0I2CIE, UCSTTIE);          // Enable STT interrupt
}
static inline void __i2c_disable()
{
    SPC_BIT_SET(UCB0CTL1, UCSWRST);
}
