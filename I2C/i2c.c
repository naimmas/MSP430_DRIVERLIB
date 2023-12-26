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

static uint8_t isMaster = false;
unsigned char byteCtr;
I2cDevice_t *i2c_device_struct;

OperationStatus_t initI2C(I2cDevice_t *self)
{
    SPC_BIT_SET(P1SEL, SDA_PIN + SCL_PIN);  // Assign I2C pins to USCI_B0
    SPC_BIT_SET(P1SEL2, SDA_PIN + SCL_PIN); // Assign I2C pins to USCI_B0
    SPC_BIT_SET(UCB0CTL1, UCSWRST);         // Enable SW reset
    switch (self->device_type)
    {
    case I2C_DEVICE_MASTER:
        if ((self->clock_prescaler & self->device_address) == 0)
            return STATUS_FAILURE;
        SPC_BIT_SET(UCB0CTL0, UCMST + UCMODE_3 + UCSYNC); // I2C Master, synchronous mode
        SPC_BIT_SET(UCB0CTL1, UCSSEL_2 + UCSWRST);        // Use SMCLK, keep SW reset
        UCB0BR0 = GET_LOW_BYTE(self->clock_prescaler);    // set prescaler
        UCB0BR1 = GET_HIGH_BYTE(self->clock_prescaler);
        UCB0I2CSA = self->device_address; // set slave address
        SPC_BIT_SET(UCB0I2CIE, UCNACKIE);
        SPC_BIT_SET(IE2, UCB0RXIE); // Enable RX interrupt
        break;

    case I2C_DEVICE_SLAVE:
        SPC_BIT_SET(UCB0CTL0, UCMODE_3 + UCSYNC); // I2C Slave, synchronous mode
        UCB0I2COA = self->device_address;         // set own (slave) address
        SPC_BIT_SET(IE2, UCB0RXIE + UCB0TXIE);    // Enable RX interrupt
        SPC_BIT_SET(UCB0I2CIE, UCSTTIE);          // Enable STT interrupt
        break;

    default:
        return STATUS_FAILURE;
    }
    self->api->transfer_data = __i2c_transfer_data;
    self->api->check_line = __i2c_check_line;
    self->api->check_slave = __i2c_check_slave;
    SPC_BIT_CLR(UCB0CTL1, UCSWRST); // Clear SW reset, resume operation
    i2c_device_struct = self;
    return STATUS_SUCCESS;
}
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
static uint8_t(__i2c_check_line)()
{
    return (UCB0STAT & UCBBUSY);
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt
void USCIAB0RX_ISR(void)
{
    switch (i2c_device_struct->device_type)
    {
    case I2C_DEVICE_MASTER:
        if (UCB0STAT & UCNACKIFG)
        {
            UCB0CTL1 |= UCTXSTP;
            UCB0STAT &= ~UCNACKIFG;
        }
        return;
    case I2C_DEVICE_SLAVE:
        UCB0STAT &= ~(UCSTPIFG + UCSTTIFG); // Clear interrupt flags
        return;
    }
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt
void USCIAB0TX_ISR(void)
{
    switch (i2c_device_struct->device_type)
    {
    case I2C_DEVICE_MASTER:
        if (byteCtr == 0)
        {
            UCB0CTL1 |= UCTXSTP; // I2C stop condition
            IFG2 &= ~UCB0TXIFG;  // Clear USCI_B0 TX int flag
        }
        else
        {
            UCB0TXBUF = i2c_device_struct->dataBuffer;
            i2c_device_struct->dataBuffer++;
            byteCtr--;
        }
        return;
    case I2C_DEVICE_SLAVE:
        *i2c_device_struct->dataBuffer++ = UCB0RXBUF; // Move RX data to address PRxData
        byteCtr++;                                    // Increment RX byte count
        return;
    }
}