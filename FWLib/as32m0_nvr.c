#include "as32m0_nvr.h"

void nvr_erase(uint8_t sector){
	uint32_t *erase_addr;
	
	if(sector < 4){
		erase_addr = ((uint32_t*) FLASH_NVR0_BASE + sector*512);
		FLASH->Write_Mode = 0x2;
		*erase_addr = 0x1;
	}
}