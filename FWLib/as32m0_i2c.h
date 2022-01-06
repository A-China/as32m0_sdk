
#ifndef __AS32M0_I2C_H__
#define __AS32M0_I2C_H__


#ifdef	__cplusplus
extern "C" {	/* allow C++ to use these headers */
#endif	/* __cplusplus */

#include "as32m0.h"
#include "as32m0_pinctrl.h"
#include "as32m0_misc.h"
#include "printf_config.h"
typedef enum{
	I2C_PB45 = 0,
	I2C_PC01 = 1,
	I2C_PC67 = 2,
	I2C_PD23 = 3
}I2C_LOC;

extern void I2C_MasterConfig(I2C_TypeDef *I2C,
                        uint8_t TarAddress,
                        uint8_t IcSlaveDis,
                        uint8_t Speed,
                        uint8_t MasterMode,
                        uint8_t tx_fifo,
                        uint8_t rx_fifo,
                        uint8_t fastmodeplus
			);
extern void I2C_SlaveConfig(I2C_TypeDef *I2C,
                        uint8_t SlaAddress,
                        uint8_t Speed,
                        uint8_t tx_fifo,
                        uint8_t rx_fifo,
                        uint8_t fastmodeplus
			);
extern void I2CWirteOneByte(I2C_TypeDef *I2C,uint8_t OffsetAddr,uint8_t data);
extern uint8_t I2CReadOneByte(I2C_TypeDef *I2C,uint8_t OffsetAddr);
extern void I2CWirteOneByte_Loopback(I2C_TypeDef *S_I2C,uint8_t data);
void I2C_ENABLE(I2C_TypeDef *I2C,uint32_t data);
void I2C_DISABLE(I2C_TypeDef *I2C,uint32_t data);
void I2C_TargerAdd(I2C_TypeDef *I2C,uint32_t data);
void I2C_SlaveAdd(I2C_TypeDef *I2C,uint32_t data);
void I2C_SCL_HCNT(I2C_TypeDef *I2C,uint8_t speed,uint8_t fastmodeplus);
void I2C_SCL_LCNT(I2C_TypeDef *I2C,uint8_t speed,uint8_t fastmodeplus);
void I2C_TransFifoSet(I2C_TypeDef *I2C,uint8_t data);
void I2C_ReceFifoSet(I2C_TypeDef *I2C,uint8_t data);
void I2C_MasterConfig(I2C_TypeDef *I2C,       \
                        uint8_t TarAddress,   \
                        uint8_t IcSlaveDis,   \
                        uint8_t Speed,        \
                        uint8_t MasterMode,   \
                        uint8_t tx_fifo,      \
                        uint8_t rx_fifo,      \
                        uint8_t fastmodeplus  \
			);
void I2C_SlaveConfig(I2C_TypeDef *I2C,        \
                        uint8_t SlaAddress,   \
                        uint8_t Speed,        \
                        uint8_t tx_fifo,      \
                        uint8_t rx_fifo,      \
                        uint8_t fastmodeplus  \
			);

void I2CWirteBytes(I2C_TypeDef *I2C,uint8_t *data,uint8_t len);
void I2CReadBytes(I2C_TypeDef *I2C,uint8_t OffsetAddr, uint8_t *data,uint8_t len);
void I2C_init(uint8_t taddr, uint8_t Speed, I2C_LOC i2c_loc);
void I2C_SearchAddr (I2C_TypeDef *I2C, I2C_LOC i2c_loc); 
uint32_t  I2C_CLR_RX_UNDER  (I2C_TypeDef *I2C); 
uint32_t  I2C_CLR_RX_OVER   (I2C_TypeDef *I2C); 
uint32_t  I2C_CLR_TX_OVER   (I2C_TypeDef *I2C); 
uint32_t  I2C_CLR_RD_REQ    (I2C_TypeDef *I2C); 
uint32_t  I2C_CLR_TX_ABRT   (I2C_TypeDef *I2C); 
uint32_t  I2C_CLR_RX_DONE   (I2C_TypeDef *I2C); 
uint32_t  I2C_CLR_ACTIVITY  (I2C_TypeDef *I2C); 
uint32_t  I2C_CLR_STOP_DET  (I2C_TypeDef *I2C); 
uint32_t  I2C_CLR_START_DET (I2C_TypeDef *I2C); 
uint32_t  I2C_CLR_GEN_CALL  (I2C_TypeDef *I2C); 
/*
 * Description:
 * Bit shifts and widths for I2C_ENABLE  0x6c
 */
#define bwI2C_ENAble                 0 
/*
 * IC INTR STAT	/IC INTR MASK/ IC RAW INTR STAT 0x2C/0x30/0x34
 *
 */
#define IC_INTR_RX_UNDER				 	1<<0UL
#define IC_INTR_RX_OVER				 		1<<1UL
#define IC_INTR_RX_FULL				 		1<<2UL
#define IC_INTR_TX_OVER				 		1<<3UL
#define IC_INTR_TX_EMPTY				 	1<<4UL
#define IC_INTR_RD_REQ				 		1<<5UL
#define IC_INTR_TX_ABRT				 		1<<6UL
#define IC_INTR_RX_DONE			 			1<<7UL
#define IC_INTR_ACTIVITY				 	1<<8UL
#define IC_INTR_STOP_DET				 	1<<9UL
#define IC_INTR_START_DET				 	1<<10UL
#define IC_INTR_GEN_CALL				 	1<<11UL
#define IC_INTR_RESTART_DET				1<<12UL
#define IC_INTR_MASTER_ON_HOLD		1<<13UL
#define IC_INTR_SCL_STUCK_AT_LOW	1<<14UL
/*
 * IC STATUS	0x70
 *
 */
#define IC_STATUS_ACTIVITY       1<<0UL
#define IC_STATUS_TFNF      		 1<<1UL
#define IC_STATUS_TFE      		 	 1<<2UL
#define IC_STATUS_RFNE      		 1<<3UL
#define IC_STATUS_RFF      		 	 1<<4UL



#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif	/* __cplusplus */

#endif

