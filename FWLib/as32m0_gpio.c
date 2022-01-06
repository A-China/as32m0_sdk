#include "as32m0_gpio.h"

/**
  * @brief  Initializes the GPIOx peripheral according to the specified
  *         parameters in the GPIO_InitStruct.
  * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that
  *         contains the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void GPIO_Init(GPIO_PAD GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
	switch(GPIO_InitStruct->GPIO_Detect){
	case GPIO_HIGH :
		APB_GPIO->GPIO_IT.SET =  (GPIO_InitStruct->GPIO_Pin << GPIOx);	// interrupt type, 0-edge/1-level
		APB_GPIO->GPIO_PE.SET =  (GPIO_InitStruct->GPIO_Pin << GPIOx);	// positive sense
		APB_GPIO->GPIO_NE.CLR =  (GPIO_InitStruct->GPIO_Pin << GPIOx);	// negative sense
		break;

	case GPIO_LOW :
		APB_GPIO->GPIO_IT.SET =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		APB_GPIO->GPIO_PE.CLR =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		APB_GPIO->GPIO_NE.SET =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		break;

	case GPIO_POSEDGE :
		APB_GPIO->GPIO_IT.CLR =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		APB_GPIO->GPIO_PE.SET =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		APB_GPIO->GPIO_NE.CLR =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		break;

	case GPIO_NEGEDGE :
		APB_GPIO->GPIO_IT.CLR =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		APB_GPIO->GPIO_PE.CLR =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		APB_GPIO->GPIO_NE.SET =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		break;

	case GPIO_EDGE :
		APB_GPIO->GPIO_IT.CLR =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		APB_GPIO->GPIO_PE.SET =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		APB_GPIO->GPIO_NE.SET =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		break;
	case GPIO_NONE :
		APB_GPIO->GPIO_IT.CLR =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		APB_GPIO->GPIO_PE.CLR =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		APB_GPIO->GPIO_NE.CLR =  (GPIO_InitStruct->GPIO_Pin << GPIOx);
		break;	
	}

	switch(GPIO_InitStruct->GPIO_Mode){
	case GPIO_Mode_In :
		APB_GPIO->GPIO_OE.CLR  = (GPIO_InitStruct->GPIO_Pin<<GPIOx);
		break;
	case GPIO_Mode_Out :
		APB_GPIO->GPIO_OE.SET  = (GPIO_InitStruct->GPIO_Pin<<GPIOx);
		break;
	}
}

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
  * @param  GPIO_Pin:  specifies the port bit to read.
  *   This parameter can be GPIO_Pin_x where x can be (0..7).
  * @retval The input port pin value.
  */
uint8_t GPIO_ReadInputDataBit(GPIO_PAD GPIOx, uint32_t GPIO_Pin)
{
  	uint8_t bitstatus = 0x00;

	if ((APB_GPIO->GPIO_DI & (GPIO_Pin << GPIOx)) != (uint32_t)Bit_CLR){
		bitstatus = (uint8_t)Bit_SET;
	}
	else{
		bitstatus = (uint8_t)Bit_CLR;
	}
	return bitstatus;
}
/**
  * @brief  Reads the specified GPIO input data port.
  * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
  * @retval GPIO input data port value.
  */
uint8_t GPIO_ReadInputData(GPIO_PAD GPIOx)
{
	return ((uint8_t)APB_GPIO->GPIO_DI >> GPIOx);
}
/**
  * @brief  Sets the selected data port bits.
  * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  *   This parameter can be any combination of GPIO_Pin_x where x can be (0..7).
  * @retval None
  */
void GPIO_SetBits(GPIO_PAD GPIOx, uint32_t GPIO_Pin)
{
	APB_GPIO->GPIO_DO.SET = GPIO_Pin << GPIOx;
}
/**
  * @brief  Clears the selected data port bits.
  * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  *   This parameter can be any combination of GPIO_Pin_x where x can be (0..7).
  * @retval None
  */
void GPIO_ClrtBits(GPIO_PAD GPIOx, uint32_t GPIO_Pin)
{
	APB_GPIO->GPIO_DO.CLR = GPIO_Pin << GPIOx;
}

uint32_t GPIO_GetIntStat(void)
{
	return APB_GPIO->GPIO_IS.WRITE;
}
void GPIO_ClrIntStat(uint32_t val)
{
	APB_GPIO->GPIO_IS.CLR = val;
}


void GPIO_Set_Debouncing_Lmt(uint32_t lmt)
{
	APB_GPIO->GPIO_DEBOUNC_LMT.WRITE = lmt;
}
void GPIO_Debouncing_Bit_Enable(GPIO_PAD GPIOx, uint32_t GPIO_Pin)
{
	APB_GPIO->GPIO_DEBOUNC_BEN.SET = GPIO_Pin << GPIOx;
}
void GPIO_Debouncing_Bit_Disable(GPIO_PAD GPIOx, uint32_t GPIO_Pin)
{
	APB_GPIO->GPIO_DEBOUNC_BEN.CLR = GPIO_Pin << GPIOx;
}
void GPIO_Debouncing_Enable(void)
{
	APB_GPIO->GPIO_DEBOUNC_EN.WRITE = 0x1;
}
void GPIO_Debouncing_Disable(void)
{
	APB_GPIO->GPIO_DEBOUNC_EN.WRITE = 0x0;
}
void GPIO_CTN_Init(CTN_InitTypeDef_Ptr CTN_Init_Ptr)
{
	if(CTN_Init_Ptr->ctn0.GPIOn == GPIOA && CTN_Init_Ptr->ctn0.GPIO_Pin == GPIO_Pin_0){
		printf("Config GPA0 for ctn0 \n");
		APB_GPIO->GPIO_OE.CLR  = GPIO_Pin_0;  //Set input

		switch(CTN_Init_Ptr->ctn0.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0x1;  //Set Level Detect
			if(CTN_Init_Ptr->ctn0.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0x1;

			}else{
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0x1;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn0.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0x1;
			}else{
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0x1;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN0.WRITE = 0x1;
            APB_GPIO->GPIO_PE_CTN0.WRITE = 0x1;
			break;
		}
	}else if(CTN_Init_Ptr->ctn0.GPIOn == GPIOB && CTN_Init_Ptr->ctn0.GPIO_Pin == GPIO_Pin_2){
		printf("Config GPB2 for ctn0 \n");
		APB_GPIO->GPIO_OE.CLR  = (GPIO_Pin_2<<8);  //Set input

		switch(CTN_Init_Ptr->ctn0.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0x2;  //Set Level Detect
			if(CTN_Init_Ptr->ctn0.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0x2;
			}else{
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0x2;

			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn0.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0x2;
			}else{
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0x2;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN0.WRITE = 0x2;
            APB_GPIO->GPIO_PE_CTN0.WRITE = 0x2;
			break;
		}
	}else if(CTN_Init_Ptr->ctn0.GPIOn == GPIOB && CTN_Init_Ptr->ctn0.GPIO_Pin == GPIO_Pin_5){
		printf("Config GPB5 for ctn0 \n");
		APB_GPIO->GPIO_OE.CLR  = (GPIO_Pin_5<<8);  //Set input

		switch(CTN_Init_Ptr->ctn0.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0x4;  //Set Level Detect
			if(CTN_Init_Ptr->ctn0.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0x4;
			}else{
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0x4;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn0.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0x4;
			}else{
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0x4;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN0.WRITE = 0x4;
            APB_GPIO->GPIO_PE_CTN0.WRITE = 0x4;
			break;
		}
	}else if(CTN_Init_Ptr->ctn0.GPIOn == GPIOD && CTN_Init_Ptr->ctn0.GPIO_Pin == GPIO_Pin_1){
		printf("Config GPD1 for ctn0 \n");
		APB_GPIO->GPIO_OE.CLR  = (GPIO_Pin_1<<24);    //Set input

		switch(CTN_Init_Ptr->ctn0.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0x8;  //Set Level Detect
			if(CTN_Init_Ptr->ctn0.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0x8;
			}else{
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0x8;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn0.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0x8;
			}else{
				APB_GPIO->GPIO_PE_CTN0.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN0.WRITE = 0x8;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN0.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN0.WRITE = 0x8;
            APB_GPIO->GPIO_PE_CTN0.WRITE = 0x8;
			break;
		}
	}


	if(CTN_Init_Ptr->ctn1.GPIOn == GPIOA && CTN_Init_Ptr->ctn1.GPIO_Pin == GPIO_Pin_1){
		printf("Config GPA1 for ctn1 \n");
		APB_GPIO->GPIO_OE.CLR  = GPIO_Pin_1;  //Set input

		switch(CTN_Init_Ptr->ctn1.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0x1;  //Set Level Detect
			if(CTN_Init_Ptr->ctn1.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0x1;
			}else{
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0x1;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn1.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0x1;
			}else{
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0x1;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN1.WRITE = 0x1;
            APB_GPIO->GPIO_PE_CTN1.WRITE = 0x1;
			break;
		}
	}else if(CTN_Init_Ptr->ctn1.GPIOn == GPIOB && CTN_Init_Ptr->ctn1.GPIO_Pin == GPIO_Pin_3){
		printf("Config GPB3 for ctn1 \n");
		APB_GPIO->GPIO_OE.CLR  = (GPIO_Pin_3<<8);  //Set input

		switch(CTN_Init_Ptr->ctn1.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0x2;  //Set Level Detect
			if(CTN_Init_Ptr->ctn1.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0x2;
			}else{
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0x2;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn1.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0x2;
			}else{
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0x2;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN1.WRITE = 0x2;
            APB_GPIO->GPIO_PE_CTN1.WRITE = 0x2;
			break;
		}
	}else if(CTN_Init_Ptr->ctn1.GPIOn == GPIOB && CTN_Init_Ptr->ctn1.GPIO_Pin == GPIO_Pin_6){
		printf("Config GPB6 for ctn1 \n");
		APB_GPIO->GPIO_OE.CLR  = (GPIO_Pin_6<<8);  //Set input

		switch(CTN_Init_Ptr->ctn1.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0x4;  //Set Level Detect
			if(CTN_Init_Ptr->ctn1.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0x4;
			}else{
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0x4;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn1.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0x4;
			}else{
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0x4;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN1.WRITE = 0x4;
            APB_GPIO->GPIO_PE_CTN1.WRITE = 0x4;
			break;
		}
	}else if(CTN_Init_Ptr->ctn1.GPIOn == GPIOD && CTN_Init_Ptr->ctn1.GPIO_Pin == GPIO_Pin_2){
		printf("Config GPD2 for ctn1 \n");
		APB_GPIO->GPIO_OE.CLR  = (GPIO_Pin_2<<24);    //Set input

		switch(CTN_Init_Ptr->ctn1.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0x8;  //Set Level Detect
			if(CTN_Init_Ptr->ctn1.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0x8;
			}else{
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0x8;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn1.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0x8;
			}else{
				APB_GPIO->GPIO_PE_CTN1.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN1.WRITE = 0x8;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN1.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN1.WRITE = 0x8;
            APB_GPIO->GPIO_PE_CTN1.WRITE = 0x8;
			break;
		}
	}

	if(CTN_Init_Ptr->ctn2.GPIOn == GPIOA && CTN_Init_Ptr->ctn2.GPIO_Pin == GPIO_Pin_2){
		printf("Config GPA2 for ctn2 \n");
		APB_GPIO->GPIO_OE.CLR  = GPIO_Pin_2;  //Set input

		switch(CTN_Init_Ptr->ctn2.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0x1;  //Set Level Detect
			if(CTN_Init_Ptr->ctn2.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0x1;
			}else{
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0x1;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn2.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0x1;
			}else{
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0x1;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN2.WRITE = 0x1;
            APB_GPIO->GPIO_PE_CTN2.WRITE = 0x1;
			break;
		}
	}else if(CTN_Init_Ptr->ctn2.GPIOn == GPIOB && CTN_Init_Ptr->ctn2.GPIO_Pin == GPIO_Pin_4){
		printf("Config GPB4 for ctn2 \n");
		APB_GPIO->GPIO_OE.CLR  = (GPIO_Pin_4<<8);  //Set input

		switch(CTN_Init_Ptr->ctn2.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0x2;  //Set Level Detect
			if(CTN_Init_Ptr->ctn2.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0x2;
			}else{
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0x2;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn2.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0x2;
			}else{
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0x2;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN2.WRITE = 0x2;
            APB_GPIO->GPIO_PE_CTN2.WRITE = 0x2;
			break;
		}
	}else if(CTN_Init_Ptr->ctn2.GPIOn == GPIOB && CTN_Init_Ptr->ctn2.GPIO_Pin == GPIO_Pin_7){
		printf("Config GPB7 for ctn2 \n");
		APB_GPIO->GPIO_OE.CLR  = (GPIO_Pin_7<<8);  //Set input

		switch(CTN_Init_Ptr->ctn2.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0x4;  //Set Level Detect
			if(CTN_Init_Ptr->ctn2.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0x4;
			}else{
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0x4;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn2.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0x4;
			}else{
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0x4;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN2.WRITE = 0x4;
            APB_GPIO->GPIO_PE_CTN2.WRITE = 0x4;
			break;
		}
	}else if(CTN_Init_Ptr->ctn2.GPIOn == GPIOD && CTN_Init_Ptr->ctn2.GPIO_Pin == GPIO_Pin_3){
		printf("Config GPD3 for ctn2 \n");
		APB_GPIO->GPIO_OE.CLR  = (GPIO_Pin_3<<24);    //Set input

		switch(CTN_Init_Ptr->ctn2.gpio_dec){
		case GPIO_HIGH : case GPIO_LOW :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0x8;  //Set Level Detect
			if(CTN_Init_Ptr->ctn2.gpio_dec == GPIO_HIGH){
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0x8;
			}else{
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0x8;
			}
			break;
		case GPIO_POSEDGE : case GPIO_NEGEDGE :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0;  //Set Edge Detect
			if(CTN_Init_Ptr->ctn2.gpio_dec == GPIO_POSEDGE){
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0x8;
			}else{
				APB_GPIO->GPIO_PE_CTN2.WRITE = 0;
				APB_GPIO->GPIO_NE_CTN2.WRITE = 0x8;
			}
			break;
		case GPIO_EDGE :
			APB_GPIO->GPIO_LV_CTN2.WRITE = 0;  //Set Edge Detect
            APB_GPIO->GPIO_NE_CTN2.WRITE = 0x8;
            APB_GPIO->GPIO_PE_CTN2.WRITE = 0x8;
			break;
		}
	}
}

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx_Pin_y:  specifies the port bit to read.
  *   This parameter can be GPIOx_Pin_y where x can be (A..D) and y caa be (0..7).
  * @retval The input port pin value.
  */
uint8_t GPIO_ReadInputBit(uint32_t GPIOx_Pin_y)
{
	uint8_t temp;
	if ((APB_GPIO->GPIO_DI & GPIOx_Pin_y) == GPIOx_Pin_y){
		temp = 0x01;
	}
	else{
		temp = 0x00;
	}
	return temp;
}
