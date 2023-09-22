#include "stdint.h"
#include "stdbool.h"

#ifndef LOCAL_TEST
#include <msp430.h>
#endif

#define BIT_SET(byte,nbit)  ((byte) |=  (1<<(nbit)))
#define BIT_CLR(byte,nbit)  ((byte) &= ~(1<<(nbit)))
#define BIT_TOG(byte,nbit)  ((byte) ^=  (1<<(nbit)))
#define BIT_CHK(byte,nbit)  ((byte) &   (1<<(nbit)))

#define SPC_BIT_SET(byte,bitName)  ((byte) |=  (bitName))
#define SPC_BIT_CLR(byte,bitName)  ((byte) &= ~(bitName))
#define SPC_BIT_TOG(byte,bitName)  ((byte) ^=  (bitName))
#define SPC_BIT_CHK(byte,bitName)  ((byte) &   (bitName))

#define GET_LOW_BYTE(word)      (word&0xff)
#define GET_HIGH_BYTE(word)     ((word>>8)&0xff)
#define STATUS_SUCCESS  0x01
#define STATUS_FAIL     0x00
