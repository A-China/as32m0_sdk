#include "test_puf_SD.h"
#include "as32m0_timer.h"
#define TESTASICOSC  16000000UL
uint32_t irq_count=0,  irq_count1=0;

void puf_encrypt_decrypt_file(struct file_node *file_ptr, uint8_t *buf, uint8_t *fat, uint32_t cluster_begin_addr, uint32_t fat_begin_addr, uint32_t sectors_per_cluster, uint8_t mode){
		uint64_t buf64[64] = {0x00};
		uint32_t i, j, count;
//		uint64_t name_ext = 0x00;
		uint32_t cur_addr, nxt_claster_num, cur_claster_num, cur_fat_sector, cur_fat_index, end_sector, num_sector, progress, timer_cnt, timer_cnt1;
		NVIC_ClearPendingIRQ(n10_TMR0_IRQn  );
		NVIC_ClearPendingIRQ(n11_TMR1_IRQn  );
		NVIC_EnableIRQ(n10_TMR0_IRQn );
		NVIC_EnableIRQ(n11_TMR1_IRQn );
		__enable_irq();					
		irq_count=0;
		irq_count1=0;
		TMR_Clr_CNT(APB_TMR0);
		TMR_Set_CMP(APB_TMR0, 0xFFFFFFFF);
		TMR_Set_Op_Mode(APB_TMR0, TMR_CTL_OP_MODE_WRAPPING);
		TMR_Clr_CNT(APB_TMR1);
		TMR_Set_CMP(APB_TMR1, 0xFFFFFFFF);
		TMR_Set_Op_Mode(APB_TMR1, TMR_CTL_OP_MODE_WRAPPING);		
		TMR_Enable(APB_TMR0);		
		TMR_Int_Enable(APB_TMR0);	
		TMR_Int_Enable(APB_TMR1);		
		if(mode == DECRYPT_MODE){
			if(del_puf_sector(file_ptr, cluster_begin_addr, fat_begin_addr, sectors_per_cluster, fat) == 0){
				printf("Decrypt Exit\n");
				return;
			}
		}			
		nxt_claster_num =  file_ptr->file.First_Cluster_number;
		cur_claster_num	= nxt_claster_num;
		end_sector = (file_ptr->file.File_size)>>9;//512 bytes each sector;
		num_sector = end_sector;
		end_sector = end_sector % sectors_per_cluster;
		cur_fat_sector = cur_claster_num>>7;
		SD_spi_read_single_blk(512, (fat_begin_addr + cur_fat_sector), fat);
		cur_fat_index = 0;
		printf("File: %s.%s\n",  file_ptr->file.File_name, file_ptr->file.Exten);
		count=0;
		j=0;
		progress = 0;
	//	systick_initialize(16);
		if(mode == DECRYPT_MODE){
			printf("Decrypting............\n");
		}
		else{
			printf("Encrypting............\n");
		}
		printf("Processing.......0%%");


	
		do{
			cur_claster_num = nxt_claster_num;
			cur_addr = (cur_claster_num - 2)*sectors_per_cluster + cluster_begin_addr;
			cur_fat_index = (cur_claster_num*4) - (cur_fat_sector<<9);
			nxt_claster_num =  (fat[cur_fat_index + 3]<<24)  | (fat[cur_fat_index + 2]<<16) |(fat[cur_fat_index + 1]<<8 )| fat[cur_fat_index];
			for(i=0;(i<sectors_per_cluster);i++){
				__disable_irq();	
				if((nxt_claster_num != 0x0FFFFFFF) || (nxt_claster_num == 0x0FFFFFFF && i<=end_sector)){
					SD_spi_read_single_blk(512, cur_addr, buf);
					__enable_irq();	
					TMR_Enable(APB_TMR1);
					buf8_to_buf64(buf, buf64);
					if(mode == DECRYPT_MODE){
						puf_decrypt(buf64);
					}
					else{
						puf_encrypt(buf64);
					}
					buf64_to_buf8(buf, buf64);
					__disable_irq();
					TMR_Disable(APB_TMR1);
					SD_spi_write_single_blk(512, cur_addr++, buf);
					j++;
				}
			}
			if((cur_fat_index == 508) &&(nxt_claster_num != 0x0FFFFFFF)){
				cur_fat_sector++;
				SD_spi_read_single_blk(512, (fat_begin_addr + cur_fat_sector), fat);
			}
			 count++;
			__enable_irq();	
			if(count == 15){
				progress = j*100;
				progress = progress/num_sector;
				count = 0;
				printf("\r");
				printf("Processing.......%d%%", (uint32_t)progress);
			}
		}while(nxt_claster_num != 0x0FFFFFFF);
		
		if(mode == ENCRYPT_MODE){
			add_puf_sector(file_ptr, cluster_begin_addr, fat_begin_addr, sectors_per_cluster, fat);
		}
		TMR_Disable(APB_TMR0);	
		TMR_Disable(APB_TMR1);	
		TMR_Int_Disable(APB_TMR0);
		TMR_Int_Disable(APB_TMR1);
		__disable_irq();			
		NVIC_DisableIRQ(n10_TMR0_IRQn);
		NVIC_DisableIRQ(n11_TMR1_IRQn);
		timer_cnt = TMR_Get_CNT(APB_TMR0);
		timer_cnt1 = TMR_Get_CNT(APB_TMR1);
		
		timer_cnt = TMR_Get_CNT(APB_TMR0) / OSC_CLK_FREQ + ((0xFFFFFFFF/ OSC_CLK_FREQ ) *irq_count);
		timer_cnt1 = TMR_Get_CNT(APB_TMR1) / OSC_CLK_FREQ + ((0xFFFFFFFF/ OSC_CLK_FREQ ) *irq_count1);
		
		printf("\rProcessing.......Done\n");
		//@ printf("Encrypt/Decrypt used: %d s\n", timer_cnt1);
		//@ printf("Total used: %d s\n", timer_cnt);					
}

void puf_encrypt(uint64_t *buf){
	DES_INFO des_info;
	//memset(encrypt_data, 0, sizeof(encrypt_data));
//	memset(decrypt_data, 0, sizeof(decrypt_data));

	des_info.algorithm_sel   = PUF;
	des_info.key_sel         = FROM_PUF;
	des_info.enc_mode        = ENCRYPT_MODE;
	des_info.keys_info.key_0 = (uint64_t)*(uint64_t *)"01234568";
	des_info.keys_info.key_1 = (uint64_t)*(uint64_t *)"fedcba98";
	des_info.keys_info.key_2 = (uint64_t)*(uint64_t *)"01234568";
	
	apDes_Config(&des_info);
	apDes_Encrypt_Decrypt_Data((uint64_t *)buf, (uint64_t *)buf, 64);
}

void puf_decrypt(uint64_t *buf){
	DES_INFO des_info;
	//memset(encrypt_data, 0, sizeof(encrypt_data));
//	memset(decrypt_data, 0, sizeof(decrypt_data));

	des_info.algorithm_sel   = PUF;
	des_info.key_sel         = FROM_PUF;
	des_info.enc_mode        = DECRYPT_MODE;
	des_info.keys_info.key_0 = (uint64_t)*(uint64_t *)"01234568";
	des_info.keys_info.key_1 = (uint64_t)*(uint64_t *)"fedcba98";
	des_info.keys_info.key_2 = (uint64_t)*(uint64_t *)"01234568";
	
	apDes_Config(&des_info);
	apDes_Encrypt_Decrypt_Data((uint64_t *)buf, (uint64_t *)buf, 64);
}

void buf64_to_buf8(uint8_t *buf, uint64_t *buf64){
	uint8_t i, j;
	for(i=0; i<64; i++, buf64++){
		for(j=0; j<8; j++)
			*buf++ = (uint8_t)(*buf64 >> (8*j));

	}
}

void buf8_to_buf64(uint8_t *buf, uint64_t *buf64){
	uint8_t i, j;
	for(i=0; i<64; i++, buf64++){
		*buf64 = 0;
		for(j=0; j<8; j++){
			*buf64 |= (uint64_t)((uint64_t)(*buf++) << (8*j));
	  }
	}
}
void clear_buf8(uint8_t *buf, uint32_t len ){
	uint32_t i;
	for(i=0;i<len;i++, buf++){
		*buf = 0;
	}


}

void add_puf_sector(struct file_node *file_ptr, uint32_t cluster_begin_addr, uint32_t fat_begin_addr, uint32_t sectors_per_cluster, uint8_t *data_buf){
		uint32_t cur_fat_sector, nxt_claster_num, cur_claster_num, end_sector, cur_addr;
		uint16_t cur_fat_index, remain=0;
	  uint32_t ser_key[24], i;
		nxt_claster_num =  file_ptr->file.First_Cluster_number;
		cur_claster_num	= nxt_claster_num;
		end_sector = (file_ptr->file.File_size)>>9;//512 bytes each sector;
		end_sector = end_sector % sectors_per_cluster;	
		printf("\nFirst Cluster Sector: %x\n", (fat_begin_addr + cur_fat_sector));
		printf("File Name_Sector : %.8x\n", file_ptr->file.Name_Sector_address);
		printf("File Exten_index : %.8x\n", file_ptr->file.Exten_index);
		printf("File First_Cluster_number : %.8x\n", file_ptr->file.First_Cluster_number);
	  printf("End Sector : %d\n", end_sector);
		printf("sectors_per_cluster : %d\n", sectors_per_cluster);
		cur_fat_sector = cur_claster_num>>7;
		SD_spi_read_single_blk(512, (fat_begin_addr + cur_fat_sector), data_buf);
		do{
			cur_claster_num = nxt_claster_num;
			//printf("cur_claster_num %x\n", cur_claster_num);
			cur_fat_index = (cur_claster_num*4) - (cur_fat_sector<<9);
			nxt_claster_num =  (data_buf[cur_fat_index + 3]<<24)  | (data_buf[cur_fat_index + 2]<<16) |(data_buf[cur_fat_index + 1]<<8 )| data_buf[cur_fat_index];	
			if((cur_fat_index == 508) &&(nxt_claster_num != 0x0FFFFFFF)){
				cur_fat_sector++;
				SD_spi_read_single_blk(512, (fat_begin_addr + cur_fat_sector), data_buf);
			}			
		}while(nxt_claster_num != 0x0FFFFFFF);
		// Add a sector
			if(end_sector < sectors_per_cluster){
				cur_addr = (cur_claster_num - 2)*sectors_per_cluster + cluster_begin_addr;
				cur_addr = cur_addr + end_sector;
				//printf("\nLast Sector Address: %.8x", cur_addr);
				clear_buf8(data_buf, 512);
				Read_puf_key(ser_key);
			/*	
				for(i=0;i<24;i++){
					printf("KEY %d : %.8x\n", i, ser_key[i]);
				}
			*/
				for(i=0;i<24;i++){
					if(i%2)
						ser_key[i] = ser_key[i] &0xFF;
					else
						ser_key[i] = ser_key[i]>>24;
					
			//		printf("KEY %d : %.8x\n", i, ser_key[i]);
				}	
				remain = 512 - (file_ptr->file.File_size%512);
				//printf("Remainder : %d\n", remain);				
				data_buf[0] = 'P';
				data_buf[1] = 'U';
				data_buf[2] = 'F';
				data_buf[3] = remain & 0xFF;
				data_buf[4] = remain>>8;
				data_buf[64] = 'K';
				data_buf[65] = 'E';
				data_buf[66] = 'Y';		
				data_buf[128] = file_ptr->file.Exten[0];
				data_buf[129] = file_ptr->file.Exten[1];
				data_buf[130] = file_ptr->file.Exten[2];
				for(i=0;i<24;i++){
					data_buf[192+i] = ser_key[i];
				}

				data_buf[508] = 0xFF;
				data_buf[509] = 0x5A;
				data_buf[510] = 0xF5;
				data_buf[511] = 0xAA;				
				//SD_printf_datamap(512, 0x0000, data_buf);
				// Write PUF INF to last Sector
				SD_spi_write_single_blk(512, ++cur_addr, data_buf);
				// Read Name Sector
				SD_spi_read_single_blk(512, file_ptr->file.Name_Sector_address, data_buf);
				//SD_printf_datamap(512, 0x0000, data_buf);
				data_buf[file_ptr->file.Exten_index] = 'P';
				data_buf[file_ptr->file.Exten_index + 1] = 'U';
				data_buf[file_ptr->file.Exten_index + 2] = 'F';
				file_ptr->file.File_size +=(512 + remain);
				//printf("file_ptr->file.File_size: %.8x", file_ptr->file.File_size);
				data_buf[file_ptr->file.Exten_index + 0x14] = (uint8_t) file_ptr->file.File_size;
				data_buf[file_ptr->file.Exten_index + 0x15] = (uint8_t)(file_ptr->file.File_size>>8);
				data_buf[file_ptr->file.Exten_index + 0x16] = (uint8_t)(file_ptr->file.File_size>>16);	
				data_buf[file_ptr->file.Exten_index + 0x17] = (uint8_t)(file_ptr->file.File_size>>24);
				//SD_printf_datamap(512, 0x0000, data_buf);
				SD_spi_write_single_blk(512, file_ptr->file.Name_Sector_address, data_buf);
			}
			else{
				printf("end_sector >= sectors_per_cluster\n");
			// Read Name Sector
				// Change File Type to PUF
				// Change File Size
				// Change First Cluster Number
			// Write Name Sector
			}

}

uint8_t del_puf_sector(struct file_node *file_ptr, uint32_t cluster_begin_addr, uint32_t fat_begin_addr, uint32_t sectors_per_cluster, uint8_t *data_buf){
		uint32_t cur_fat_sector, nxt_claster_num, cur_claster_num, end_sector, cur_addr;
		uint16_t cur_fat_index, remain;
	  uint8_t temp[3], temp2;
		uint32_t ser_key[24], i;
		nxt_claster_num =  file_ptr->file.First_Cluster_number;
		cur_claster_num	= nxt_claster_num;
		end_sector = (file_ptr->file.File_size)>>9;//512 bytes each sector;
		end_sector = (end_sector % sectors_per_cluster) - 1;	
		printf("\nFirst Cluster Sector: %x\n", (fat_begin_addr + cur_fat_sector));
		printf("File Name_Sector : %.8x\n", file_ptr->file.Name_Sector_address);
		printf("File Exten_index : %.8x\n", file_ptr->file.Exten_index);
		printf("File First_Cluster_number : %.8x\n", file_ptr->file.First_Cluster_number);
	  printf("End Sector : %d\n", end_sector);
		printf("sectors_per_cluster : %d\n", sectors_per_cluster);
		cur_fat_sector = cur_claster_num>>7;
		SD_spi_read_single_blk(512, (fat_begin_addr + cur_fat_sector), data_buf);
	
		do{
			cur_claster_num = nxt_claster_num;
			//printf("cur_claster_num %x\n", cur_claster_num);
			cur_fat_index = (cur_claster_num*4) - (cur_fat_sector<<9);
			nxt_claster_num =  (data_buf[cur_fat_index + 3]<<24)  | (data_buf[cur_fat_index + 2]<<16) |(data_buf[cur_fat_index + 1]<<8 )| data_buf[cur_fat_index];	
			if((cur_fat_index == 508) &&(nxt_claster_num != 0x0FFFFFFF)){
				cur_fat_sector++;
				SD_spi_read_single_blk(512, (fat_begin_addr + cur_fat_sector), data_buf);
			}			
		}while(nxt_claster_num != 0x0FFFFFFF);
		// Add a sector
			if(end_sector <= sectors_per_cluster){
				cur_addr = (cur_claster_num - 2)*sectors_per_cluster + cluster_begin_addr;
				cur_addr = cur_addr + end_sector;
		//		printf("\nLast Sector Address: %.8x", cur_addr);
				SD_spi_read_single_blk(512, cur_addr, data_buf);
				//SD_printf_datamap(512, 0x0000, data_buf);
				temp[0] = data_buf[128];
				temp[1] = data_buf[129];
				temp[2] = data_buf[130];
				remain = (data_buf[4] << 8 | data_buf[3]); 
				Read_puf_key(ser_key);
				for(i=0;i<24;i++){
					if(i%2)
						ser_key[i] = ser_key[i] &0xFF;
					else
						ser_key[i] = ser_key[i]>>24;
				}
				for(i=0, temp2=1;(i<24 && temp2==1);i++){
					if(data_buf[192+i]==ser_key[i])
						temp2=1;
					else
						temp2=0;
				}
				 //temp2=1;
				if((data_buf[0] == 'P' && data_buf[1] == 'U' && data_buf[2] == 'F') && (data_buf[508] == 0xFF && data_buf[509] == 0x5A && data_buf[510] == 0xF5 && data_buf[511] == 0xAA))
				{
					if(temp2==1){
					 clear_buf8(data_buf, 512);
					 SD_spi_write_single_blk(512, cur_addr, data_buf);
					 SD_spi_read_single_blk(512, file_ptr->file.Name_Sector_address, data_buf);
					 //SD_printf_datamap(512, 0x0000, data_buf);
					 data_buf[file_ptr->file.Exten_index] = temp[0];
					 data_buf[file_ptr->file.Exten_index + 1] = temp[1];
					 data_buf[file_ptr->file.Exten_index + 2] = temp[2];
					 file_ptr->file.File_size -=(512 + remain);
					 //printf("file_ptr->file.File_size: %.8x", file_ptr->file.File_size);
					 data_buf[file_ptr->file.Exten_index + 0x14] = (uint8_t) file_ptr->file.File_size;
					 data_buf[file_ptr->file.Exten_index + 0x15] = (uint8_t)(file_ptr->file.File_size>>8);
					 data_buf[file_ptr->file.Exten_index + 0x16] = (uint8_t)(file_ptr->file.File_size>>16);	
					 data_buf[file_ptr->file.Exten_index + 0x17] = (uint8_t)(file_ptr->file.File_size>>24);
					 //SD_printf_datamap(512, 0x0000, data_buf);		
					 SD_spi_write_single_blk(512, file_ptr->file.Name_Sector_address, data_buf);
					}
					else{
						printf("KEY NOT MATCHED\n");
						return 0;
					}	
				}
				// Write PUF INF to last Sector
				//SD_spi_write_single_blk(512, ++cur_addr, data_buf);
				// Read Name Sector
				else{
					printf("Corrupted PUF File \n");
					return 0;
				}
					
			/*	
				SD_spi_read_single_blk(512, file_ptr->file.Name_Sector_address, data_buf);
				SD_printf_datamap(512, 0x0000, data_buf);
				data_buf[file_ptr->file.Exten_index] = 'P';
				data_buf[file_ptr->file.Exten_index + 1] = 'U';
				data_buf[file_ptr->file.Exten_index + 2] = 'F';
				file_ptr->file.File_size +=512;
				printf("file_ptr->file.File_size: %.8x", file_ptr->file.File_size);
				data_buf[file_ptr->file.Exten_index + 0x14] = (uint8_t) file_ptr->file.File_size;
				data_buf[file_ptr->file.Exten_index + 0x15] = (uint8_t)(file_ptr->file.File_size>>8);
				data_buf[file_ptr->file.Exten_index + 0x16] = (uint8_t)(file_ptr->file.File_size>>16);	
				data_buf[file_ptr->file.Exten_index + 0x17] = (uint8_t)(file_ptr->file.File_size>>24);
				SD_printf_datamap(512, 0x0000, data_buf);
				*/
			}
			else{
				printf("end_sector >= sectors_per_cluster\n");
				return 0;
			// Read Name Sector
				// Change File Type to PUF
				// Change File Size
				// Change First Cluster Number
			// Write Name Sector
			}
		return 1;
}

struct file_node * music_file_list(struct file_node *file){
	struct file_node *head_ptr, *prev_ptr, *tmp_ptr;
	head_ptr = file;
	prev_ptr = file;
	do{
	//	if((file->file.Exten[0] == 'P' && file->file.Exten[1] == 'U' && file->file.Exten[2] == 'F')){
		if(((file->file.Exten[0] == 'M' && file->file.Exten[1] == 'P' && file->file.Exten[2] == '3') || (file->file.Exten[0] == 'W' && file->file.Exten[1] == 'A' && file->file.Exten[2] == 'V'))==0){
			if(head_ptr == file){
				head_ptr = file->nxtfile_node;
	//			printf("PUF FILE\n");
				prev_ptr = file;
			}
			else{
	//			printf("PUF FILE NOT HEAD\n");
				prev_ptr->nxtfile_node = file->nxtfile_node;
				tmp_ptr = file->nxtfile_node;
				tmp_ptr->prevfile_node = prev_ptr;
			}
		}
		else{
	//		printf("NOT PUF FILE\n");
			prev_ptr = file;
		
		}
		
		file = file->nxtfile_node;
	} while(file != NULL);	
   return head_ptr;
}

struct file_node * encrypt_file_list(struct file_node *file){
	struct file_node *head_ptr, *prev_ptr, *tmp_ptr;
	head_ptr = file;
	prev_ptr = file;
	do{
		if((file->file.Exten[0] == 'P' && file->file.Exten[1] == 'U' && file->file.Exten[2] == 'F')){
			if(head_ptr == file){
				head_ptr = file->nxtfile_node;
				prev_ptr = file;
			//	printf("NOT PUF FILE\n");
			}
			else{
			//	printf("NOT PUF FILE NOT HEAD\n");
				prev_ptr->nxtfile_node = file->nxtfile_node;
				tmp_ptr = file->nxtfile_node;
				tmp_ptr->prevfile_node = prev_ptr;
				//prev_ptr = file;
			}
		}
		else{
//			printf("PUF FILE\n");
//			file->
			prev_ptr = file;
		
		}
		file = file->nxtfile_node;
	} while(file != NULL);	
   return head_ptr;

}

struct file_node * puf_file_list(struct file_node *file){
	struct file_node *head_ptr, *prev_ptr, *tmp_ptr;
	head_ptr = file;
	prev_ptr = file;
	do{
		if((file->file.Exten[0] == 'P' && file->file.Exten[1] == 'U' && file->file.Exten[2] == 'F') == 0){
			if(head_ptr == file){
				head_ptr = file->nxtfile_node;
				prev_ptr = file;
			//	printf("NOT PUF FILE\n");
			}
			else{
			//	printf("NOT PUF FILE NOT HEAD\n");
				prev_ptr->nxtfile_node = file->nxtfile_node;
				tmp_ptr = file->nxtfile_node;
				tmp_ptr->prevfile_node = prev_ptr;
				//prev_ptr = file;
			}
		}
		else{
//			printf("PUF FILE\n");
//			file->
			prev_ptr = file;
		
		}
		file = file->nxtfile_node;
	} while(file != NULL);	
   return head_ptr;
}

void show_puf_key(){
	uint32_t ser_key[24], i;
	Read_puf_key(ser_key);
	printf("\n PUF Key : ");
	printf("0x");
	for(i=0;i<24;i++){
	 printf("%.8x", ser_key[23-i]);
	}
  printf("\n"); 
	printf("\nPUF key in decimal: ");
	for(i=0;i<24;i++){
	 printf("%d", ser_key[23-i]);	
	}
};

void n10_TMR0_IRQHandler()
{
	TMR_Int_Clr(APB_TMR0);
	irq_count++;
}
void n11_TMR1_IRQHandler()
{
	TMR_Int_Clr(APB_TMR1);
	irq_count1++;
}