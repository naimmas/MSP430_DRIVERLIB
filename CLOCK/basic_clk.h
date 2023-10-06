/**
 * @file basic_clk.h
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once
#include <msp430.h>
#include "../common.h"
#include "gpio.h"
typedef enum{
    CLK_DIV_1 = 0,
    CLK_DIV_2 = 1,
    CLK_DIV_4 = 2,
    CLK_DIV_8 = 3,
}ClockDivder_t;

typedef enum{
    ACLK_SRC_32KHZ = LFXT1S_0,
    ACLK_SRC_VLO = LFXT1S_2
}AuxClkSource_t;

typedef enum{
    DCO_FRQ_1MHZ ,
    DCO_FRQ_8MHZ ,
    DCO_FRQ_12MHZ,
    DCO_FRQ_16MHZ,
}DCOFreq_t;

typedef enum{
    MCLK_SRC_DCO = SELM_0,
    MCLK_SRC_LFXT = SELM_3
}MainClkSource_t;

typedef enum
{
  SMCLK_SRC_DCO  = 0,  
  SMCLK_SRC_LFXT = SELS 
}SubMainClkSource_t;

typedef enum{
    XCAP_1pF    = XCAP_0,
    XCAP_6pF    = XCAP_1,
    XCAP_10pF   = XCAP_2,
    XCAP_12_5pF = XCAP_3 
}LFCrystalCapacitor_t;

typedef struct BasicClock_t BasicClock_t;
struct BasicClock_t{
    DCOFreq_t dco_freq;
    AuxClkSource_t aux_clk;
    MainClkSource_t main_clk;
    SubMainClkSource_t submain_clk;
    ClockDivder_t aux_clk_divider;
    ClockDivder_t main_clk_divider;
    ClockDivder_t submain_clk_divider;
    LFCrystalCapacitor_t xcap;
    
    void (*disableWDT)        ();
    void (*enableOscFaultInt) ();
    OperationStatus_t (*setDCOFreq)       (BasicClock_t* const self);
    OperationStatus_t (*configAuxClk)     (BasicClock_t* const self);
    OperationStatus_t (*configMainClk)    (BasicClock_t* const self);
    OperationStatus_t (*configSubMainClk) (BasicClock_t* const self);
};

void initBasicClock(BasicClock_t* basic_clk_structure);
inline void __basic_clk_disable_wdt      ();
inline void __enable_osc_fault_interrupt ();

OperationStatus_t __basic_clk_set_dco_frq     (BasicClock_t* const self);
OperationStatus_t __basic_clk_cfg_aux_clk     (BasicClock_t* const self);
OperationStatus_t __basic_clk_cfg_main_clk    (BasicClock_t* const self);
OperationStatus_t __basic_clk_cfg_submain_clk (BasicClock_t* const self);