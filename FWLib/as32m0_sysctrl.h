
#ifndef __AS32M0_SYSCTRL_H__
#define __AS32M0_SYSCTRL_H__


#ifdef	__cplusplus
extern "C" {	/* allow C++ to use these headers */
#endif	/* __cplusplus */

#include "as32m0.h"
/*
 * Description:
 * Bit shifts and widths for iClkSrcSel    0x00
 */
#define bsCLKSRCSEL_FAST_CLK_SEL                       0
#define bwCLKSRCSEL_FAST_CLK_SEL                           1
#define bsCLKSRCSEL_SLOW_CLK_SEL                       1
#define bwCLKSRCSEL_SLOW_CLK_SEL                           1


/*
 * Description:
 * Bit shifts and widths for iClkCtrlSel  0x10
 */
#define bsCLKCTRLSEL_OSC_CLK_SEL                      0
#define bwCLKCTRLSEL_OSC_CLK_SEL                          1
#define bsCLKCTRLSEL_SYS_CLK_SEL                      1
#define bwCLKCTRLSEL_SYS_CLK_SEL                          1
#define bsCLKCTRLSEL_APB_CLK_SEL                      2
#define bwCLKCTRLSEL_APB_CLK_SEL                          1


/*
 * Description:
 * Bit shifts and widths for iOscClkDiv  0x20
 */
#define bsOSCCLKDIV                                  0
#define bwOSCCLKDIV                                      0x1F


/*
 * Description:
 * Bit shifts and widths for iSysClkDiv  0x24
 */
#define bsSYSCLKDIV                                 0
#define bwSYSCLKDIV                                     0x1F


/*
 * Description:
 * Bit shifts and widths for iAhbClkGate  0x30
 */
#define bsAHBCLKGATE_DMA_CLK_GATE                  0  
#define bwAHBCLKGATE_DMA_CLK_GATE                      1  
#define bsAHBCLKGATE_DES_CLK_GATE                  1  
#define bwAHBCLKGATE_DES_CLK_GATE                      1  
#define AHBCLKGATE_DMA_CLK_GATE                    1 
#define AHBCLKGATE_DES_CLK_GATE                    2 

/*
 * Description:
 * Bit shifts and widths for iApbClkGate  0x40
 */
#define bsAPBCLKGATE_PINCTRL_CLK_GATE            0
#define bwAPBCLKGATE_PINCTRL_CLK_GATE                1
#define bsAPBCLKGATE_LVD_CLK_GATE                1
#define bwAPBCLKGATE_LVD_CLK_GATE                    1
#define bsAPBCLKGATE_ADC_CLK_GATE                2
#define bwAPBCLKGATE_ADC_CLK_GATE                    1
#define bsAPBCLKGATE_PWM0_CLK_GATE               3
#define bwAPBCLKGATE_PWM0_CLK_GATE                   1
#define bsAPBCLKGATE_PWM1_CLK_GATE               4
#define bwAPBCLKGATE_PWM1_CLK_GATE                   1
#define bsAPBCLKGATE_PWM2_CLK_GATE               5
#define bwAPBCLKGATE_PWM2_CLK_GATE                   1
#define bsAPBCLKGATE_PWM3_CLK_GATE               6
#define bwAPBCLKGATE_PWM3_CLK_GATE                   1
#define bsAPBCLKGATE_I2C_CLK_GATE                7
#define bwAPBCLKGATE_I2C_CLK_GATE                    1
#define bsAPBCLKGATE_SPI_CLK_GATE                8
#define bwAPBCLKGATE_SPI_CLK_GATE                    1
#define bsAPBCLKGATE_SCI0_CLK_GATE               9
#define bwAPBCLKGATE_SCI0_CLK_GATE                   1
#define bsAPBCLKGATE_SCI1_CLK_GATE               10
#define bwAPBCLKGATE_SCI1_CLK_GATE                   1
#define bsAPBCLKGATE_TIMER0_CLK_GATE             11
#define bwAPBCLKGATE_TIMER0_CLK_GATE                 1
#define bsAPBCLKGATE_TIMER1_CLK_GATE             12
#define bwAPBCLKGATE_TIMER1_CLK_GATE                 1
#define bsAPBCLKGATE_TIMER2_CLK_GATE             13
#define bwAPBCLKGATE_TIMER2_CLK_GATE                 1
#define bsAPBCLKGATE_WDG_CLK_GATE                14
#define bwAPBCLKGATE_WDG_CLK_GATE                    1
#define bsAPBCLKGATE_GPIO_CLK_GATE               15
#define bwAPBCLKGATE_GPIO_CLK_GATE                   1

#define APBCLKGATE_PINCTRL_CLK_GATE            ((uint32_t)0x00000001)
#define APBCLKGATE_LVD_CLK_GATE                ((uint32_t)0x00000002)
#define APBCLKGATE_ADC_CLK_GATE                ((uint32_t)0x00000004)
#define APBCLKGATE_PWM0_CLK_GATE               ((uint32_t)0x00000008)
#define APBCLKGATE_PWM1_CLK_GATE               ((uint32_t)0x00000010)
#define APBCLKGATE_PWM2_CLK_GATE               ((uint32_t)0x00000020)
#define APBCLKGATE_PWM3_CLK_GATE               ((uint32_t)0x00000040)
#define APBCLKGATE_I2C_CLK_GATE                ((uint32_t)0x00000080)
#define APBCLKGATE_SPI_CLK_GATE                ((uint32_t)0x00000100)
#define APBCLKGATE_SCI0_CLK_GATE               ((uint32_t)0x00000200)
#define APBCLKGATE_SCI1_CLK_GATE               ((uint32_t)0x00000400)
#define APBCLKGATE_TIMER0_CLK_GATE             ((uint32_t)0x00000800)
#define APBCLKGATE_TIMER1_CLK_GATE             ((uint32_t)0x00001000)
#define APBCLKGATE_TIMER2_CLK_GATE             ((uint32_t)0x00002000)
#define APBCLKGATE_WDG_CLK_GATE                ((uint32_t)0x00004000)
#define APBCLKGATE_GPIO_CLK_GATE               ((uint32_t)0x00008000)
/*
 * Description:
 * Bit shifts and widths for iWorkClkGate  0x50
 */
#define bsWORKCLKGATE_WDG_CLK_GATE               0
#define bwWORKCLKGATE_WDG_CLK_GATE                   1

#define WORKCLKGATE_WDG_CLK_GATE  							((uint32_t)0x00000001)
/*
 * Description:
 * Bit shifts and widths for iAhbBlkRst  0x60
 */
#define bsAHBBLKRST_DMA_BLK_RST                 0
#define bwAHBBLKRST_DMA_BLK_RST                     1
#define bsAHBBLKRST_DES_BLK_RST                 1
#define bwAHBBLKRST_DES_BLK_RST                     1


/*
 * Description:
 * Bit shifts and widths for iApbBlkRst  0x70
 */
#define bsAPBBLKRST_PINCTRL_BLK_RST            0
#define bwAPBBLKRST_PINCTRL_BLK_RST                1
#define bsAPBBLKRST_LVD_BLK_RST                1
#define bwAPBBLKRST_LVD_BLK_RST                    1
#define bsAPBBLKRST_ADC_BLK_RST                2
#define bwAPBBLKRST_ADC_BLK_RST                    1
#define bsAPBBLKRST_PWM0_BLK_RST               3
#define bwAPBBLKRST_PWM0_BLK_RST                   1
#define bsAPBBLKRST_PWM1_BLK_RST               4
#define bwAPBBLKRST_PWM1_BLK_RST                   1
#define bsAPBBLKRST_PWM2_BLK_RST               5
#define bwAPBBLKRST_PWM2_BLK_RST                   1
#define bsAPBBLKRST_PWM3_BLK_RST               6
#define bwAPBBLKRST_PWM3_BLK_RST                   1
#define bsAPBBLKRST_I2C_BLK_RST                7
#define bwAPBBLKRST_I2C_BLK_RST                    1
#define bsAPBBLKRST_SPI_BLK_RST                8
#define bwAPBBLKRST_SPI_BLK_RST                    1
#define bsAPBBLKRST_SCI0_BLK_RST               9
#define bwAPBBLKRST_SCI0_BLK_RST                   1
#define bsAPBBLKRST_SCI1_BLK_RST               10
#define bwAPBBLKRST_SCI1_BLK_RST                   1
#define bsAPBBLKRST_TIMER0_BLK_RST             11
#define bwAPBBLKRST_TIMER0_BLK_RST                 1
#define bsAPBBLKRST_TIMER1_BLK_RST             12
#define bwAPBBLKRST_TIMER1_BLK_RST                 1
#define bsAPBBLKRST_TIMER2_BLK_RST             13
#define bwAPBBLKRST_TIMER2_BLK_RST                 1
#define bsAPBBLKRST_WDG_BLK_RST                14
#define bwAPBBLKRST_WDG_BLK_RST                    1
#define bsAPBBLKRST_GPIO_BLK_RST               15
#define bwAPBBLKRST_GPIO_BLK_RST                   1


/*
 * Description:
 * Bit shifts and widths for iWorkBlkRst  0x80
 */
#define bsWORKBLKRST_WDG_BLK_RST               0
#define bwWORKBLKRST_WDG_BLK_RST                   1


/*
 * Description:
 * Bit shifts and widths for iSoftWarmReset  0x90
 */
#define bsSOFTWARMRESET                        0
#define bwSOFTWARMRESET                            1


/*
 * Description:
 * Bit shifts and widths for iSoftColdReset  0x94
 */
#define bsSOFTCOLDRESET                        0
#define bwSOFTCOLDRESET                            1


/*
 * Description:
 * Bit shifts and widths for iChipID  0x98
 */
#define bsCHIPID                              0
#define bwCHIPID                              0x12345678

/*
 * Description:
 * Bit shifts and widths for iROscCtrl  0xA0
 */
#define bsROSCCTRL_ROSC16M_TIMING_SET        0
#define bwROSCCTRL_ROSC16M_TIMING_SET            0x3F
#define bsROSCCTRL_ROSC16M_SELECTION         8
#define bwROSCCTRL_ROSC16M_SELECTION             1
#define bsROSCCTRL_ROSC16M_POWERDOWN         16
#define bwROSCCTRL_ROSC16M_POWERDOWN             1
#define bsROSCCTRL_ROSC32k_POWERDOWN         24
#define bwROSCCTRL_ROSC32k_POWERDOWN             1

/*
 * Description:
 * Bit shifts and widths for iChipMode  0xB0
 */
#define bsCHIPMODE_TEST_MODE                 0
#define bwCHIPMODE_TEST_MODE                     1
#define bsCHIPMODE_WORK_MODE                 1
#define bwCHIPMODE_WORK_MODE                     1
#define bsCHIPMODE_AUX_WORK_MODE             2
#define bwCHIPMODE_AUX_WORK_MODE                 1

#define TM0_WM0                               0x0
#define TM0_WM1                               0x2
#define TM1_WM0_AWM0                          0x1
#define TM1_WM1_AWM0                          0x3
#define TM1_WM0_AWM1                          0x5
#define TM1_WM1_AWM1                          0x7

/*
 * Description:
 * Bit shifts and widths for iMBIST  0xB8
 */
#define bsMBIST_RESET                        27
#define bwMBIST_RESET                            1
#define bsMBIST_ENABLE                       26
#define bwMBIST_ENABLE                           1
#define bsMBIST_MODE                         24
#define bwMBIST_MODE                             0x3
#define bsMBIST_INIT                         16
#define bwMBIST_INIT                             0xFF
#define bsMBIST_CS                           10
#define bwMBIST_CS                               0x3
#define bsMBIST_DONE                         9
#define bwMBIST_DONE                             1
#define bsMBIST_PASS                         8
#define bwMBIST_PASS                             1
#define bsMBIST_LOG                          0
#define bwMBIST_LOG                              0xFF

// Clk Selection
void SYSCTRL_FastClockSwitch_ROSC(void);
void SYSCTRL_FastClockSwitch_EXT(void);
void SYSCTRL_SlowClockSwitch_ROSC(void);
void SYSCTRL_SlowClockSwitch_EXT(void);
void SYSCTRL_ApbClkSel_Sys(void);
void SYSCTRL_ApbClkSel_SysDiv(void);
void SYSCTRL_SysClkSel_OSC(void);
void SYSCTRL_SysClkSel_OSCDiv(void);
void SYSCTRL_OSCClkSel_FastClk(void);
void SYSCTRL_OSCClkSel_SlowClk(void);

//Clk Div
void SYSCTRL_OSC_ClkDIv_Set(uint8_t div);
void SYSCTRL_Sys_ClkDIv_Set(uint8_t div);

//Clk Gate
void SYSCTRL_SetClkGate_AHB_DES(void);
void SYSCTRL_SetClkGate_AHB_DMA(void);

void SYSCTRL_SetClkGate_APB_GPIO(void);
void SYSCTRL_SetClkGate_APB_WDG(void);
void SYSCTRL_SetClkGate_APB_TIMER0(void);
void SYSCTRL_SetClkGate_APB_TIMER1(void);
void SYSCTRL_SetClkGate_APB_TIMER2(void);
void SYSCTRL_SetClkGate_APB_SCI0(void);
void SYSCTRL_SetClkGate_APB_SCI1(void);
void SYSCTRL_SetClkGate_APB_SPI(void);
void SYSCTRL_SetClkGate_APB_I2C(void);
void SYSCTRL_SetClkGate_APB_PWM0(void);
void SYSCTRL_SetClkGate_APB_PWM1(void);
void SYSCTRL_SetClkGate_APB_PWM2(void);
void SYSCTRL_SetClkGate_APB_PWM3(void);
void SYSCTRL_SetClkGate_APB_ADC(void);
void SYSCTRL_SetClkGate_APB_LVD(void);
void SYSCTRL_SetClkGate_APB_PinCtrl(void);
void SYSCTRL_SetClkGate_WDG_WorkClk(void);

void SYSCTRL_SetAhbClkGate(uint32_t gate_val);
void SYSCTRL_SetApbClkGate(uint32_t gate_val);
void SYSCTRL_SetWorkClkGate(uint32_t gate_val);

uint32_t SYSCTRL_GetAhbClkGate(void);
uint32_t SYSCTRL_GetApbClkGate(void);
uint32_t SYSCTRL_GetWorkClkGate(void);


//Clk Gate Clr
void SYSCTRL_ClrClkGate_AHB_DES(void);
void SYSCTRL_ClrClkGate_AHB_DMA(void);

void SYSCTRL_ClrClkGate_APB_GPIO(void);
void SYSCTRL_ClrClkGate_APB_WDG(void);
void SYSCTRL_ClrClkGate_APB_TIMER0(void);
void SYSCTRL_ClrClkGate_APB_TIMER1(void);
void SYSCTRL_ClrClkGate_APB_TIMER2(void);
void SYSCTRL_ClrClkGate_APB_SCI0(void);
void SYSCTRL_ClrClkGate_APB_SCI1(void);
void SYSCTRL_ClrClkGate_APB_SPI(void);
void SYSCTRL_ClrClkGate_APB_I2C(void);
void SYSCTRL_ClrClkGate_APB_PWM0(void);
void SYSCTRL_ClrClkGate_APB_PWM1(void);
void SYSCTRL_ClrClkGate_APB_PWM2(void);
void SYSCTRL_ClrClkGate_APB_PWM3(void);
void SYSCTRL_ClrClkGate_APB_ADC(void);
void SYSCTRL_ClrClkGate_APB_LVD(void);
void SYSCTRL_ClrClkGate_APB_PinCtrl(void);
void SYSCTRL_ClrClkGate_WDG_WorkClk(void);

void SYSCTRL_ClrAhbClkGate(uint32_t gate_val); 
void SYSCTRL_ClrApbClkGate(uint32_t gate_val); 
void SYSCTRL_ClrWorkClkGate(uint32_t gate_val);


//Clk Gate Toggle
void SYSCTRL_TogClkGate_AHB_DES(void);
void SYSCTRL_TogClkGate_AHB_DMA(void);

void SYSCTRL_TogClkGate_APB_GPIO(void);
void SYSCTRL_TogClkGate_APB_WDG(void);
void SYSCTRL_TogClkGate_APB_TIMER0(void);
void SYSCTRL_TogClkGate_APB_TIMER1(void);
void SYSCTRL_TogClkGate_APB_TIMER2(void);
void SYSCTRL_TogClkGate_APB_SCI0(void);
void SYSCTRL_TogClkGate_APB_SCI1(void);
void SYSCTRL_TogClkGate_APB_SPI(void);
void SYSCTRL_TogClkGate_APB_I2C(void);
void SYSCTRL_TogClkGate_APB_PWM0(void);
void SYSCTRL_TogClkGate_APB_PWM1(void);
void SYSCTRL_TogClkGate_APB_PWM2(void);
void SYSCTRL_TogClkGate_APB_PWM3(void);
void SYSCTRL_TogClkGate_APB_ADC(void);
void SYSCTRL_TogClkGate_APB_LVD(void);
void SYSCTRL_TogClkGate_APB_PinCtrl(void);
void SYSCTRL_TogClkGate_WDG_WorkClk(void);


//Block Reset
void SYSCTRL_SetReset_AHB_DES(void);
void SYSCTRL_SetReset_AHB_DMA(void);

void SYSCTRL_SetReset_APB_GPIO(void);
void SYSCTRL_SetReset_APB_WDG(void);
void SYSCTRL_SetReset_APB_TIMER0(void);
void SYSCTRL_SetReset_APB_TIMER1(void);
void SYSCTRL_SetReset_APB_TIMER2(void);
void SYSCTRL_SetReset_APB_SCI0(void);
void SYSCTRL_SetReset_APB_SCI1(void);
void SYSCTRL_SetReset_APB_SPI(void);
void SYSCTRL_SetReset_APB_I2C(void);
void SYSCTRL_SetReset_APB_PWM0(void);
void SYSCTRL_SetReset_APB_PWM1(void);
void SYSCTRL_SetReset_APB_PWM2(void);
void SYSCTRL_SetReset_APB_PWM3(void);
void SYSCTRL_SetReset_APB_ADC(void);
void SYSCTRL_SetReset_APB_LVD(void);
void SYSCTRL_SetReset_APB_PinCtrl(void);

void SYSCTRL_SetReset_WDG_WorkClk(void);

void SYSCTRL_SetReset_Soft_Warm(void);
void SYSCTRL_SetReset_Soft_Cold(void);

void SYSCTRL_SetReset_AHB(uint32_t reg_val);
void SYSCTRL_SetReset_APB(uint32_t reg_val);
void SYSCTRL_SetReset_WorkClk(int32_t reg_val);

uint32_t SYSCTRL_GetReset_AHB(void);
uint32_t SYSCTRL_GetReset_APB(void);
uint32_t SYSCTRL_GetReset_WorkClk(void);

//Block Reset Clear
void SYSCTRL_ClrReset_AHB_DES(void);
void SYSCTRL_ClrReset_AHB_DMA(void);

void SYSCTRL_ClrReset_APB_GPIO(void);
void SYSCTRL_ClrReset_APB_WDG(void);
void SYSCTRL_ClrReset_APB_TIMER0(void);
void SYSCTRL_ClrReset_APB_TIMER1(void);
void SYSCTRL_ClrReset_APB_TIMER2(void);
void SYSCTRL_ClrReset_APB_SCI0(void);
void SYSCTRL_ClrReset_APB_SCI1(void);
void SYSCTRL_ClrReset_APB_SPI(void);
void SYSCTRL_ClrReset_APB_I2C(void);
void SYSCTRL_ClrReset_APB_PWM0(void);
void SYSCTRL_ClrReset_APB_PWM1(void);
void SYSCTRL_ClrReset_APB_PWM2(void);
void SYSCTRL_ClrReset_APB_PWM3(void);
void SYSCTRL_ClrReset_APB_ADC(void);
void SYSCTRL_ClrReset_APB_LVD(void);
void SYSCTRL_ClrReset_APB_PinCtrl(void);

void SYSCTRL_ClrReset_WDG_WorkClk(void);

void SYSCTRL_ClrReset_AHB(uint32_t reg_val);
void SYSCTRL_ClrReset_APB(uint32_t reg_val);
void SYSCTRL_ClrReset_WorkClk(int32_t reg_val);


//Block Reset Toggle
void SYSCTRL_TogReset_AHB_DES(void);
void SYSCTRL_TogReset_AHB_DMA(void);

void SYSCTRL_TogReset_APB_GPIO(void);
void SYSCTRL_TogReset_APB_WDG(void);
void SYSCTRL_TogReset_APB_TIMER0(void);
void SYSCTRL_TogReset_APB_TIMER1(void);
void SYSCTRL_TogReset_APB_TIMER2(void);
void SYSCTRL_TogReset_APB_SCI0(void);
void SYSCTRL_TogReset_APB_SCI1(void);
void SYSCTRL_TogReset_APB_SPI(void);
void SYSCTRL_TogReset_APB_I2C(void);
void SYSCTRL_TogReset_APB_PWM0(void);
void SYSCTRL_TogReset_APB_PWM1(void);
void SYSCTRL_TogReset_APB_PWM2(void);
void SYSCTRL_TogReset_APB_PWM3(void);
void SYSCTRL_TogReset_APB_ADC(void);
void SYSCTRL_TogReset_APB_LVD(void);
void SYSCTRL_TogReset_APB_PinCtrl(void);

void SYSCTRL_TogReset_WDG_WorkClk(void);


void SYSCTRL_ClrReset_Soft_Warm(void);
void SYSCTRL_ClrReset_Soft_Cold(void);

void SYSCTRL_ROSC_16M_PowerDown(void);
void SYSCTRL_ROSC_16M_Clr_PowerDown(void);
void SYSCTRL_ROSC_32K_PowerDown(void);
void SYSCTRL_ROSC_32K_Clr_PowerDown(void);

uint32_t SYSCTRL_Get_Chip_ID(void);
uint32_t SYSCTRL_Get_Chip_MOde(void);

#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif	/* __cplusplus */

#endif

