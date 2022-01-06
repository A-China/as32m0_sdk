
#include "as32m0_pinctrl.h"

void PINCTRL_SetPadCtrl(uint32_t addr, uint8_t data)
{ 
	((PINC_RegDef *)(addr))->PadCtrl = data;
}

void PINCTRL_SetFuncMux(uint32_t addr, uint8_t data)
{
	((PINC_RegDef *)(addr))->FuncSel = data;
}

uint32_t PINCTRL_GetPadConfig(uint32_t addr)
{
	return io_read(addr);
}

void PinCtrl_GIOSet(GPIO_PAD_CTRL GPIOx, uint32_t GPIO_Pin, PIN_INFO_PTR pin_info_ptr)
{
	uint32_t pin_offset = 0;
	for(pin_offset=0; pin_offset < 8; pin_offset++){

		if((GPIO_Pin >> pin_offset) & 0x1){
			((PINC_RegDef *)(APB_PINC_BASE + GPIOx*0x20 + pin_offset*4))->PadCtrl = pin_info_ptr->pin_stat | pin_info_ptr->pin_sonof | pin_info_ptr->pin_od | pin_info_ptr->pin_ds;
			((PINC_RegDef *)(APB_PINC_BASE + GPIOx*0x20 + pin_offset*4))->FuncSel = pin_info_ptr->pin_func;
//			vprint_var("PIN : %d \n",pin_offset);
//			vprint_var("Addr : %#x \n",APB_PINC_BASE + GPIOx*0x20 + pin_offset*4);

		}

	}
}

