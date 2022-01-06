#ifndef __AS32M0_DES_H__
#define __AS32M0_DES_H__
#include "as32m0.h"

#define P_LINE	printf("Func [%s], Line [%d]\n", __FUNCTION__, __LINE__);

#define     DES_PHY_ADDR               DES_REG_BASE

#define     DES_ENABLE                 (0x000)
#define     DES_MODE                   (0x010)
#define     DES_STATUS                 (0x020)
#define     DES_DINL                   (0x030)
#define     DES_DINH                   (0x040)
#define     DES_DOUTL                  (0x050)
#define     DES_DOUTH                  (0x060)
#define     DES_KEY0                   (0x070)
#define     DES_KEY1                   (0x080)
#define     DES_KEY2                   (0x090)
#define     DES_KEY3                   (0x0A0)
#define     DES_KEY4                   (0x0B0)
#define     DES_KEY5                   (0x0C0)
#define     DES_PUF_ENABLE             (0x140)


/* bits in bit DES_REG_ENABLE function */
#define DES_BIT_ENABLE                 (0)

#define DES_EN                         (1 << DES_BIT_ENABLE)

/* bits in bit DES_MODE function */
#define DES_MODE_BIT_ENC               (0)       
#define DES_MODE_BIT_KEY_SEL           (4) 
#define DES_MODE_BIT_ALGORITHM_SEL     (6) // [7:6]
#define DES_MODE_BIT_QUARITY_MODE      (8) 


#define DES_MODE_ENC                   (1   << DES_MODE_BIT_ENC         )       
#define DES_MODE_KEY_SEL               (1   << DES_MODE_BIT_KEY_SEL     ) 
#define DES_MODE_ALGORITHM_SEL         (0x3 << DES_MODE_BIT_ALGORITHM_SE) // [7:6]
#define DES_MODE_QUARITY_MODE          (1   << DES_MODE_BIT_QUARITY_MODE) 

/* bits in bit DES_STATUS function */
#define DES_STATUS_BIT_OUTPUT_READY    (0)   
#define DES_STATUS_BIT_INPUT_READY     (4)  
#define DES_STATUS_BIT_DES_BUSY        (8)    
#define DES_STATUS_BIT_QUALITY_STATE   (12)  
#define DES_STATUS_BIT_QUALITY_DONE    (13)   
#define DES_STATUS_BIT_MCU_PUF         (16)  // [20:16]     
#define DES_STATUS_BIT_PUF_STRUP       (22)  // [31:22]  

#define DES_STATUS_OUTPUT_READY        (1 << DES_STATUS_BIT_OUTPUT_READY )   
#define DES_STATUS_INPUT_READY         (1 << DES_STATUS_BIT_INPUT_READY  )  
#define DES_STATUS_DES_BUSY            (1 << DES_STATUS_BIT_DES_BUSY     )    
#define DES_STATUS_QUALITY_STATE       (1 << DES_STATUS_BIT_QUALITY_STATE)  
#define DES_STATUS_QUALITY_DONE        (1 << DES_STATUS_BIT_QUALITY_DONE )   
#define DES_STATUS_MCU_PUF             (0x1F  << DES_STATUS_BIT_MCU_PUF  )  // [20:16]     
#define DES_STATUS_PUF_STRUP           (0x3FF << DES_STATUS_BIT_PUF_STRUP)  // [31:22]  


/* bits in bit DES_PUF_ENABLE function */
#define DES_PUF_BIT_ENABLE             (0)

#define DES_PUF_EN                     (1 << DES_PUF_BIT_ENABLE)

typedef enum algorithm_sel{
	DES   = 0,
	TDES  = 1,
	PUF   = 2
}ALGORITHM_SEL;

typedef enum key_sel{
	FROM_REG = 0,
	FROM_PUF = 1
}KEY_SEL;

typedef enum enc_mode {
	DECRYPT_MODE = 0,
	ENCRYPT_MODE = 1
}ENC_MODE;

typedef struct keys_info {
	uint64_t key_0;
	uint64_t key_1;
	uint64_t key_2;
}KEYS_INFO;

typedef struct des_info{
	ALGORITHM_SEL  algorithm_sel;
	KEY_SEL        key_sel;
	ENC_MODE       enc_mode;
	KEYS_INFO      keys_info;
}DES_INFO, *DES_INFO_PTR;

int32_t apDes_Write_Data(uint64_t *data);
int32_t apDes_Read_Data(uint64_t *data);
int32_t apDes_Encrypt_Decrypt_Data(uint64_t *Serc_data, uint64_t *Dst_data, uint32_t count);

void apDes_Config(DES_INFO_PTR des_info_ptr);
void apDes_Enable(void);
void apDes_Disable(void);
void Read_puf_key(uint32_t* ser_key);
int32_t apDes_Debug_Test(void);

#endif
