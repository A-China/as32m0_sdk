
#ifndef __AS32M0_DMA_H__
#define __AS32M0_DMA_H__


#ifdef	__cplusplus
extern "C" {	/* allow C++ to use these headers */
#endif	/* __cplusplus */

#include "as32m0.h"

/*
 * Description:
 *  DMA Controller - Configuration Register
 *
 */

#define bwDMA_CTRLR_ENABLED     1   /* DMA Ctrlr enabled flag */
#define bwDMA_AHB1_ENDIAN       1   /* AHB Bus 1 Endianness */
#define bwDMA_AHB2_ENDIAN       1   /* AHB Bus 2 Endianness */

#define bsDMA_CTRLR_ENABLED     0
#define bsDMA_AHB1_ENDIAN       1
#define bsDMA_AHB2_ENDIAN       2


/*
 * Description:
 *  DMA Controller - DMA Channel LLI Register
 *
 */

#define bwDMA_NEXT_LLI_BUS      1   /* AHB Bus of next LLI */
#define bwDMA_LLI_RESERVED      1   /* Reserved (set to 0 on write) */
#define bwDMA_NEXT_LLI          0x3fffffff  //30  /* Address of next LLI */

#define bsDMA_NEXT_LLI_BUS      0
#define bsDMA_LLI_RESERVED      1
#define bsDMA_NEXT_LLI          2

/*
 * Description:
 *  DMA Controller - DMA Channel Control Register
 *
 */

#define bwDMA_TRANSFER_SIZE        0xfff  //12   /* Transfer size */
#define bwDMA_SRC_BURST_SIZE       0x7    //3   /* Source Burst Size */
#define bwDMA_DEST_BURST_SIZE      0x7    //3   /* Destination Burst Size */
#define bwDMA_SRC_WIDTH            0x7    //3   /* Source Width */
#define bwDMA_DEST_WIDTH           0x7    //3   /* Destination Width */
#define bwDMA_SRC_BUS              1   /* Source AHB Bus */
#define bwDMA_DEST_BUS             1   /* Destination AHB Bus */
#define bwDMA_SRC_INCREMENT        1   /* Source auto Increment */
#define bwDMA_DEST_INCREMENT       1   /* Destination auto Increment */
#define bwDMA_PROTECTION           0x7    //3   /* Bus Protection Lines */
#define bwDMA_TC_INTERRUPT_ENABLE  1   /* TC Interrupt enable */

#define bsDMA_TRANSFER_SIZE        0
#define bsDMA_SRC_BURST_SIZE      12
#define bsDMA_DEST_BURST_SIZE     15
#define bsDMA_SRC_WIDTH           18
#define bsDMA_DEST_WIDTH          21
#define bsDMA_SRC_BUS             24
#define bsDMA_DEST_BUS            25
#define bsDMA_SRC_INCREMENT       26
#define bsDMA_DEST_INCREMENT      27
#define bsDMA_PROTECTION          28
#define bsDMA_TC_INTERRUPT_ENABLE 31

/*
 * Description:
 *  DMA Controller - DMA Channel Configuration Register
 *
 */

#define bwDMA_CHANNEL_ENABLED       1   /* Channel Enable */
#define bwDMA_SRC_PERIPHERAL        0xf 
#define bwDMA_DEST_PERIPHERAL       0xf 
#define bwDMA_FLOW_CONTROL          0x7  //3   /* Flow Control */
#define bwDMA_ERROR_INTERRUPT_MASK  1   /* Error Interrupt Mask */
#define bwDMA_TC_INTERRUPT_MASK     1   /* Terminal Count Interrupt Mask */
#define bwDMA_BUS_LOCK              1   /* Bus Lock */
#define bwDMA_ACTIVE                1   /* FIFO Active */
#define bwDMA_HALT                  1   /* Halt */
#define bwDMA_CONFIG_RESERVED       0x1fff //13  /* Reserved */

#define bsDMA_CHANNEL_ENABLED       0
#define bsDMA_SRC_PERIPHERAL        1
#define bsDMA_DEST_PERIPHERAL       6
#define bsDMA_FLOW_CONTROL          11
#define bsDMA_ERROR_INTERRUPT_MASK  14
#define bsDMA_TC_INTERRUPT_MASK     15
#define bsDMA_BUS_LOCK              16
#define bsDMA_ACTIVE                17
#define bsDMA_HALT                  18
#define bsDMA_CONFIG_RESERVED       19


#define   DMA_Bit_Clr    0
#define 	DMA_Bit_Set    1
	

#define   AHBMaster_1    0              /* AHB Bus 1 */
#define   AHBMaster_2    1              /* AHB Bus 2 */


#define    Ch_0        0             /* Channel 0 */
#define    Ch_1        1
#define    Ch_2        2
#define    Ch_3        3
#define    Ch_4        4
#define    Ch_5        5
#define    Ch_6        6
#define    Ch_7        7           



/************************/
typedef struct 
{
    uint32_t SrcAddr;
    uint32_t DstAddr;
    uint32_t NextLLI;
    uint32_t TransferCtrl;

} DMA_sRawLLI;


typedef struct 
{
  uint32_t        LLIAddr;
  uint8_t         LM;
  
} DMA_sChLLI;


/************************/



#define    DMA_PROT_USER              0     /* User access */
#define    DMA_PROT_SUPER             1     /* Supervisor access */

#define    DMA_PROT_NON_BUFFERABLE    0     /* Non bufferable data */
#define    DMA_PROT_BUFFERABLE        2     /* Bufferable data */

#define    DMA_PROT_NON_CACHEABLE     0     /* Non cacheable data */
#define    DMA_PROT_CACHEABLE         4     /* Cacheable data */



#define    DMA_WIDTH_8_BIT        0    /* 8 Bits per transfer */
#define    DMA_WIDTH_16_BIT       1    /* 16 Bits per transfer */
#define    DMA_WIDTH_32_BIT       2    /* 32 Bits per transfer */


#define    DMA_BURST_1        0    /* 1 transfer per burst */
#define    DMA_BURST_4        1    /* 4 transfers per burst */
#define    DMA_BURST_8        2    /* 8 transfers per burst */
#define    DMA_BURST_16       3    /* 16 transfers per burst */
#define    DMA_BURST_32       4    /* 32 transfers per burst */
#define    DMA_BURST_64       5    /* 64 transfers per burst */
#define    DMA_BURST_128      6    /* 128 transfers per burst */
#define    DMA_BURST_256      7     /* 256 transfers per burst */




typedef struct 
{
    uint8_t          TCIrqEn;
    uint8_t          Prot;
    uint8_t          DestInc;
    uint8_t          SourInc;
    uint8_t          DestBusSel;
    uint8_t          SourBusSel;
    uint8_t          DestWidth;
    uint8_t          SourWidth;
    uint8_t          DestBurstSize;
    uint8_t          SourBurstSize;
    uint32_t         TransferSize;
  
} DMA_sChCtrl;

/************************/

#define    DMA_MEM_TO_MEM_DMA_CTRL                             0
#define    DMA_MEM_TO_PERIPHERAL_DMA_CTRL                      1
#define    DMA_PERIPHERAL_TO_MEM_DMA_CTRL                      2
#define    DMA_PERIPHERAL_TO_PERIPHERAL_DMA_CTRL               3
#define    DMA_PERIPHERAL_TO_PERIPHERAL_DEST_PERIPHERAL_CTRL   4
#define    DMA_MEM_TO_PERIPHERAL_PERIPHERAL_CTRL               5
#define    DMA_PERIPHERAL_TO_MEM_PERIPHERAL_CTRL               6
#define    DMA_PERIPHERAL_TO_PERIPHERAL_SRC_PERIPHERAL_CTRL    7




typedef struct 
{
    uint8_t                Halt;
    uint8_t                ActiveChannel;  // RO, fifo is not empty
    uint8_t                LockTransfer;
    uint8_t                TCIrqMask;
    uint8_t                ErrorIrqMask;
    uint8_t                FlowCtrl;
    uint8_t                DestPeripheral;  // 0~15
    uint8_t                SourPeripheral;  // 0~15
    uint8_t                ChannelEnable;
  
} DMA_sChConfig;



/******************************************************/
uint32_t DMA_GetIntStatus(void);

uint32_t DMA_GetIntTCStatus(void);

void DMA_SetIntTCClear(uint8_t ch);

uint32_t DMA_GetIntErrorStatus(void);

void DMA_SetIntErrClr(uint8_t ch);

uint32_t DMA_GetRawIntTCStatus(void);

uint32_t DMA_GetRawIntErrorStatus(void);

uint32_t DMA_GetActiveChannels(void);

void DMA_SetSoftBReq(uint32_t id);

uint32_t DMA_GetSoftBReq(void);

void DMA_SetSoftSReq(uint32_t id);

uint32_t DMA_GetSoftSReq(void);

void DMA_SetSoftLBReq(uint32_t id);

uint32_t DMA_GetSoftLBReq(void);

void DMA_SetSoftLSReq(uint32_t id);

uint32_t DMA_GetSoftLSReq(void);

void DMA_BigEndian(uint8_t master);

void DMA_LittleEndian(uint8_t master);

void DMA_Enable(void);

void DMA_Disable(void);

uint32_t DMA_GetConfiguration(void);

void DMA_Enable_Sync(uint32_t id);

void DMA_Disable_Sync(uint32_t id);

uint32_t DMA_GetSync(void);

uint32_t DMA_GetSourAddr(uint8_t ch);

uint32_t DMA_GetDestAddr(uint8_t ch);

uint32_t DMA_SetChLLI(DMA_sChLLI* pChLLI);

void DMA_GetChLLI(DMA_sChLLI* pChLLI, uint8_t ch);

uint32_t DMA_SetChControl(DMA_sChCtrl* pChCtrl);

void DMA_GetChControl(DMA_sChCtrl* pChCtrl, uint8_t ch);

uint32_t DMA_SetChConfig(DMA_sChConfig* pChConfig);

void DMA_GetChConfig(DMA_sChConfig* pChConfig, uint8_t ch);

void DMA_SetChannelReg(uint32_t DestAddr, 
                       uint32_t SourADDR,
                       DMA_sChLLI* pChLLI,
                       DMA_sChCtrl* pChCtrl,
                       DMA_sChConfig* pChConfig,
                       uint8_t ch);
                       
void DMA_Data_Transfer(uint32_t DestAddr,
                       uint32_t SourADDR,
                       uint32_t TransLength,
                       uint32_t ChLLI_Addr, // LLI address
                       uint32_t Load_master, // 0- AHBMaster_1; 1- AHBMaster_2.
                       DMA_sChCtrl* pChCtrl,
                       DMA_sChConfig* pChConfig,
                       uint8_t ch);

#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif	/* __cplusplus */

#endif

