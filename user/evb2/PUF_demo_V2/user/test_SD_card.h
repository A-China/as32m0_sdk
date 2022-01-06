#ifndef __TEST_SD_CARD_H__
#define __TEST_SD_CARD_H__
#include "as32m0.h"
#include "as32m0_gpio.h"
#include "as32m0_ssp.h"
#include "as32m0_uart.h"
#include "as32m0_dma.h"
#define SD_CS							GPIO_Pin_0 << (1*8)
void delay(uint32_t count);
void init_SD_card_PowerUp(void);
void init_SD_card_spi_dma(void);
void SD_printf_datamap(uint32_t len, uint32_t sd_addr, uint8_t *data);
void SD_spi_read_CSD(void);
void SD_spi_set_block_len(uint32_t len);
uint8_t SD_spi_read_single_blk(uint32_t len, uint32_t sd_addr, uint8_t *data);
uint8_t SD_spi_write_single_blk(uint32_t len, uint32_t sd_addr, uint8_t *data);
uint64_t SD_spi_command(uint8_t cmd, uint32_t arg, uint8_t resp_type);
uint32_t SD_spi_read_command(uint8_t cmd, uint32_t arg, uint8_t resp_type);
void SD_spi_send_byte(uint8_t data);
uint8_t SD_spi_getdata(void);
uint8_t crc7_mmc(uint8_t *data, uint8_t byte_length);
uint16_t crc16_mmc(uint8_t *data, uint16_t byte_length);
#endif

