#ifndef __AS32M0_GPIO_H__
#define __AS32M0_GPIO_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "as32m0.h"

#define GPIO_Pin_0                 ((uint32_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint32_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint32_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint32_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint32_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint32_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint32_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint32_t)0x0080)  /*!< Pin 7 selected */

#define GPIOA_Pin_0                 ((uint32_t)0x00000001)  /*!< PA0 selected */
#define GPIOA_Pin_1                 ((uint32_t)0x00000002)  /*!< PA1 selected */
#define GPIOA_Pin_2                 ((uint32_t)0x00000004)  /*!< PA2 selected */
#define GPIOA_Pin_3                 ((uint32_t)0x00000008)  /*!< PA3 selected */
#define GPIOA_Pin_4                 ((uint32_t)0x00000010)  /*!< PA4 selected */
#define GPIOA_Pin_5                 ((uint32_t)0x00000020)  /*!< PA5 selected */
#define GPIOA_Pin_6                 ((uint32_t)0x00000040)  /*!< PA6 selected */
#define GPIOA_Pin_7                 ((uint32_t)0x00000080)  /*!< PA7 selected */

#define GPIOB_Pin_0                 ((uint32_t)0x00000100)  /*!< PB0 selected */
#define GPIOB_Pin_1                 ((uint32_t)0x00000200)  /*!< PB1 selected */
#define GPIOB_Pin_2                 ((uint32_t)0x00000400)  /*!< PB2 selected */
#define GPIOB_Pin_3                 ((uint32_t)0x00000800)  /*!< PB3 selected */
#define GPIOB_Pin_4                 ((uint32_t)0x00001000)  /*!< PB4 selected */
#define GPIOB_Pin_5                 ((uint32_t)0x00002000)  /*!< PB5 selected */
#define GPIOB_Pin_6                 ((uint32_t)0x00004000)  /*!< PB6 selected */
#define GPIOB_Pin_7                 ((uint32_t)0x00008000)  /*!< PB7 selected */

#define GPIOC_Pin_0                 ((uint32_t)0x00010000)  /*!< PC0 selected */
#define GPIOC_Pin_1                 ((uint32_t)0x00020000)  /*!< PC1 selected */
#define GPIOC_Pin_2                 ((uint32_t)0x00040000)  /*!< PC2 selected */
#define GPIOC_Pin_3                 ((uint32_t)0x00080000)  /*!< PC3 selected */
#define GPIOC_Pin_4                 ((uint32_t)0x00100000)  /*!< PC4 selected */
#define GPIOC_Pin_5                 ((uint32_t)0x00200000)  /*!< PC5 selected */
#define GPIOC_Pin_6                 ((uint32_t)0x00400000)  /*!< PC6 selected */
#define GPIOC_Pin_7                 ((uint32_t)0x00800000)  /*!< PC7 selected */

#define GPIOD_Pin_0                 ((uint32_t)0x01000000)  /*!< PD0 selected */
#define GPIOD_Pin_1                 ((uint32_t)0x02000000)  /*!< PD1 selected */
#define GPIOD_Pin_2                 ((uint32_t)0x04000000)  /*!< PD2 selected */
#define GPIOD_Pin_3                 ((uint32_t)0x08000000)  /*!< PD3 selected */


typedef enum {
  GPIOA = 0,
  GPIOB = 8,
  GPIOC = 16,
  GPIOD = 24,
}GPIO_PAD;

typedef enum {
  GPIO_Mode_In  = 0x0,
  GPIO_Mode_Out = 0x1,
}GPIOMode_TypeDef;

typedef enum {
	GPIO_HIGH    = 0,
	GPIO_LOW     = 1,
	GPIO_POSEDGE = 2,
	GPIO_NEGEDGE = 3,
	GPIO_EDGE = 4,
	GPIO_NONE = 5
}GPIO_DETECT;

typedef enum {
  Bit_CLR = 0,
  Bit_SET = 1
}BitAction;

typedef struct{
	uint32_t GPIO_Pin;
	GPIO_PAD   GPIOn;
	GPIO_DETECT gpio_dec;
}GPIO_CTNx;

typedef struct {
  uint32_t GPIO_Pin;
  GPIOMode_TypeDef GPIO_Mode;    
  GPIO_DETECT      GPIO_Detect;
}GPIO_InitTypeDef;

typedef struct{
	GPIO_CTNx ctn0;
	GPIO_CTNx ctn1;
	GPIO_CTNx ctn2;
}CTN_InitTypeDef,*CTN_InitTypeDef_Ptr;


void GPIO_Init(GPIO_PAD GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
uint8_t GPIO_ReadInputDataBit(GPIO_PAD GPIOx, uint32_t GPIO_Pin);
uint8_t GPIO_ReadInputData(GPIO_PAD GPIOx);
void GPIO_SetBits(GPIO_PAD GPIOx, uint32_t GPIO_Pin);
void GPIO_ClrtBits(GPIO_PAD GPIOx, uint32_t GPIO_Pin);
uint32_t GPIO_GetIntStat(void);
void GPIO_ClrIntStat(uint32_t val);
void GPIO_Set_Debouncing_Lmt(uint32_t lmt);
void GPIO_Debouncing_Bit_Enable(GPIO_PAD GPIOx, uint32_t GPIO_Pin);
void GPIO_Debouncing_Bit_Disable(GPIO_PAD GPIOx, uint32_t GPIO_Pin);
void GPIO_Debouncing_Enable(void);
void GPIO_Debouncing_Disable(void);
void GPIO_CTN_Init(CTN_InitTypeDef_Ptr CTN_Init_Ptr);
uint8_t GPIO_ReadInputBit(uint32_t GPIOx_Pin_y);

#ifdef __cplusplus
  }
#endif

#endif
