
#include "as32m0_sysctrl.h"
#include "as32m0.h"
// Clk Selection
void SYSCTRL_FastClockSwitch_ROSC(void)   {AHB_SYSCTRL->iClkSrcSel.CLR  = (1 << bsCLKSRCSEL_FAST_CLK_SEL);\
																					 APB_PINC->PD0.FuncSel=0xC0;}
void SYSCTRL_FastClockSwitch_EXT(void)    {APB_PINC->PD0.FuncSel=0xF0;\
																					 AHB_SYSCTRL->iClkSrcSel.SET  = (1 << bsCLKSRCSEL_FAST_CLK_SEL);}
void SYSCTRL_SlowClockSwitch_ROSC(void)   {AHB_SYSCTRL->iClkSrcSel.CLR  = (1 << bsCLKSRCSEL_SLOW_CLK_SEL);\
																					 APB_PINC->PD1.FuncSel=0xC0;}
void SYSCTRL_SlowClockSwitch_EXT(void)    {APB_PINC->PD1.FuncSel=0xF0;\
																					 AHB_SYSCTRL->iClkSrcSel.SET  = (1 << bsCLKSRCSEL_SLOW_CLK_SEL);}
void SYSCTRL_ApbClkSel_Sys(void)          {AHB_SYSCTRL->iClkCtrlSel.CLR = (1 << bsCLKCTRLSEL_APB_CLK_SEL);}
void SYSCTRL_ApbClkSel_SysDiv(void)       {AHB_SYSCTRL->iClkCtrlSel.SET = (1 << bsCLKCTRLSEL_APB_CLK_SEL);}
void SYSCTRL_SysClkSel_OSC(void)          {AHB_SYSCTRL->iClkCtrlSel.CLR = (1 << bsCLKCTRLSEL_SYS_CLK_SEL);}
void SYSCTRL_SysClkSel_OSCDiv(void)       {AHB_SYSCTRL->iClkCtrlSel.SET = (1 << bsCLKCTRLSEL_SYS_CLK_SEL);}
void SYSCTRL_OSCClkSel_FastClk(void)      {AHB_SYSCTRL->iClkCtrlSel.CLR = (1 << bsCLKCTRLSEL_OSC_CLK_SEL);}
void SYSCTRL_OSCClkSel_SlowClk(void)      {AHB_SYSCTRL->iClkCtrlSel.SET = (1 << bsCLKCTRLSEL_OSC_CLK_SEL);}

//Clk Div
void SYSCTRL_OSC_ClkDIv_Set(uint8_t div)  {AHB_SYSCTRL->iOscClkDiv = (div & bwOSCCLKDIV);}
void SYSCTRL_Sys_ClkDIv_Set(uint8_t div)  {AHB_SYSCTRL->iSysClkDiv = (div & bwSYSCLKDIV);}

//Clk Gate
void SYSCTRL_SetClkGate_AHB_DES(void)     {AHB_SYSCTRL->iAhbClkGate.SET  = (1 << bsAHBCLKGATE_DES_CLK_GATE    );}
void SYSCTRL_SetClkGate_AHB_DMA(void)     {AHB_SYSCTRL->iAhbClkGate.SET  = (1 << bsAHBCLKGATE_DMA_CLK_GATE    );}

void SYSCTRL_SetClkGate_APB_GPIO(void)    {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_GPIO_CLK_GATE   );}
void SYSCTRL_SetClkGate_APB_WDG(void)     {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_WDG_CLK_GATE    );}
void SYSCTRL_SetClkGate_APB_TIMER0(void)  {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_TIMER0_CLK_GATE );}
void SYSCTRL_SetClkGate_APB_TIMER1(void)  {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_TIMER1_CLK_GATE );}
void SYSCTRL_SetClkGate_APB_TIMER2(void)  {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_TIMER2_CLK_GATE );}
void SYSCTRL_SetClkGate_APB_SCI0(void)    {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_SCI0_CLK_GATE   );}
void SYSCTRL_SetClkGate_APB_SCI1(void)    {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_SCI1_CLK_GATE   );}
void SYSCTRL_SetClkGate_APB_SPI(void)     {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_SPI_CLK_GATE    );}
void SYSCTRL_SetClkGate_APB_I2C(void)     {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_I2C_CLK_GATE    );}
void SYSCTRL_SetClkGate_APB_PWM0(void)    {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_PWM0_CLK_GATE   );}
void SYSCTRL_SetClkGate_APB_PWM1(void)    {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_PWM1_CLK_GATE   );}
void SYSCTRL_SetClkGate_APB_PWM2(void)    {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_PWM2_CLK_GATE   );}
void SYSCTRL_SetClkGate_APB_PWM3(void)    {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_PWM3_CLK_GATE   );}
void SYSCTRL_SetClkGate_APB_ADC(void)     {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_ADC_CLK_GATE    );}
void SYSCTRL_SetClkGate_APB_LVD(void)     {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_LVD_CLK_GATE    );}
void SYSCTRL_SetClkGate_APB_PinCtrl(void) {AHB_SYSCTRL->iApbClkGate.SET  = (1 << bsAPBCLKGATE_PINCTRL_CLK_GATE);}
void SYSCTRL_SetClkGate_WDG_WorkClk(void) {AHB_SYSCTRL->iWorkClkGate.SET = (1 << bsWORKCLKGATE_WDG_CLK_GATE   );}

void SYSCTRL_SetAhbClkGate(uint32_t gate_val) {AHB_SYSCTRL->iAhbClkGate.WRITE = gate_val;}
void SYSCTRL_SetApbClkGate(uint32_t gate_val) {AHB_SYSCTRL->iApbClkGate.WRITE = gate_val;}
void SYSCTRL_SetWorkClkGate(uint32_t gate_val) {AHB_SYSCTRL->iWorkClkGate.WRITE = gate_val;}

uint32_t SYSCTRL_GetAhbClkGate(void)          {return AHB_SYSCTRL->iAhbClkGate.WRITE;}
uint32_t SYSCTRL_GetApbClkGate(void)          {return AHB_SYSCTRL->iApbClkGate.WRITE;}
uint32_t SYSCTRL_GetWorkClkGate(void)         {return AHB_SYSCTRL->iWorkClkGate.WRITE;}

//Clk Gate Clr
void SYSCTRL_ClrClkGate_AHB_DES(void)     {AHB_SYSCTRL->iAhbClkGate.CLR  = (1 << bsAHBCLKGATE_DES_CLK_GATE     );}
void SYSCTRL_ClrClkGate_AHB_DMA(void)	  {AHB_SYSCTRL->iAhbClkGate.CLR  = (1 << bsAHBCLKGATE_DMA_CLK_GATE     );}  
                                                                                                            
void SYSCTRL_ClrClkGate_APB_GPIO(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_GPIO_CLK_GATE    );}  
void SYSCTRL_ClrClkGate_APB_WDG(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_WDG_CLK_GATE     );}  
void SYSCTRL_ClrClkGate_APB_TIMER0(void)  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_TIMER0_CLK_GATE  );}
void SYSCTRL_ClrClkGate_APB_TIMER1(void)  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_TIMER1_CLK_GATE  );}  
void SYSCTRL_ClrClkGate_APB_TIMER2(void)  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_TIMER2_CLK_GATE  );}  
void SYSCTRL_ClrClkGate_APB_SCI0(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_SCI0_CLK_GATE    );}  
void SYSCTRL_ClrClkGate_APB_SCI1(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_SCI1_CLK_GATE    );}  
void SYSCTRL_ClrClkGate_APB_SPI(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_SPI_CLK_GATE     );}  
void SYSCTRL_ClrClkGate_APB_I2C(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_I2C_CLK_GATE     );}  
void SYSCTRL_ClrClkGate_APB_PWM0(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_PWM0_CLK_GATE    );}  
void SYSCTRL_ClrClkGate_APB_PWM1(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_PWM1_CLK_GATE    );}  
void SYSCTRL_ClrClkGate_APB_PWM2(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_PWM2_CLK_GATE    );}  
void SYSCTRL_ClrClkGate_APB_PWM3(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_PWM3_CLK_GATE    );}  
void SYSCTRL_ClrClkGate_APB_ADC(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_ADC_CLK_GATE     );}  
void SYSCTRL_ClrClkGate_APB_LVD(void)	  {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_LVD_CLK_GATE     );}  
void SYSCTRL_ClrClkGate_APB_PinCtrl(void) {AHB_SYSCTRL->iApbClkGate.CLR  = (1 << bsAPBCLKGATE_PINCTRL_CLK_GATE );}
void SYSCTRL_ClrClkGate_WDG_WorkClk(void) {AHB_SYSCTRL->iWorkClkGate.CLR = (1 << bsWORKCLKGATE_WDG_CLK_GATE    );}

void SYSCTRL_ClrAhbClkGate(uint32_t gate_val) {AHB_SYSCTRL->iAhbClkGate.CLR = gate_val;}
void SYSCTRL_ClrApbClkGate(uint32_t gate_val) {AHB_SYSCTRL->iApbClkGate.CLR = gate_val;}
void SYSCTRL_ClrWorkClkGate(uint32_t gate_val) {AHB_SYSCTRL->iWorkClkGate.CLR = gate_val;}

//Clk Gate Toggle
void SYSCTRL_TogClkGate_AHB_DES(void)     {AHB_SYSCTRL->iAhbClkGate.MODIFY  = (1 << bsAHBCLKGATE_DES_CLK_GATE     );}
void SYSCTRL_TogClkGate_AHB_DMA(void)	  {AHB_SYSCTRL->iAhbClkGate.MODIFY  = (1 << bsAHBCLKGATE_DMA_CLK_GATE     );}  
                                                                                                      
void SYSCTRL_TogClkGate_APB_GPIO(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_GPIO_CLK_GATE    );}  
void SYSCTRL_TogClkGate_APB_WDG(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_WDG_CLK_GATE     );}  
void SYSCTRL_TogClkGate_APB_TIMER0(void)  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_TIMER0_CLK_GATE  );}
void SYSCTRL_TogClkGate_APB_TIMER1(void)  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_TIMER1_CLK_GATE  );}  
void SYSCTRL_TogClkGate_APB_TIMER2(void)  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_TIMER2_CLK_GATE  );}  
void SYSCTRL_TogClkGate_APB_SCI0(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_SCI0_CLK_GATE    );}  
void SYSCTRL_TogClkGate_APB_SCI1(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_SCI1_CLK_GATE    );}  
void SYSCTRL_TogClkGate_APB_SPI(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_SPI_CLK_GATE     );}  
void SYSCTRL_TogClkGate_APB_I2C(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_I2C_CLK_GATE     );}  
void SYSCTRL_TogClkGate_APB_PWM0(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_PWM0_CLK_GATE    );}  
void SYSCTRL_TogClkGate_APB_PWM1(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_PWM1_CLK_GATE    );}  
void SYSCTRL_TogClkGate_APB_PWM2(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_PWM2_CLK_GATE    );}  
void SYSCTRL_TogClkGate_APB_PWM3(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_PWM3_CLK_GATE    );}  
void SYSCTRL_TogClkGate_APB_ADC(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_ADC_CLK_GATE     );}  
void SYSCTRL_TogClkGate_APB_LVD(void)	  {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_LVD_CLK_GATE     );}  
void SYSCTRL_TogClkGate_APB_PinCtrl(void) {AHB_SYSCTRL->iApbClkGate.MODIFY  = (1 << bsAPBCLKGATE_PINCTRL_CLK_GATE );}
void SYSCTRL_TogClkGate_WDG_WorkClk(void) {AHB_SYSCTRL->iWorkClkGate.MODIFY = (1 << bsWORKCLKGATE_WDG_CLK_GATE    );}




//Block Reset
void SYSCTRL_SetReset_AHB_DES(void)       {AHB_SYSCTRL->iAhbBlkRst.SET = (1 << bsAHBBLKRST_DES_BLK_RST     );}
void SYSCTRL_SetReset_AHB_DMA(void)       {AHB_SYSCTRL->iAhbBlkRst.SET = (1 << bsAHBBLKRST_DMA_BLK_RST     );}

void SYSCTRL_SetReset_APB_GPIO(void)      {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_GPIO_BLK_RST    );}
void SYSCTRL_SetReset_APB_WDG(void)       {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_WDG_BLK_RST     );}
void SYSCTRL_SetReset_APB_TIMER0(void)    {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_TIMER0_BLK_RST  );}
void SYSCTRL_SetReset_APB_TIMER1(void)    {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_TIMER1_BLK_RST  );}
void SYSCTRL_SetReset_APB_TIMER2(void)	  {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_TIMER2_BLK_RST  );}
void SYSCTRL_SetReset_APB_SCI0(void)	  {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_SCI0_BLK_RST    );}
void SYSCTRL_SetReset_APB_SCI1(void)	  {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_SCI1_BLK_RST    );}
void SYSCTRL_SetReset_APB_SPI(void)       {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_SPI_BLK_RST     );}
void SYSCTRL_SetReset_APB_I2C(void)       {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_I2C_BLK_RST     );}
void SYSCTRL_SetReset_APB_PWM0(void)	  {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_PWM0_BLK_RST    );}
void SYSCTRL_SetReset_APB_PWM1(void)	  {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_PWM1_BLK_RST    );}
void SYSCTRL_SetReset_APB_PWM2(void)	  {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_PWM2_BLK_RST    );}
void SYSCTRL_SetReset_APB_PWM3(void)	  {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_PWM3_BLK_RST    );}
void SYSCTRL_SetReset_APB_ADC(void)       {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_ADC_BLK_RST     );}
void SYSCTRL_SetReset_APB_LVD(void)       {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_LVD_BLK_RST     );}
void SYSCTRL_SetReset_APB_PinCtrl(void)	  {AHB_SYSCTRL->iApbBlkRst.SET = (1 << bsAPBBLKRST_PINCTRL_BLK_RST );}

void SYSCTRL_SetReset_WDG_WorkClk(void)	  {AHB_SYSCTRL->iWorkBlkRst.SET = (1<< bsWORKBLKRST_WDG_BLK_RST)    ;}

void SYSCTRL_SetReset_Soft_Warm(void)	  {AHB_SYSCTRL->iSoftWarmReset |= (1 << bsSOFTWARMRESET);}
void SYSCTRL_SetReset_Soft_Cold(void)	  {AHB_SYSCTRL->iSoftColdReset |= (1 << bsSOFTCOLDRESET);}

void SYSCTRL_SetReset_AHB(uint32_t reg_val)    {AHB_SYSCTRL->iAhbBlkRst.WRITE  = reg_val;}
void SYSCTRL_SetReset_APB(uint32_t reg_val)    {AHB_SYSCTRL->iApbBlkRst.WRITE  = reg_val;}
void SYSCTRL_SetReset_WorkClk(int32_t reg_val) {AHB_SYSCTRL->iWorkBlkRst.WRITE = reg_val;}

uint32_t SYSCTRL_GetReset_AHB(void)            {return AHB_SYSCTRL->iAhbBlkRst.WRITE ;}
uint32_t SYSCTRL_GetReset_APB(void)            {return AHB_SYSCTRL->iApbBlkRst.WRITE ;}
uint32_t SYSCTRL_GetReset_WorkClk(void)        {return AHB_SYSCTRL->iWorkBlkRst.WRITE;}


//Block Reset Clear
void SYSCTRL_ClrReset_AHB_DES(void)       {AHB_SYSCTRL->iAhbBlkRst.CLR  = (1 << bsAHBBLKRST_DES_BLK_RST     );}
void SYSCTRL_ClrReset_AHB_DMA(void)       {AHB_SYSCTRL->iAhbBlkRst.CLR  = (1 << bsAHBBLKRST_DMA_BLK_RST     );}

void SYSCTRL_ClrReset_APB_GPIO(void)      {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_GPIO_BLK_RST    );}
void SYSCTRL_ClrReset_APB_WDG(void)       {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_WDG_BLK_RST     );}
void SYSCTRL_ClrReset_APB_TIMER0(void)    {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_TIMER0_BLK_RST  );}
void SYSCTRL_ClrReset_APB_TIMER1(void)    {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_TIMER1_BLK_RST  );}
void SYSCTRL_ClrReset_APB_TIMER2(void)    {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_TIMER2_BLK_RST  );}
void SYSCTRL_ClrReset_APB_SCI0(void)      {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_SCI0_BLK_RST    );}        
void SYSCTRL_ClrReset_APB_SCI1(void)      {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_SCI1_BLK_RST    );}
void SYSCTRL_ClrReset_APB_SPI(void)       {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_SPI_BLK_RST     );}
void SYSCTRL_ClrReset_APB_I2C(void)       {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_I2C_BLK_RST     );}
void SYSCTRL_ClrReset_APB_PWM0(void)      {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_PWM0_BLK_RST    );}
void SYSCTRL_ClrReset_APB_PWM1(void)      {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_PWM1_BLK_RST    );}
void SYSCTRL_ClrReset_APB_PWM2(void)      {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_PWM2_BLK_RST    );}
void SYSCTRL_ClrReset_APB_PWM3(void)      {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_PWM3_BLK_RST    );}
void SYSCTRL_ClrReset_APB_ADC(void)       {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_ADC_BLK_RST     );}
void SYSCTRL_ClrReset_APB_LVD(void)       {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_LVD_BLK_RST     );}
void SYSCTRL_ClrReset_APB_PinCtrl(void)	  {AHB_SYSCTRL->iApbBlkRst.CLR  = (1 << bsAPBBLKRST_PINCTRL_BLK_RST );}

void SYSCTRL_ClrReset_WDG_WorkClk(void)	  {AHB_SYSCTRL->iWorkBlkRst.CLR = (1 << bsWORKBLKRST_WDG_BLK_RST    );}
                                                                                                             

void SYSCTRL_ClrReset_AHB(uint32_t reg_val)    {AHB_SYSCTRL->iAhbBlkRst.CLR  = reg_val;}
void SYSCTRL_ClrReset_APB(uint32_t reg_val)    {AHB_SYSCTRL->iApbBlkRst.CLR  = reg_val;}
void SYSCTRL_ClrReset_WorkClk(int32_t reg_val) {AHB_SYSCTRL->iWorkBlkRst.CLR = reg_val;}


//Block Reset Toggle
void SYSCTRL_TogReset_AHB_DES(void)       {AHB_SYSCTRL->iAhbBlkRst.MODIFY  = (1 << bsAHBBLKRST_DES_BLK_RST     );}
void SYSCTRL_TogReset_AHB_DMA(void)       {AHB_SYSCTRL->iAhbBlkRst.MODIFY  = (1 << bsAHBBLKRST_DMA_BLK_RST     );}
                                                             
void SYSCTRL_TogReset_APB_GPIO(void)      {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_GPIO_BLK_RST    );}
void SYSCTRL_TogReset_APB_WDG(void)       {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_WDG_BLK_RST     );}
void SYSCTRL_TogReset_APB_TIMER0(void)    {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_TIMER0_BLK_RST  );}
void SYSCTRL_TogReset_APB_TIMER1(void)    {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_TIMER1_BLK_RST  );}
void SYSCTRL_TogReset_APB_TIMER2(void)    {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_TIMER2_BLK_RST  );}
void SYSCTRL_TogReset_APB_SCI0(void)      {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_SCI0_BLK_RST    );}        
void SYSCTRL_TogReset_APB_SCI1(void)      {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_SCI1_BLK_RST    );}
void SYSCTRL_TogReset_APB_SPI(void)       {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_SPI_BLK_RST     );}
void SYSCTRL_TogReset_APB_I2C(void)       {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_I2C_BLK_RST     );}
void SYSCTRL_TogReset_APB_PWM0(void)      {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_PWM0_BLK_RST    );}
void SYSCTRL_TogReset_APB_PWM1(void)      {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_PWM1_BLK_RST    );}
void SYSCTRL_TogReset_APB_PWM2(void)      {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_PWM2_BLK_RST    );}
void SYSCTRL_TogReset_APB_PWM3(void)      {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_PWM3_BLK_RST    );}
void SYSCTRL_TogReset_APB_ADC(void)       {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_ADC_BLK_RST     );}
void SYSCTRL_TogReset_APB_LVD(void)       {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_LVD_BLK_RST     );}
void SYSCTRL_TogReset_APB_PinCtrl(void)	  {AHB_SYSCTRL->iApbBlkRst.MODIFY  = (1 << bsAPBBLKRST_PINCTRL_BLK_RST );}

void SYSCTRL_TogReset_WDG_WorkClk(void)	  {AHB_SYSCTRL->iWorkBlkRst.MODIFY = (1 << bsWORKBLKRST_WDG_BLK_RST    );}

void SYSCTRL_ClrReset_Soft_Warm(void)     {AHB_SYSCTRL->iSoftWarmReset |= (1 << bsSOFTWARMRESET);}
void SYSCTRL_ClrReset_Soft_Cold(void)     {AHB_SYSCTRL->iSoftColdReset |= (1 << bsSOFTCOLDRESET);}


void SYSCTRL_ROSC_16M_Set(uint8_t trimming)     {AHB_SYSCTRL->iROscCtrl.CLR = bwROSCCTRL_ROSC16M_TIMING_SET; \
                                                 AHB_SYSCTRL->iROscCtrl.SET = trimming & bwROSCCTRL_ROSC16M_TIMING_SET; }
void SYSCTRL_ROSC_16M_Trimming_Sel(void)        {AHB_SYSCTRL->iROscCtrl.SET = (1 << bsROSCCTRL_ROSC16M_SELECTION);}
void SYSCTRL_ROSC_16M_PowerDown(void)           {AHB_SYSCTRL->iROscCtrl.SET = (1 << bsROSCCTRL_ROSC16M_POWERDOWN);}
void SYSCTRL_ROSC_32K_PowerDown(void)           {AHB_SYSCTRL->iROscCtrl.SET = (1 << bsROSCCTRL_ROSC32k_POWERDOWN);}

void SYSCTRL_ROSC_16M_Clr_PowerDown(void)       {AHB_SYSCTRL->iROscCtrl.CLR = (1 << bsROSCCTRL_ROSC16M_POWERDOWN);}
void SYSCTRL_ROSC_32K_Clr_PowerDown(void)       {AHB_SYSCTRL->iROscCtrl.CLR = (1 << bsROSCCTRL_ROSC32k_POWERDOWN);}


uint32_t SYSCTRL_Get_Chip_ID(void)              {return AHB_SYSCTRL->iChipID;   }
uint32_t SYSCTRL_Get_Chip_MOde(void)            {return AHB_SYSCTRL->iChipMode; }



