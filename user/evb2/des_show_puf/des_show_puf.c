#include <string.h>
#include "printf_config.h"
#include "as32m0_des.h"
#include "as32m0_misc.h"
#include "as32m0_pinctrl.h"
#include "as32m0_timer.h"

#define DATA_SIZE 16
#define TEST_DEBUG	1

#define LED GPIOA_Pin_0

void init_timer(float second);
void _sys_exit(void);
int32_t des_check_result(void);
DES_INFO des_info;
char src_data[DATA_SIZE][8] = {"eeff0c00","36987412","qWeRrTyU","qwertyui",\
		                       "zxcvbnmm","eeff0c00","36987412","qWeRrTyU",\
							   "qwertyui","zxcvbnmm","eeff0c00","36987412",\
							   "eeff0c00","qwertyui","qWeRrTyU","zxcvbnmm"};
char decrypt_data[DATA_SIZE][8] = {0};
char encrypt_data[DATA_SIZE][8] = {0};
uint32_t data_count=0;
char temp[3] = {0};

void show_puf_key(){
	uint32_t ser_key[24], i;
	Read_puf_key(ser_key);
	printf("\n PUF Key : ");
	printf("0x");
	for(i=0;i<24;i++){
	 printf("%.8x", ser_key[23-i]);
	}
  printf("\n"); 

};
static void des_reg_test(void)
{
	int32_t ret = -1;

	printf("Des from Reg test \n");

	memset(encrypt_data, 0, sizeof(encrypt_data));
	memset(decrypt_data, 0, sizeof(decrypt_data));

	des_info.algorithm_sel   = DES;
	des_info.key_sel         = FROM_REG;
	des_info.enc_mode        = ENCRYPT_MODE;
	des_info.keys_info.key_0 = (uint64_t)*(uint64_t *)"01234568";
	des_info.keys_info.key_1 = (uint64_t)*(uint64_t *)"fedcba98";
	des_info.keys_info.key_2 = (uint64_t)*(uint64_t *)"01234568";

	apDes_Config(&des_info);

	apDes_Encrypt_Decrypt_Data((uint64_t *)src_data, (uint64_t *)encrypt_data, data_count);

	des_info.enc_mode      = DECRYPT_MODE;
	apDes_Config(&des_info);

	ret = apDes_Encrypt_Decrypt_Data((uint64_t *)encrypt_data, (uint64_t *)decrypt_data, data_count);
	if(ret < 0){
		printf("[%d] apDes_Encrypt_Decrypt_Data Failed \n",__LINE__);
	}

	ret = des_check_result();
	if(ret < 0){
		printf("[%d] des_check_result Failed \n",__LINE__);
	}
}
static void des_puf_test(void)
{
	int32_t ret = -1;

	printf("Des form PUF test \n");

	memset(encrypt_data, 0, sizeof(encrypt_data));
	memset(decrypt_data, 0, sizeof(decrypt_data));

	des_info.algorithm_sel   = DES;
	des_info.key_sel         = FROM_PUF;
	des_info.enc_mode        = ENCRYPT_MODE;

	apDes_Config(&des_info);

	apDes_Encrypt_Decrypt_Data((uint64_t *)src_data, (uint64_t *)encrypt_data, data_count);
#if  PRINT_KEY_PUF
		print_des_key();
#endif

	des_info.enc_mode      = DECRYPT_MODE;
	apDes_Config(&des_info);

	ret = apDes_Encrypt_Decrypt_Data((uint64_t *)encrypt_data, (uint64_t *)decrypt_data, data_count);
	if(ret < 0){
		printf("[%d] apDes_Encrypt_Decrypt_Data Failed \n",__LINE__);
	}
#if  PRINT_KEY_PUF
		print_des_key();
#endif

	ret = des_check_result();
	if(ret < 0){
		printf("[%d] des_check_result Failed \n",__LINE__);
	}
}
static void tdes_reg_test(void)
{
	int32_t ret = -1;
	
	printf("TDes from Reg test \n");
	memset(encrypt_data, 0, sizeof(encrypt_data));
	memset(decrypt_data, 0, sizeof(decrypt_data));

	des_info.algorithm_sel   = TDES;
	des_info.key_sel         = FROM_REG;
	des_info.enc_mode        = ENCRYPT_MODE;
	des_info.keys_info.key_0 = (uint64_t)*(uint64_t *)"01234568";
	des_info.keys_info.key_1 = (uint64_t)*(uint64_t *)"fedcba98";
	des_info.keys_info.key_2 = (uint64_t)*(uint64_t *)"01234568";

	apDes_Config(&des_info);

	apDes_Encrypt_Decrypt_Data((uint64_t *)src_data, (uint64_t *)encrypt_data, data_count);

	des_info.enc_mode      = DECRYPT_MODE;
	apDes_Config(&des_info);

	ret = apDes_Encrypt_Decrypt_Data((uint64_t *)encrypt_data, (uint64_t *)decrypt_data, data_count);
	
	if(ret < 0){
		printf("[%d] apDes_Encrypt_Decrypt_Data Failed \n",__LINE__);
	}

	ret = des_check_result();
	if(ret < 0){
		printf("[%d] des_check_result Failed \n",__LINE__);
	}
}
static void tdes_puf_test(void)
{
	int32_t ret = -1;

	printf("TDes form PUF test \n");
	memset(encrypt_data, 0, sizeof(encrypt_data));
	memset(decrypt_data, 0, sizeof(decrypt_data));

	des_info.algorithm_sel   = TDES;
	des_info.key_sel         = FROM_PUF;
	des_info.enc_mode        = ENCRYPT_MODE;

	apDes_Config(&des_info);

	apDes_Encrypt_Decrypt_Data((uint64_t *)src_data, (uint64_t *)encrypt_data, data_count);
#if  PRINT_KEY_PUF
		print_des_key();
#endif

	des_info.enc_mode      = DECRYPT_MODE;
	apDes_Config(&des_info);

	ret = apDes_Encrypt_Decrypt_Data((uint64_t *)encrypt_data, (uint64_t *)decrypt_data, data_count);
	if(ret < 0){
		printf("[%d] apDes_Encrypt_Decrypt_Data Failed \n",__LINE__);
	}
#if  PRINT_KEY_PUF
		print_des_key();
#endif

	ret = des_check_result();
	if(ret < 0){
		printf("[%d] des_check_result Failed \n",__LINE__);
	}
}
static void puf_test(void)
{
	int32_t ret = -1;

	printf("PUF Mode test \n");

	memset(encrypt_data, 0, sizeof(encrypt_data));
	memset(decrypt_data, 0, sizeof(decrypt_data));

	des_info.algorithm_sel   = PUF;
	des_info.key_sel         = FROM_PUF;
	des_info.enc_mode        = ENCRYPT_MODE;

	apDes_Config(&des_info);

	apDes_Encrypt_Decrypt_Data((uint64_t *)src_data, (uint64_t *)encrypt_data, data_count);
#if  PRINT_KEY_PUF
		print_des_key();
#endif

	des_info.enc_mode      = DECRYPT_MODE;
	apDes_Config(&des_info);

	ret = apDes_Encrypt_Decrypt_Data((uint64_t *)encrypt_data, (uint64_t *)decrypt_data, data_count);
	if(ret < 0){
		printf("[%d] apDes_Encrypt_Decrypt_Data Failed \n",__LINE__);
	}
#if  PRINT_KEY_PUF
		print_des_key();
#endif

	ret = des_check_result();
	if(ret < 0){
		printf("[%d] des_check_result Failed \n",__LINE__);
	}
}

int main()
{
	int32_t ret = -1;
	uint32_t i;
	
	// LED @ GPIOA Pin0
	APB_PINC->PA0.FuncSel = GIO_FUNC0;
	APB_GPIO->GPIO_OE.SET = LED;
	APB_GPIO->GPIO_DO.CLR = LED;
	
//	PIN_INFO LEDPIN;
//	LEDPIN.pin_func = GIO_FUNC0;
//	LEDPIN.pin_ds = GIO_DS_2_4;
//	LEDPIN.pin_stat = GIO_PU;
//	LEDPIN.pin_od = OD_OFF;
//	LEDPIN.pin_sonof = SONOF_OFF;
//	PinCtrl_GIOSet(PIN_CTL_GPIOA, LED, &LEDPIN);
//	APB_GPIO->GPIO_OE.SET = LED;
//	APB_GPIO->GPIO_DO.CLR = LED;
	
	APB_GPIO->GPIO_DO.SET = LED;
	config_uart0(OSC_CLK_FREQ, UART_SETBAUD, UART0_PD23);	
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST DES Begin \n");
	printf("==> \n");
	printf("==> \n");
	// wait
	while(apUART_Check_BUSY(APB_UART0) == 1);
	
	
	//Get valid data count
	for(i=0; i<DATA_SIZE; ){
		if(src_data[i][0] == 0){
			data_count = i;
			break;
		}
		i++;
		if(i == DATA_SIZE){
			data_count = i;
			break;
		}
	}
	des_reg_test();
	des_puf_test();
	tdes_reg_test();
	tdes_puf_test();
	puf_test();
	show_puf_key();
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST DES End \n");
	printf("==> \n");
	printf("==> \n");
	APB_GPIO->GPIO_DO.CLR = LED;
	printf("#"); // invoke TB $finish;
//	_sys_exit();
//	return 0x12;
	init_timer(0.5);
	while(1);
	
}

int32_t des_check_result(void)
{
	uint32_t i = 0, j=0;
	uint64_t temp64;
	uint8_t temp8[9];
	temp8[8] = '\0';
	for(i=0; i<data_count; i++){
#if !TEST_DEBUG
		if(memcmp(&decrypt_data[i],&src_data[i],sizeof(src_data[i])) != 0){
#endif
		temp64 = *(uint64_t *)src_data[i];
		printf("[%d] src_data[%d] : 0x%x%x\n",    __LINE__, i, (uint32_t)(temp64>>32), (uint32_t)temp64);
		temp64 = *(uint64_t *)encrypt_data[i];
		printf("[%d] encrypt_data[%d] : 0x%x%x\n",__LINE__, i, (uint32_t)(temp64>>32), (uint32_t)temp64);
		temp64 = *(uint64_t *)decrypt_data[i];
		printf("[%d] decrypt_data[%d] : 0x%x%x\n",__LINE__, i, (uint32_t)(temp64>>32), (uint32_t)temp64);

    for(j=0;j<8;j++){
			temp8[j] = src_data[i][j];
			}
		printf("[%d] src_data[%d] : %s\n",      __LINE__, i, temp8);
			for(j=0;j<8;j++){
					temp8[j] = encrypt_data[i][j];
			}
		printf("[%d] encrypt_data[%d] : %s\n",  __LINE__, i, temp8);
			for(j=0;j<8;j++){
					temp8[j] = decrypt_data[i][j];
			}
		printf("[%d] decrypt_data[%d] : %s\n\n",__LINE__, i, temp8);

#if !TEST_DEBUG
			return -1;
		}
#endif
	}
	return 0;
}

void init_timer(float second){
	NVIC_ClearPendingIRQ(n10_TMR0_IRQn  );
	NVIC_EnableIRQ(n10_TMR0_IRQn );
	__enable_irq();
	TMR_Clr_CNT(APB_TMR0);
	TMR_Set_CMP(APB_TMR0, OSC_CLK_FREQ*second); 		// NOTE : Make sure use osc clock
	TMR_Set_Op_Mode(APB_TMR0, TMR_CTL_OP_MODE_WRAPPING);
	TMR_Int_Enable(APB_TMR0);
	TMR_Enable(APB_TMR0);
}

void n10_TMR0_IRQHandler()
{
	TMR_Int_Clr(APB_TMR0);

		APB_GPIO->GPIO_DO.MODIFY = GPIOA_Pin_0;

}
