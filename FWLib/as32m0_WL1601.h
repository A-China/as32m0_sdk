#ifndef AS32M0_WL1601
#define AS32M0_WL1601
#include "as32m0.h"
#include "as32m0_gpio.h"
#include "as32m0_pinctrl.h"
#include "as32m0_ssp.h"
#include "as32m0_misc.h"
#define WL1601_CSn 		GPIOB_Pin_0
#define WL1601_MOSI 	GPIOB_Pin_3
#define WL1601_MISO		GPIOB_Pin_2
#define WL1601_CLK		GPIOB_Pin_1
#define WL1601_CE  		GPIOB_Pin_5
#define	WL1601_PKT_FLAG 1<<6
#define	WL1601_FIFO_FLAG 1<<5

#define WL1601_CSn_SET(n)	 (n? ( APB_GPIO->GPIO_DO.SET = WL1601_CSn) :(APB_GPIO->GPIO_DO.CLR = WL1601_CSn))
#define WL1601_CSn_OUT_OE(n)	 (n? ( APB_GPIO->GPIO_OE.SET = WL1601_CSn) :(APB_GPIO->GPIO_OE.CLR = WL1601_CSn))
#define WL1601_CE_SET(n)	 (n? ( APB_GPIO->GPIO_DO.SET = WL1601_CE) :(APB_GPIO->GPIO_DO.CLR = WL1601_CE))
#define WL1601_CE_OUT_OE(n)	 (n? ( APB_GPIO->GPIO_OE.SET = WL1601_CE) :(APB_GPIO->GPIO_OE.CLR = WL1601_CE))

#define REG_BIT_SET    1
#define REG_BIT_CLEAN  0

typedef enum{
	WL1600_Get_Auto_Flag = 1,
	WL1600_Get_Pkg_Flag,
	WL1600_Get_Fifo_Flag,
	WL1600_Fec_Error_Flag,
	WL1600_Crc_Error_Flag,
	WL1600_Syncword_Recv,
	WL1600_Ch_No3_No0,
	WL1600_Fifo_Wr_Ptr,
	WL1600_Fifo_Rr_Ptr,
} WL1600_GetFlag_Type;

typedef enum{
	WL1600_Rx_Mode = 1,
	WL1600_Tx_Mode,
	WL1600_Idle_Mode,
	WL1600_Sleep_Mode,
	WL1600_Auto_Ack_Mask_Disable,
	WL1600_Auto_Ack_Mask_Enable,
	WL1600_Clean_Auto_Flag,
	WL1600_Auto_Ack_Disable,
	WL1600_Auto_Ack_Enable,
	WL1600_Pkg_Flag_Mask_Disable,
	WL1600_Pkg_Flag_Mask_Enable,
	WL1600_Clean_Pkg_Flag,
	WL1600_Set_FEC_Type_No,
	WL1600_Set_FEC_Type_FEC13,
	WL1600_Set_FEC_Type_FEC23,
	WL1600_CRC_ON_Disable,
	WL1600_CRC_ON_Enable,
	WL1600_Scramble_ON_Disable,
	WL1600_Scramble_ON_Enable,
	WL1600_Pack_Length_Disable,
	WL1600_Pack_Length_Enable,
	WL1600_Pkt_Hint_Pority_High,
	WL1600_Pkt_Hint_Pority_Low,
	WL1600_Clean_Tx_Fifo,
	WL1600_Clean_Rx_Fifo,
} WL1600_SetMask_Type;

void wl1601_init_spi(uint8_t speed);
void wl1601_init_RF(void);
void wl1601_init_TX(uint8_t power_gain, uint8_t channel);
void wl1601_init_RX(uint8_t mode, uint8_t channel); // mode1: short distance mode, mode0: normal working mode
void wl1601_state_to_direct(void);
uint8_t wl1601_check_PKT_FIFO_flag(void);
uint8_t wl1601_check_CRC_error(void);
uint8_t wl1601_TX_send_FIFO_small_packet(uint8_t *data, uint8_t len);
uint8_t wl1601_RX_recv_FIFO_small_packet(uint8_t *data);
uint8_t wl1601_RX_recv_FIFO_large_packet(uint8_t *data);
uint8_t wl1601_rd_FIFO_full_thld(void);
void wl1601_chip_enable(void);
void wl1601_chip_disable(void);
void wl1601_wake_up(void);
void wl1601_set250K(uint8_t tx);
uint8_t WL1600_GetFlagStatus(WL1600_GetFlag_Type flag_type);
void WL1600_SetFlag_Mask(WL1600_SetMask_Type flag_type);
uint8_t wl1601_wr_regs(uint8_t addr, uint8_t data);
uint8_t wl1601_rd_regs(uint8_t addr, uint8_t *data);
uint8_t wl1601_wr_reg_bit(uint8_t addr, uint8_t bit, uint8_t value);
uint8_t wl1601_rd_reg_bit(uint8_t addr, uint8_t bit);
uint8_t wl1601_rd_reg_nbit(uint8_t addr, uint8_t begin_bit, uint8_t end_bit);
void wl1601_wr_FIFO(uint8_t len, uint8_t *data, uint8_t data_size);
void wl1601_rd_FIFO(uint8_t len, uint8_t *data, uint8_t enable_len);
#endif
