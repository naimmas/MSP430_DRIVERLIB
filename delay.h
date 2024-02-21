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
/**
 * @brief
 * ! accuracy is 100us with 16Mhz cpu frequency
 */
#define FRQ_CPU FRQ_1MHz
#ifndef FRQ_CPU
#error CPU clock frequency is not defined
#else
#define FRQ_TO_CPU_NS (FRQ_CPU / 1000000000.0)
#define FRQ_TO_CPU_US (FRQ_CPU / 1000000.0)
#define FRQ_TO_CPU_MS (FRQ_CPU / 1000.0)
#define FRQ_TO_CPU_S (FRQ_CPU / 1.0)

#define _delay_ns(__ns)                                         \
  if ((uint32_t)(FRQ_TO_CPU_NS * __ns) != FRQ_TO_CPU_NS * __ns) \
    __delay_cycles((uint32_t)(FRQ_TO_CPU_NS * __ns) + 1);       \
  else                                                          \
    __delay_cycles((uint32_t)(FRQ_TO_CPU_NS * __ns))

#define _delay_us(__us) __delay_cycles((uint32_t)(FRQ_TO_CPU_US * __us))

#define _delay_ms(__ms)                                         \
  if ((uint32_t)(FRQ_TO_CPU_MS * __ms) != FRQ_TO_CPU_MS * __ms) \
    __delay_cycles((uint32_t)(FRQ_TO_CPU_MS * __ms) + 1);       \
  else                                                          \
    __delay_cycles((uint32_t)(FRQ_TO_CPU_MS * __ms))

#define _delay_s(__s)                                       \
  if ((uint32_t)(FRQ_TO_CPU_S * __s) != FRQ_TO_CPU_S * __s) \
    __delay_cycles((uint32_t)(FRQ_TO_CPU_S * __s) + 1);     \
  else                                                      \
    __delay_cycles((uint32_t)(FRQ_TO_CPU_S * __s))
#endif
