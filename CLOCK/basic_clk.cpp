/**
 * @file basic_clk.c
 * @author Naim MASRI (naimmas@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-10-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "basic_clk.h"

void defaultBasicClock(BasicClock_t* basic_clk_structure)
{
    basic_clk_structure->dco_freq = DCO_FRQ_16MHZ;
    basic_clk_structure->aux_clk = ACLK_SRC_VLO;
    basic_clk_structure->main_clk = MCLK_SRC_DCO;
    basic_clk_structure->submain_clk = SMCLK_SRC_DCO;
    basic_clk_structure->aux_clk_divider = CLK_DIV_1;
    basic_clk_structure->submain_clk_divider = CLK_DIV_1;
    basic_clk_structure->main_clk_divider = CLK_DIV_1;
    basic_clk_structure->xcap = XCAP_1pF;
}
void initBasicClock(BasicClock_t* basic_clk_structure)
{
    basic_clk_structure->disableWDT = __basic_clk_disable_wdt;
    basic_clk_structure->enableOscFaultInt = __enable_osc_fault_interrupt;
    basic_clk_structure->setDCOFreq = __basic_clk_set_dco_frq;
    basic_clk_structure->configAuxClk = __basic_clk_cfg_aux_clk;
    basic_clk_structure->configMainClk = __basic_clk_cfg_main_clk;
    basic_clk_structure->configSubMainClk = __basic_clk_cfg_submain_clk;
    __basic_clk_disable_wdt();
    __basic_clk_set_dco_frq(basic_clk_structure);
    __basic_clk_cfg_aux_clk(basic_clk_structure);
    __basic_clk_cfg_main_clk(basic_clk_structure);
    __basic_clk_cfg_submain_clk(basic_clk_structure);
}

inline void __basic_clk_disable_wdt()
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
}
inline void __enable_osc_fault_interrupt()
{
    SPC_BIT_SET(IE1, OFIE);
}
OperationStatus_t __basic_clk_set_dco_frq(BasicClock_t *const self)
{
    uint8_t caldco = 0;
    uint8_t calbc1 = 0;
    switch (self->dco_freq)
    {
    case DCO_FRQ_1MHZ:
        caldco = CALDCO_1MHZ;
        calbc1 = CALBC1_1MHZ;
        break;
    case DCO_FRQ_8MHZ:
        caldco = CALDCO_8MHZ;
        calbc1 = CALBC1_8MHZ;
        break;
    case DCO_FRQ_12MHZ:
        caldco = CALDCO_12MHZ;
        calbc1 = CALBC1_12MHZ;
        break;
    case DCO_FRQ_16MHZ:
        caldco = CALDCO_16MHZ;
        calbc1 = CALBC1_16MHZ;
        break;
    default:
        caldco = 0xff;
        calbc1 = 0xff;
        break;
    }
    if(0xff == caldco ||  0xff == calbc1) return STATUS_FAILURE;
    
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = calbc1;
    DCOCTL  = caldco;
    return STATUS_SUCCESS;
}
OperationStatus_t __basic_clk_cfg_aux_clk(BasicClock_t *const self)
{
    if(self->aux_clk == ACLK_SRC_32KHZ)
    {
        GPIO_setModeInput(GPIO_PORT_2, 6);
        GPIO_setModeOutput(GPIO_PORT_2, 7);
        SPC_BIT_SET(P2SEL, BIT6|BIT7);
        SPC_BIT_CLR(P2SEL2, BIT6|BIT7);

        SPC_BIT_CLR(BCSCTL3, BIT2 | BIT3);
        SPC_BIT_SET(BCSCTL3, self->xcap);
    }
    SPC_BIT_CLR(BCSCTL3, BIT4 | BIT5);
    SPC_BIT_SET(BCSCTL3, self->aux_clk);

    SPC_BIT_CLR(BCSCTL1, BIT4 | BIT5);
    SPC_BIT_SET(BCSCTL1, self->aux_clk_divider<<4);

    if(self->aux_clk == ACLK_SRC_32KHZ)
    {
        do{
            SPC_BIT_CLR(BCSCTL3, LFXT1OF);
            SPC_BIT_CLR(IFG1, OFIFG);
        }while(SPC_BIT_CHK(BCSCTL3, LFXT1OF));
    }
    return STATUS_SUCCESS;
}
OperationStatus_t __basic_clk_cfg_main_clk(BasicClock_t *const self)
{
    SPC_BIT_CLR(BCSCTL2, BIT6|BIT7);
    SPC_BIT_SET(BCSCTL2, self->main_clk);
    SPC_BIT_CLR(BCSCTL2, BIT5|BIT4);
    SPC_BIT_SET(BCSCTL2, self->main_clk_divider<<4);
    return STATUS_SUCCESS;
}
OperationStatus_t __basic_clk_cfg_submain_clk(BasicClock_t *const self)
{
    SPC_BIT_CLR(BCSCTL2, BIT3);
    SPC_BIT_SET(BCSCTL2, self->submain_clk);
    SPC_BIT_CLR(BCSCTL2, BIT2|BIT1);
    SPC_BIT_SET(BCSCTL2, self->submain_clk_divider<<1);
    return STATUS_SUCCESS;
}
