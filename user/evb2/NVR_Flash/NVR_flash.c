#include "printf_config.h"
#include "as32m0_nvr.h"
#include "as32m0_misc.h"
extern void _sys_exit(void);

int main(void)
{
	uint8_t buf8[16]={0x88, 0x99, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 
										0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
	uint32_t buf32[4] = {0x78561234, 0xA1525411, 0xE3A15118, 0x0F581968};
	
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);
	printf("===>  NVR Flash ================\n");
	printf("===>  	\n");
	printf("===>  	\n");
	printf("===>  	\n");
	printf("Secctor Erase\n");
	nvr_erase(NVR1_sector);
	nvr_erase(NVR2_sector);
	nvr_erase(NVR3_sector);
	printf("Secctor prog\n");
	nvr_prog_sector_word(NVR1_sector, buf32, 4);
	nvr_prog_sector_byte(NVR2_sector, buf8, 16);
	nvr_prog_word(NVR1_sector, 0x010, 0x12548752);
	nvr_prog_word(NVR2_sector, 0x020, 0xA4548752);
	nvr_prog_word(NVR3_sector, 0x120, 0xD892BB50);
	printf("Done\n");
	while(1);
} 

