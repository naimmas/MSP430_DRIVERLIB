/**
 * @file delay.h
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "common.h"
#define FRQ_CPU FRQ_1MHz

#ifndef FRQ_CPU
#error CPU clock frequency is not defined
#else
#define FRQ_TO_CPU_NS (F_CPU/1000000000.0)
#define FRQ_TO_CPU_US (F_CPU/1000000.0)
#define FRQ_TO_CPU_MS (F_CPU/1000.0)
#define FRQ_TO_CPU_S  (F_CPU/1.0)

#define _delay_ns(__ns) \
  if((uint32_t) (F_CPU_NS * __ns) != F_CPU_NS * __ns)\
        __delay_cycles((uint32_t) ( F_CPU_NS * __ns)+1);\
  else __delay_cycles((uint32_t) ( F_CPU_NS * __ns))

#define _delay_us(__us) \
  if((uint32_t) (F_CPU_US * __us) != F_CPU_US * __us)\
        __delay_cycles((uint32_t) ( F_CPU_US * __us)+1);\
  else __delay_cycles((uint32_t) ( F_CPU_US * __us))

#define _delay_ms(__ms) \
  if((uint32_t) (F_CPU_MS * __ms) != F_CPU_MS * __ms)\
        __delay_cycles((uint32_t) ( F_CPU_MS * __ms)+1);\
  else __delay_cycles((uint32_t) ( F_CPU_MS * __ms))

#define _delay_s(__s) \
  if((uint32_t) (F_CPU_S * __s) != F_CPU_S * __s)\
        __delay_cycles((uint32_t) ( F_CPU_S * __s)+1);\
  else __delay_cycles((uint32_t) ( F_CPU_S * __s))
#endif