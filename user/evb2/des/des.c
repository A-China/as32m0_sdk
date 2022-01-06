#include <string.h>
#include "printf_config.h"
#include "as32m0_des.h"
#include "as32m0_misc.h"

#define DATA_SIZE 16
#define TEST_DEBUG	1

void _sys_exit(void);
int32_t des_check_result(void);

char src_data[DATA_SIZE][8] = {"eeff0c00","36987412","qWeRrTyU","qwertyui",\
		                       "zxcvbnmm","eeff0c00","36987412","qWeRrTyU",\
							   "qwertyui","zxcvbnmm","eeff0c00","36987412",\
							   "eeff0c00","qwertyui","qWeRrTyU","zxcvbnmm"};
char decrypt_data[DATA_SIZE][8] = {0};
char encrypt_data[DATA_SIZE][8] = {0};
uint32_t data_count=0;
char temp[3] = {0};

int main()
{
	DES_INFO des_info;
	int32_t ret = -1;
	uint32_t i;
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
	printf("data_count : %d \n",data_count);	
	printf("Des form Reg test \n");
	memset(encrypt_data, 0, sizeof(encrypt_data));
	memset(decrypt_data, 0, sizeof(decrypt_data));
	des_info.algorithm_sel   = DES;						// DES/TDES/PUF
	des_info.key_sel         = FROM_REG;			// FROM_REG/FROM_PUF
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
	printf("==> \n");
	printf("==> \n");
	printf("==> TEST DES End \n");
	printf("==> \n");
	printf("==> \n");

	printf("#"); // invoke TB $finish;
	_sys_exit();
	return 0x12;
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
