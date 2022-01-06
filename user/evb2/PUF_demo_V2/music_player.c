#include "music_player.h"
uint8_t music_player_func(struct file_node *ptr_file, struct file_node *head_ptr, uint8_t buf[512], uint8_t fat[512], FAT32_info F1){
//	struct file_node *head_ptr;
//	head_ptr = ptr_file;
	while(1){
			if(ptr_file != NULL){
				if(music_player_playfile(ptr_file, buf, fat,  F1) == 27){
					break;
				}
				ptr_file = ptr_file->nxtfile_node;
			}
			else
				ptr_file = head_ptr;
		}
	return 1;
};

uint8_t music_player_playfile(struct file_node *ptr_file, uint8_t buf[512], uint8_t fat[512], FAT32_info	F1){
		uint16_t  i;
		uint8_t in;
		uint32_t cur_addr, nxt_claster_num, cur_claster_num, cur_fat_sector, cur_fat_index, end_sector;
		softReset(0);
		cur_claster_num = ptr_file->file.First_Cluster_number;
		nxt_claster_num = cur_claster_num;
		end_sector = (ptr_file->file.File_size)>>9;//512 bytes each sector;
		end_sector = end_sector % F1.sectors_per_cluster;
		cur_fat_sector = cur_claster_num>>7;
		cur_fat_index = 0;
		SD_spi_read_single_blk(512, (F1.fat_begin_lba + cur_fat_sector), fat);
		writeRegister(SPI_MODE, 0x0800);
		printf("Playing Music File ");
		printf("File %s.%s\n", ptr_file->file.File_name, ptr_file->file.Exten);
		do{
			cur_claster_num = nxt_claster_num;
			cur_addr = (cur_claster_num - 2)*F1.sectors_per_cluster + F1.cluster_begin_lba;
			cur_fat_index = (cur_claster_num*4) - (cur_fat_sector<<9);
			nxt_claster_num =  (fat[cur_fat_index + 3]<<24)  | (fat[cur_fat_index + 2]<<16) |(fat[cur_fat_index + 1]<<8 )| fat[cur_fat_index];
			for(i=0;(i<F1.sectors_per_cluster);i++){
				if((nxt_claster_num != 0x0FFFFFFF) || (nxt_claster_num == 0x0FFFFFFF && i<=end_sector)){
					SD_spi_read_single_blk(512, cur_addr++, buf);
					writeData(buf);
				}
			}
		
			in = SER_GetChar();
			if(in==27){
				return 27;
			}
			
			if((cur_fat_index == 508) &&(nxt_claster_num != 0x0FFFFFFF)){
				cur_fat_sector++;
				SD_spi_read_single_blk(512, (F1.fat_begin_lba + cur_fat_sector), fat);		
			}
		}while(nxt_claster_num != 0x0FFFFFFF);

		writeRegister(SPI_MODE, 0x0808);
	return 1;
}
