#include "as32m0_des.h"

#define DES_DEBUG     0

static void des_write_reg(uint32_t reg, uint32_t val)
{
	*(volatile uint32_t * )(DES_PHY_ADDR + reg) = val;
}

static uint32_t des_read_reg(uint32_t reg)
{
	return *(volatile uint32_t * )(DES_PHY_ADDR + reg);
}

static void des_clr_bits(uint32_t reg, uint32_t val)
{
	*(volatile uint32_t * )(DES_PHY_ADDR + reg) &= ~(val);
}

static void des_set_bits(uint32_t reg, uint32_t val)
{
	*(volatile uint32_t * )(DES_PHY_ADDR + reg) |= val;
}

static int32_t des_check_busy(void)
{
	uint32_t retry = 1000;
	while(retry--){
		if(!(des_read_reg(DES_STATUS) & DES_STATUS_DES_BUSY)){
			return 0;
		}
	}
	return -1;
}

static int32_t des_check_input_ready(void)
{
	uint32_t retry = 1000;
	while(retry--){
		if(des_read_reg(DES_STATUS) & DES_STATUS_INPUT_READY){
			return 0;
		}
	}
	return -1;
}

static int32_t des_check_output_ready(void)
{
	uint32_t retry = 1000;
	while(retry--){
		if(des_read_reg(DES_STATUS) & DES_STATUS_OUTPUT_READY){
			return 0;
		}
	}
	return -1;
}

void apDes_Config(DES_INFO_PTR des_info_ptr)
{
        int32_t reg_val = 0;

		reg_val = (uint32_t)((des_info_ptr->algorithm_sel << DES_MODE_BIT_ALGORITHM_SEL) | \
							 (des_info_ptr->enc_mode      << DES_MODE_BIT_ENC          ) | \
							 (des_info_ptr->key_sel       << DES_MODE_BIT_KEY_SEL      ) );

		des_write_reg(DES_MODE, reg_val);

		if(des_info_ptr->key_sel == FROM_PUF){
			des_set_bits(DES_PUF_ENABLE, DES_PUF_EN);
		}else{
				des_write_reg(DES_KEY0, des_info_ptr->keys_info.key_0 & 0xFFFFFFFF);
				des_write_reg(DES_KEY1, des_info_ptr->keys_info.key_0 >> 32);
				if(des_info_ptr->algorithm_sel == TDES){
						des_write_reg(DES_KEY2, des_info_ptr->keys_info.key_1 & 0xFFFFFFFF);
						des_write_reg(DES_KEY3, des_info_ptr->keys_info.key_1 >> 32);
						des_write_reg(DES_KEY4, des_info_ptr->keys_info.key_2 & 0xFFFFFFFF);
						des_write_reg(DES_KEY5, des_info_ptr->keys_info.key_2 >> 32);
			}
		}

		apDes_Enable();
}

int32_t apDes_Write_Data(uint64_t *data)
{
	int32_t ret = -1;
	ret = des_check_input_ready();
	if(ret < 0){
		printf("[%d] des_check_input_ready time out \n",__LINE__);
		return -1;
	}
#if DES_DEBUG
	printf("DES_KEY0 VAL : %#x \n", des_read_reg(DES_KEY0));
	printf("DES_KEY1 VAL : %#x \n", des_read_reg(DES_KEY1));
	printf("DES_KEY2 VAL : %#x \n", des_read_reg(DES_KEY2));
	printf("DES_KEY3 VAL : %#x \n", des_read_reg(DES_KEY3));
	printf("DES_KEY4 VAL : %#x \n", des_read_reg(DES_KEY4));
	printf("DES_KEY5 VAL : %#x \n", des_read_reg(DES_KEY5));
#endif
	des_write_reg(DES_DINL, (uint32_t)((*data) & 0xFFFFFFFF));

	des_write_reg(DES_DINH, (uint32_t)((*data) >> 32));

	return 0;
}

int32_t apDes_Read_Data(uint64_t *data)
{
	int32_t ret = -1;
	uint64_t reg_vall = 0, reg_valh = 0;
	ret = des_check_output_ready();
	if(ret < 0){
		printf("[%d] des_check_output_ready time out \n",__LINE__);
		return -1;
	}

	reg_vall = des_read_reg(DES_DOUTL);
	reg_valh = des_read_reg(DES_DOUTH);


	*data =(uint64_t)((reg_valh << 32) | reg_vall);

	return 0;
}

int32_t apDes_Encrypt_Decrypt_Data(uint64_t *Serc_data, uint64_t *Dst_data,uint32_t count)
{
	int32_t ret = -1;
	uint32_t data_count = 0;
	for(data_count=0; data_count<count; data_count++){
		ret = apDes_Write_Data(&Serc_data[data_count]);
		if(ret < 0){
			printf("apDes_Write_Data Time Out \n");
			return -1;
		}
		ret = apDes_Read_Data(&Dst_data[data_count]);
		if(ret < 0){
			printf("apDes_Read_Data Time Out \n");
			return -1;
		}
	}

	return 0;
}
void apDes_Enable(void)
{
	des_set_bits(DES_ENABLE,DES_EN);
}
void apDes_Disable(void)
{
	des_clr_bits(DES_ENABLE,DES_EN);
}


int32_t apDes_Debug_Test(void)
{
	uint32_t retry = 10000;
	des_set_bits(DES_MODE, DES_MODE_QUARITY_MODE);
	des_set_bits(DES_PUF_ENABLE, DES_PUF_EN);
	while(des_read_reg(DES_STATUS) & DES_STATUS_QUALITY_DONE != DES_STATUS_QUALITY_DONE){
		retry--;
		if(retry == 0){
			printf("Des check Quality Time Out \n");
			return -1;
		}
	}
	if(des_read_reg(DES_STATUS) & DES_STATUS_QUALITY_STATE != DES_STATUS_QUALITY_STATE){
		printf("Des Quality Failed \n");
		return -1;
	}
	return 0;
}

static void set_des_key(uint32_t key)
{
		*(uint32_t *)(DES_REG_BASE + DES_KEY0) = key;
        *(uint32_t *)(DES_REG_BASE + DES_KEY1) = key;
        *(uint32_t *)(DES_REG_BASE + DES_KEY2) = key;
        *(uint32_t *)(DES_REG_BASE + DES_KEY3) = key;
        *(uint32_t *)(DES_REG_BASE + DES_KEY4) = key;
        *(uint32_t *)(DES_REG_BASE + DES_KEY5) = key;
	
}

void Read_puf_key(uint32_t* ser_key){
	uint32_t i=0;
	uint64_t Data=0x000016466464;
	uint32_t temp[8][6];
	uint32_t bk;
  int32_t reg_val = 0;
  des_write_reg(DES_KEY0, 0x0);
	des_write_reg(DES_KEY1, 0x0);
	des_write_reg(DES_KEY2, 0x0);
	des_write_reg(DES_KEY3, 0x0);
	des_write_reg(DES_KEY4, 0x0);
	des_write_reg(DES_KEY5, 0x0);
	reg_val = (uint32_t)((PUF << DES_MODE_BIT_ALGORITHM_SEL) | \
						 (FROM_PUF      << DES_MODE_BIT_ENC          ) | \
						 (ENCRYPT_MODE       << DES_MODE_BIT_KEY_SEL      ) );
	des_write_reg(DES_MODE, reg_val);
	des_set_bits(DES_PUF_ENABLE, DES_PUF_EN);
	apDes_Enable();	
	bk = des_read_reg(DES_STATUS);
	des_write_reg(DES_STATUS, 0x2d1d3010);

	apDes_Write_Data(&Data);
	for(i=0;i<8;i++){
			temp[i][0]=des_read_reg(DES_KEY0);
			temp[i][1]=des_read_reg(DES_KEY1);
			temp[i][2]=des_read_reg(DES_KEY2);
			temp[i][3]=des_read_reg(DES_KEY3);
			temp[i][4]=des_read_reg(DES_KEY4);
			temp[i][5]=des_read_reg(DES_KEY5);	
	}
/*	
	for(i=0;i<8;i++){	
		printf("DES_KEY0[%d] VAL : %#x \n", i, temp[i][0]);
		printf("DES_KEY1[%d] VAL : %#x \n", i, temp[i][1]);
		printf("DES_KEY2[%d] VAL : %#x \n", i, temp[i][2]);
		printf("DES_KEY3[%d] VAL : %#x \n", i, temp[i][3]);
		printf("DES_KEY4[%d] VAL : %#x \n", i, temp[i][4]);
		printf("DES_KEY5[%d] VAL : %#x \n", i, temp[i][5]);
	}
*/
	for(i=0;i<24;i++){
		*ser_key = temp[(i/6+2)][i%6];
		ser_key++;
	}
		apDes_Read_Data(&Data);
	des_write_reg(DES_STATUS, bk);
}
