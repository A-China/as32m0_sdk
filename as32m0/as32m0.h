

#ifndef __AS32M0_H__
#define __AS32M0_H__

#ifdef __cplusplus
 extern "C" {
 
#endif
typedef enum IRQn
{
/* -------------------  Cortex-M4 Processor Exceptions Numbers  ------------------- */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt          */
  HardFault_IRQn                = -13,      /*!<  3 HardFault Interrupt             */
  MemoryManagement_IRQn         = -12,      /*!<  4 Memory Management Interrupt     */
  BusFault_IRQn                 = -11,      /*!<  5 Bus Fault Interrupt             */
  UsageFault_IRQn               = -10,      /*!<  6 Usage Fault Interrupt           */
  SVCall_IRQn                   =  -5,      /*!< 11 SV Call Interrupt               */
  DebugMonitor_IRQn             =  -4,      /*!< 12 Debug Monitor Interrupt         */
  PendSV_IRQn                   =  -2,      /*!< 14 Pend SV Interrupt               */
  SysTick_IRQn                  =  -1,      /*!< 15 System Tick Interrupt           */

/******  CM32GPM4 specific Interrupt Numbers *********************************************************/
  n00_WDG_IRQn                  =  0 ,
  n01_DMA_IRQn                  =  1 ,
  n02_ADC_IRQn			        		=  2 ,
  n03_LVD_IRQn                  =  3 ,
  n04_PWM0_IRQn                 =  4 ,
  n05_PWM1_IRQn                 =  5 ,
  n06_PWM2_IRQn                 =  6 ,
  n07_PWM3_IRQn                 =  7 ,
  n08_UART0_IRQn                =  8 ,
  n09_UART1_IRQn                =  9 ,
  n10_TMR0_IRQn                 =  10,
  n11_TMR1_IRQn                 =  11,
  n12_TMR2_IRQn                 =  12,
  n13_I2C_IRQn                  =  13,
  n14_SPI_IRQn                  =  14,
  n15_GIO_IRQn                  =  15,
  n16_flash_IRQn                =  16
} IRQn_Type;


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* --------  Configuration of the Cortex-M0 Processor and Core Peripherals  ------- */
#define __CM0_REV                 0x0201    /*!< Core revision r2p1                              */
#define __MPU_PRESENT             0         /*!< MPU present or not                              */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels         */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used    */
#define  __FPU_PRESENT   					0
#include <core_cm0.h>                       /* Processor and core peripherals                    */
#include <stdio.h>

/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */

/* -------------------  Start of section using anonymous unions  ------------------ */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler type
#endif
// Flash Registers
typedef struct {
	__IO uint32_t  Chip_Erase ; //0x00
	__IO uint32_t  SWD_Unlock ; //0x04
	__IO uint32_t  Flash_Unlock;//0x08
	__IO uint32_t  Write_Mode ; //0x0c
	__IO uint32_t  Read_Delay ; //0x10
	__IO uint32_t  Pin_Set    ; //0x14
	__IO uint32_t  Timer_Set  ; //0x18
	__IO uint32_t  Timing_Set0; //0x1C
	__IO uint32_t  Timing_Set1; //0x20
	__IO uint32_t  Timing_Set2; //0x24
	__IO uint32_t  Timing_Set3; //0x28
	__IO uint32_t  Timing_Set4; //0x2C
	__IO uint32_t  Interrupt  ; //0x30
    __IO uint32_t  Remap      ; //0x34
} FLASH_TypeDef;


// i2c

typedef struct
{
  __IO uint32_t     IC_CON           ; //0x00
  __IO uint32_t     IC_TAR           ; //0x04
  __IO uint32_t     IC_SAR           ; //0x08
  __IO uint32_t     IC_HS_MADDR      ; //0x0c
  __IO uint32_t     IC_DATA_CMD      ; //0x10
  __IO uint32_t     IC_SS_SCL_HCNT   ; //0x14
  __IO uint32_t     IC_SS_SCL_LCNT   ; //0x18
  __IO uint32_t     IC_FS_SCL_HCNT   ; //0x1c
  __IO uint32_t     IC_FS_SCL_LCNT   ; //0x20
  __IO uint32_t     IC_HS_SCL_HCNT   ; //0x24
  __IO uint32_t     IC_HS_SCL_LCNT   ; //0X28      
  __IO uint32_t     IC_INTR_STAT     ; //0x2C      
  __IO uint32_t     IC_INTR_MASK     ; //0X30
  __IO uint32_t     IC_RAW_INTR_STAT ; //0X34
  __IO uint32_t     IC_RX_TL         ; //0X38
  __IO uint32_t     IC_TX_TL         ; //0x3C    
  __IO uint32_t     IC_CLR_INTR      ; //0x40
  __IO uint32_t     IC_CLR_RX_UNDER  ; //0x44      
  __IO uint32_t     IC_CLR_RX_OVER   ; //0x48      
  __IO uint32_t     IC_CLR_TX_OVER   ; //0x4c      
  __IO uint32_t     IC_CLR_RD_REQ    ; //0x50      
  __IO uint32_t     IC_CLR_TX_ABRT   ; //0x54      
  __IO uint32_t     IC_CLR_RX_DONE   ; //0x58      
  __IO uint32_t     IC_CLR_ACTIVITY  ; //0x5c      
  __IO uint32_t     IC_CLR_STOP_DET  ; //0x60      
  __IO uint32_t     IC_CLR_START_DET ; //0x64      
  __IO uint32_t     IC_CLR_GEN_CALL  ; //0x68      
  __IO uint32_t     IC_ENABLE        ; //0x6c      
  __IO uint32_t     IC_STATUS        ; //0x70      
  __IO uint32_t     IC_TXFLR         ; //0x74      
  __IO uint32_t     IC_RXFLR         ; //0x78      
  __IO uint32_t     IC_SDA_HOLD      ; //0x7c      
  __IO uint32_t     IC_TX_ABRT_SOURCE                ; //0x80      
  __IO uint32_t     IC_SLV_DATA_NACK_ONLY            ; //0x84      
  __IO uint32_t     IC_DMA_CR                        ; //0x88      
  __IO uint32_t     IC_DMA_TDLR                      ; //0x8c      
  __IO uint32_t     IC_DMA_RDLR                      ; //0x90      
  __IO uint32_t     IC_SDA_SETUP                     ; //0x94      
  __IO uint32_t     IC_ACK_GENERAL_CALL              ; //0x98      
  __IO uint32_t     IC_ENABLE_STATUS                 ; //0x9C      
  __IO uint32_t     IC_FS_SPKLEN                     ; //0xa0          
  __IO uint32_t     IC_HS_SPKLEN                     ; //0xa4
  __IO uint32_t     Reserved1               				 ; //0xa8     
  __IO uint32_t     Reserved2      									 ; //0xac      
  __IO uint32_t     Reserved3      									 ; //0xb0      
  __IO uint32_t     Reserved4             					 ; //0xb4      
  __IO uint32_t     Reserved5                     	 ; //0xb8      
  __IO uint32_t     Reserved6            						 ; //0xbc      
  __IO uint32_t     Reserved7            						 ; //0xc0      
  __IO uint32_t     Reserved8    										 ; //0xc4      
  __IO uint32_t     Reserved9               				 ; //0xc8      
  __IO uint32_t     Reserved10               				 ; //0xcc      
  __IO uint32_t     Reserved11           						 ; //0xd0      
  __IO uint32_t     Reserved12                			 ; //0xd4      
  __IO uint32_t     Reserved13                  		 ; //0xd8      
  __IO uint32_t     Reserved14                			 ; //0xdc
  __IO uint32_t     Reserved15                       ; //0xe0    
  __IO uint32_t     Reserved16                       ; //0xe4     
  __IO uint32_t     Reserved17                       ; //0xe8    
  __IO uint32_t     Reserved18                       ; //0xec    
  __IO uint32_t     Reserved19                       ; //0xf0    
  __IO uint32_t     IC_COMP_PARAM_1                  ; //0xf4     
  __IO uint32_t     IC_COMP_VERSION                  ; //0xf8       
  __IO uint32_t     IC_COMP_TYPE                     ; //0xfc    
} I2C_TypeDef;

// adc
typedef struct
{
  __IO uint32_t     ADC_Reset          ; //0x00
  __IO uint32_t     ADC_Data           ; //0x04
  __IO uint32_t     ADC_Clock_Div      ; //0x08
  __IO uint32_t     ADC_Mode           ; //0x0c
  __IO uint32_t     ADC_Soc_Ctrl       ; //0x10
  __IO uint32_t     ADC_Config         ; //0x14
  __IO uint32_t     ADC_PD             ; //0x18
  __IO uint32_t     ADC_Sample_Num     ; //0x1c
  __IO uint32_t     ADC_Inter_Enable   ; //0x20
  __IO uint32_t     ADC_Valid_intr     ; //0x24
  __IO uint32_t     ADC_Overflow_intr  ; //0x28
  __IO uint32_t     ADC_Busy           ; //0x2c
  __IO uint32_t     ADC_Clk_Gate       ; //0x30

} ADC_TypeDef;

// PWM
typedef struct
{
  __IO uint32_t     PWM_M_CTRL         ; //0x00
  __IO uint32_t     PWM_CH0_CTRL       ; //0x04
  __IO uint32_t     PWM_CH1_CTRL       ; //0x08
  __IO uint32_t     PWM_CH0_PER        ; //0x0c
  __IO uint32_t     PWM_CH1_PER        ; //0x10
  __IO uint32_t     PWM_PERIOD         ; //0x14
  __IO uint32_t     PWM_INT            ; //0x18
  __IO uint32_t     PWM_CMD_ST         ; //0x1c
} PWM_TypeDef;


//uart
typedef struct
{
  __IO uint32_t  DataRead;                    // 0x00
  __IO uint32_t  StatusClear;                 // 0x04
  __IO uint32_t  Reserved1;                   // 0x08
  __IO uint32_t  Reserved2;                   // 0x0C
  __IO uint32_t  Reserved3;                   // 0x10
  __IO uint32_t  Reserved4;                   // 0x14
  __IO uint32_t  Flag;                        // 0x18
  __IO uint32_t  Reserved5;                   // 0x1C
  __IO uint32_t  LowPower;                    // 0x20
  __IO uint32_t  IntBaudDivisor;              // 0x24
  __IO uint32_t  FractBaudDivisor;            // 0x28
  __IO uint32_t  LineCon_H;                   // 0x2C
  __IO uint32_t  Control;                     // 0x30
  __IO uint32_t  FifoSelect;                  // 0x34
  __IO uint32_t  IntMask;                     // 0x38
  __IO uint32_t  IntRaw;                      // 0x3C
  __IO uint32_t  Interrupt;                   // 0x40  // Masked Interrupt
  __IO uint32_t  IntClear;                    // 0x44
  __IO uint32_t  DmaCon;                      // 0x48
} UART_TypeDef;


// timer
typedef struct
{
  __IO uint32_t CNT;
  __IO uint32_t CMP;
  __IO uint32_t CTL;
  __IO uint32_t LOCK;
} TMR_TypeDef;


//gpio
typedef struct{
    __IO uint32_t WRITE; 
    __IO uint32_t SET  ;
    __IO uint32_t CLR  ;
    __IO uint32_t MODIFY;
} GPIO_RegDef;

typedef struct
{
  __IO uint32_t          GPIO_DI;       //0x4000_0000                
  __IO uint32_t             rev0;       //0x004                 
  __IO uint32_t             rev1;       //0x008                   
  __IO uint32_t             rev2;       //0x00C                    
  GPIO_RegDef 	         GPIO_DO;       //0x4000_0010                  
  GPIO_RegDef 	         GPIO_OE;       //0x4000_0020                        
  GPIO_RegDef	            	rev3;       //0x4000_0030                      
  GPIO_RegDef 	         GPIO_IS;       //0x4000_0040                         
  GPIO_RegDef 	         GPIO_IT;       //0x4000_0050    /@interrupt type                
  GPIO_RegDef 	         GPIO_PE;       //0x4000_0060                  
  GPIO_RegDef            GPIO_NE;       //0x4000_0070                   
  GPIO_RegDef    GPIO_DEBOUNC_EN;       //0x080
  GPIO_RegDef   GPIO_DEBOUNC_LMT;       //0x090
  GPIO_RegDef       GPIO_LV_CTN0;       //0x0A0
  GPIO_RegDef       GPIO_PE_CTN0;       //0x0B0
  GPIO_RegDef       GPIO_NE_CTN0;       //0x0C0
  GPIO_RegDef       GPIO_LV_CTN1;       //0x0D0
  GPIO_RegDef       GPIO_PE_CTN1;       //0x0E0
  GPIO_RegDef       GPIO_NE_CTN1;       //0x0F0
  GPIO_RegDef       GPIO_LV_CTN2;       //0x100
  GPIO_RegDef       GPIO_PE_CTN2;       //0x110
  GPIO_RegDef       GPIO_NE_CTN2;       //0x120
  GPIO_RegDef    GPIO_DEBOUNC_BEN;      //0x130
} GPIO_TypeDef;

 
// dma
typedef struct
{
	__IO uint32_t ChSrcAddr;
	__IO uint32_t ChDestAddr;
	__IO uint32_t ChLLI;
	__IO uint32_t ChControl;
	__IO uint32_t ChConfiguration;
	__IO uint32_t Reserved1;
  __IO uint32_t Reserved2;
  __IO uint32_t Reserved3;
  
} DMA_ChDef;

#define DMA_NUM_RSRVD_WRDS_BEFORE_CHANNELS      ( (0x100 - 0x038) >> 2 )
#define DMA_NUM_RSRVD_WRDS_BEFORE_PERIPHERAL_ID ( (0xfe0 - 0x1f4) >> 2 )

typedef struct
{
	  __IO uint32_t         IntStatus;              /* 0x000 */  // RO
    __IO uint32_t         IntTCStatus;                         // RO
    __IO uint32_t         IntTCClear;                          // WO
    __IO uint32_t         IntErrorStatus;                      // RO
    __IO uint32_t         IntErrorClear;          /* 0x010 */  // WO
    __IO uint32_t         RawIntTCStatus;                      // RO
    __IO uint32_t         RawIntErrorStatus;                   // RO
    __IO uint32_t         ActiveChannels;                      // RO, A bit is cleared on completion of the DMA transfer.

    __IO uint32_t         SoftBReq;               /* 0x020 */  // RW
    __IO uint32_t         SoftSReq;               /* 0x024 */  // RW
    __IO uint32_t         SoftLBReq;              /* 0x028 */  // RW
    __IO uint32_t         SoftSBReq;              /* 0x02C */  // RW
    
    __IO uint32_t         Configuration;          /* 0x030 */  // RW
    __IO uint32_t         Sync;                   /* 0x034 */  // RW

    __IO uint32_t         Reserved1[ DMA_NUM_RSRVD_WRDS_BEFORE_CHANNELS ];  // RW

    DMA_ChDef             ChReg[7];     /* 0x100 -  */

    __IO uint32_t         Reserved2[ DMA_NUM_RSRVD_WRDS_BEFORE_PERIPHERAL_ID ];

    __IO uint32_t         PeripheralId0;          /* 0xFE0 */
    __IO uint32_t         PeripheralId1;
    __IO uint32_t         PeripheralId2;
    __IO uint32_t         PeripheralId3;
    __IO uint32_t         CellId0;                /* 0xFF0 */
    __IO uint32_t         CellId1;
    __IO uint32_t         CellId2;
    __IO uint32_t         CellId3;
	
} DMA_TypeDef;

// System Control
typedef struct{
    __IO uint32_t WRITE;//common write 
    __IO uint32_t SET  ;//bit set
    __IO uint32_t CLR  ;//bit clr
    __IO uint32_t MODIFY;//bit toggle
} SYSCTRL_RegDef;

typedef struct{
    // 0x00
    SYSCTRL_RegDef    iClkSrcSel;          //0x00
    SYSCTRL_RegDef    iClkCtrlSel;         //0x10
    __IO uint32_t     iOscClkDiv;          //0x20
    __IO uint32_t     iSysClkDiv;          //0x24
    __IO uint32_t     Reserved0;           //0x28
    __IO uint32_t     Reserved1;           //0x2C
    SYSCTRL_RegDef    iAhbClkGate;         //0x30
    SYSCTRL_RegDef    iApbClkGate;         //0x40
    SYSCTRL_RegDef    iWorkClkGate;        //0x50
    SYSCTRL_RegDef    iAhbBlkRst;          //0x60
    SYSCTRL_RegDef    iApbBlkRst;          //0x70
    SYSCTRL_RegDef    iWorkBlkRst;         //0x80
    __IO uint32_t     iSoftWarmReset;      //0x90
    __IO uint32_t     iSoftColdReset;      //0x94
    __IO uint32_t     iChipID;             //0x98
    __IO uint32_t     Reserved2;           //0x9C
    SYSCTRL_RegDef    iROscCtrl;           //0xA0
    __IO uint32_t     iChipMode;           //0xB0
    __IO uint32_t     iDbgSel;             //0xB4
    __IO uint32_t     iMBIST;              //0xB8
    __IO uint32_t     iSCI0;               //0xBC
    __IO uint32_t     iSCI1;               //0xC0		
} SYSCTRL_TypeDef;

// Apb Pin Control
typedef struct{
		__IO uint8_t  PadCtrl;
		__IO uint8_t  FuncSel;
    __IO uint8_t  Reserved1; 
    __IO uint8_t  Reserved2;
} PINC_RegDef;

typedef struct{		                          
		PINC_RegDef PA0     ; 
		PINC_RegDef PA1     ; 
		PINC_RegDef PA2     ; 
		PINC_RegDef PA3     ; 
		PINC_RegDef PA4     ; 
		PINC_RegDef PA5     ; 
		PINC_RegDef PA6     ; 
		PINC_RegDef PA7     ; 
	
		PINC_RegDef PB0     ; 
		PINC_RegDef PB1     ; 
		PINC_RegDef PB2     ; 
		PINC_RegDef PB3     ; 
		PINC_RegDef PB4     ; 
		PINC_RegDef PB5     ; 
		PINC_RegDef PB6     ; 
		PINC_RegDef PB7     ; 
	
		PINC_RegDef PC0     ; 
		PINC_RegDef PC1     ; 
		PINC_RegDef PC2     ; 
		PINC_RegDef PC3     ; 
		PINC_RegDef PC4     ; 
		PINC_RegDef PC5     ; 
		PINC_RegDef PC6     ; 
		PINC_RegDef PC7     ; 
		
		PINC_RegDef PD0     ; 
		PINC_RegDef PD1     ; 
		PINC_RegDef PD2     ; 
		PINC_RegDef PD3     ; 
} PINC_TypeDef;

// spi
typedef struct 
{
  __IO uint32_t ControlRegister0;           /* SSPCR0 Address SSP Base + 0x00 */
  __IO uint32_t ControlRegister1;           /* SSPCR1 Address SSP Base + 0x04 */
  __IO uint32_t DataRegister;               /* SSPDR Address SSP Base + 0x08 */
  __IO uint32_t StatusRegister;             /* SSPSR Address SSP Base + 0x0C */
  __IO uint32_t ClockPreScale;              /* SSPCPSR Address SSP Base + 0x10 */
  __IO uint32_t IntMask;                    /* SSPIMSC Interrupt mask set and clear register */
  __IO uint32_t IntRawStatus;               /* SSPRIS Raw interrupt status register */
  __IO uint32_t IntMaskedStatus;            /* SSPMIS Maked interrupt status register */
  __IO uint32_t IntClearRegister;           /* SSPICR Interrupt clear register */
  __IO uint32_t DMAControlReg;              /* SSPDMACR DMA control register */
} SSP_TypeDef;

typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t ST;
  __IO uint32_t SET;
  __IO uint32_t FLT;
}LVD_TypeDef;

/* --------------------  End of section using anonymous unions  ------------------- */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
  /* leave anonymous unions enabled */
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler type
#endif

/******************************************************************************/
/*                         memory map                                         */
/******************************************************************************/
// AHB
#define ROM_SIZE           	((uint32_t)0x5000UL)
#define SRAM_SIZE          	((uint32_t)0x1000UL)

#define ROM_BASE           	((uint32_t)0x00000000UL)
#define SRAM_MIRROR_BASE   	((uint32_t)0x00040000UL)
#define SRAM_BASE          	((uint32_t)0x20000000UL)
#define ROM_MIRROR_BASE    	((uint32_t)0x21000000UL)

#define APB_BASE           	    ((uint32_t)0x40000000UL)
#define SYSCTRL_BASE       	    ((uint32_t)0x40060000UL)
#define DMA_BASE                ((uint32_t)0x40070000UL)
#define FLASHCTRL_REG_BASE   	  ((uint32_t)0x40080000UL)
#define DES_REG_BASE   	        ((uint32_t)0x40090000UL)

#define NVR_SIZE           	 ((uint32_t)0x200UL)
#define FLASH_NVR0_BASE			 ((uint32_t)0x00005000UL)
#define FLASH_NVR1_BASE			 ((uint32_t)0x00005200UL)
#define FLASH_NVR2_BASE			 ((uint32_t)0x00005400UL)
#define FLASH_NVR3_BASE			 ((uint32_t)0x00005600UL)
// APB
#define APB_GPIO_BASE      (APB_BASE          )               //0x00000  // gpio
#define APB_WDG_TMR_BASE   (APB_BASE + (8<<12))               //0x8000   // watchdog timer
#define APB_TMR0_BASE      (APB_BASE + (9<<12))               //0x9000   // timer0
#define APB_TMR1_BASE      (APB_BASE + (10<<12))              //0xA000   // timer1
#define APB_TMR2_BASE      (APB_BASE + (11<<12))              //0xB000   // timer2
#define APB_SCI0_BASE      (APB_BASE + (1<<16))               //0x10000  // UART0
#define APB_SCI1_BASE      (APB_BASE + (1<<16) +(1<<12))      //0x11000  // UART1
#define APB_SPI_BASE       (APB_BASE + (1<<16) +(8<<12))      //0x18000  // SPI
#define APB_I2C_BASE       (APB_BASE + (2<<16))               //0x20000  // I2C
#define APB_LVD_BASE       (APB_BASE + (2<<16) + (8<<12))     //0x28000  // LVD
#define APB_ADC_BASE       (APB_BASE + (3<<16))               //0x30000  // ADC
#define APB_PWM0_BASE      (APB_BASE + (3<<16) + (8<<12))     //0x38000  // PWM0 
#define APB_PWM1_BASE      (APB_BASE + (3<<16) + (9<<12))     //0x39000  // PWM1
#define APB_PWM2_BASE      (APB_BASE + (3<<16) + (10<<12))    //0x3A000  // PWM2
#define APB_PWM3_BASE      (APB_BASE + (3<<16) + (11<<12))    //0x3B000  // PWM3
#define APB_PINC_BASE      (APB_BASE + (4<<16))               //0x40000  // PIN ctrl

// dma
#define AHB_DMA      ((DMA_TypeDef *)  DMA_BASE) 

// system control
#define AHB_SYSCTRL  ((SYSCTRL_TypeDef *)  SYSCTRL_BASE)

#define AHB_SYSCTRL_ClkSwitch_ADDR     	SYSCTRL_BASE
#define AHB_SYSCTRL_ApbClkDiv_ADDR     	SYSCTRL_BASE + 0x04
#define AHB_SYSPLL   			SYSCTRL_BASE + 0x40

// gpio
#define APB_GPIO	 	 ((GPIO_TypeDef *) APB_GPIO_BASE)
// i2c
#define APB_I2C      ((I2C_TypeDef *) APB_I2C_BASE)
// adc
#define APB_ADC      ((ADC_TypeDef *) APB_ADC_BASE)
// uart
#define APB_UART0    ((UART_TypeDef *) APB_SCI0_BASE)
#define APB_UART1    ((UART_TypeDef *) APB_SCI1_BASE)
// timer
#define APB_WDG_TMR  ((TMR_TypeDef *) APB_WDG_TMR_BASE)
#define APB_TMR0     ((TMR_TypeDef *) APB_TMR0_BASE)
#define APB_TMR1     ((TMR_TypeDef *) APB_TMR1_BASE)
#define APB_TMR2     ((TMR_TypeDef *) APB_TMR2_BASE)
//spi
#define APB_SPI      ((SSP_TypeDef *)APB_SPI_BASE)    
// flash
#define FLASH        ((FLASH_TypeDef*) FLASHCTRL_REG_BASE)
// PWM
#define APB_PWM0     ((PWM_TypeDef*) APB_PWM0_BASE)
#define APB_PWM1     ((PWM_TypeDef*) APB_PWM1_BASE)
#define APB_PWM2     ((PWM_TypeDef*) APB_PWM2_BASE)
#define APB_PWM3     ((PWM_TypeDef*) APB_PWM3_BASE)
//lvd
#define APB_LVD      ((LVD_TypeDef*) APB_LVD_BASE)


// check RTC Domain Update regitser before Write other RTC Domain register
//#define RtcDomainUpdating    io_read(APB_UPDATE)

// apb pin ctrl
#define APB_PINC     ((PINC_TypeDef *) APB_PINC_BASE) 
// register access
#define io_write_b(a,d)  (*(__IO uint8_t*)(a)=(d))
#define io_read_b(a)     (*(__IO uint8_t*)(a))
#define io_write_h(a,d)   (*(__IO uint16_t*)(a)=(d))
#define io_read_h(a)      (*(__IO uint16_t*)(a))
#define io_write(a,d)    (*(__IO uint32_t*)(a)=(d))
#define io_read(a)       (*(__IO uint32_t*)(a))
#define io_write_dw(a,d)  (*(__IO uint64_t*)(a)=(d))
#define io_read_dw(a)     (*(__IO uint64_t*)(a))

// Bits Width change to Mask Bits
#define BW2M(a)          (1 << a) -1


// Clock Freq Define
#define OSC_CLK_FREQ      24000000UL   // 16MHz
#define OSC_SLOW_CLK_FREQ	32000
//#define OSC_CLK_FREQ      14700000UL
#define UART_SETBAUD  (115200)
#define SLOW_CLK_FREQ  32768UL   //  32768Hz, Slow Clk

extern uint32_t SystemCoreClock;
void SystemCoreClockUpdate (void);
void SystemInit (void);

#ifdef __cplusplus
  }
#endif

#endif



