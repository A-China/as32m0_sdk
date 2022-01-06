#include "test_file_system.h"

struct file_node g_file_list[16];

uint8_t return_chosen_file_ptr(struct file_node *file_ptr){
	uint8_t in;
	uint32_t in2;
	show_file_list(file_ptr);
	printf("Enter a number to select the file : ");
	scanf("%d", &in2);
	printf("\n");
	in = (uint8_t)in2;
	return in;
}

void show_file_list(struct file_node *file_ptr){
		uint32_t i=0;
		do{
			printf("%d. %s.%s\n", (i+1), file_ptr->file.File_name, file_ptr->file.Exten);
			file_ptr = file_ptr->nxtfile_node;
			i++;
		}while(file_ptr != NULL);
}

void file_extract_parttition_table(File_Partition_entry *Partition, uint8_t *sector){
		uint8_t i;
	  sector = sector + 446;
		Partition->BootFlag = *sector++;
	  for(i=0;i<3;i++){
			Partition->CHS_Begin |= (*sector++)<<(i*8);
		}
		Partition->TypeCode = *sector++;
	  for(i=0;i<3;i++){
			Partition->CHS_End |= (*sector++)<<(i*8);
		}
	  for(i=0;i<4;i++){
			Partition->LBA_Begin |= (*sector++)<<(i*8);
		}
	  for(i=0;i<4;i++){
			Partition->NumOfSector |= (*sector++)<<(i*8);
		}	
		printf("Partition 1 BootFlag = %.2x\n", Partition->BootFlag);
		printf("Partition 1 CHS_Begin = %.8x\n", Partition->CHS_Begin);
		printf("Partition 1 TypeCode = %.2x\n", Partition->TypeCode);
		printf("Partition 1 CHS_End = %.8x\n", Partition->CHS_End);
		printf("Partition 1 LBA_Begin = %.8x\n", Partition->LBA_Begin);
		printf("Partition 1 NumOfSector = %x\n", Partition->NumOfSector);
		printf("Partition 1 Size = %d GB\n", ((Partition->NumOfSector)/(2*1024*1024)));
}

void file_extract_FAT32(FAT32_info *FAT32_entry, uint8_t *sector, uint32_t LBA_addr){
		uint32_t Number_of_Reserved_Sectors=0, Sectors_Per_FAT=0;
	  uint8_t Number_of_FATs  , i, *tmp_ptr;
	  tmp_ptr = sector;
		printf("Bytes Per Sector = %.2x%.2x \n",*(sector + 0x0C), *(sector + 0x0B));

		sector = sector + 0x0D;
		FAT32_entry->sectors_per_cluster = *sector++;
	  for(i=0;i<2;i++){
			Number_of_Reserved_Sectors |= (*sector++)<<(i*8);
		}
	  Number_of_FATs = (*sector++);
	  sector = tmp_ptr + 0x24;
	  for(i=0;i<4;i++){
			Sectors_Per_FAT |= (*sector++)<<(i*8);
		}		
		sector = tmp_ptr + 0x2C;
		FAT32_entry->root_dir_first_cluster = (*sector++);
		FAT32_entry->fat_begin_lba = LBA_addr + Number_of_Reserved_Sectors;
		FAT32_entry->cluster_begin_lba = FAT32_entry->fat_begin_lba + (Number_of_FATs * Sectors_Per_FAT);
		FAT32_entry->Sectors_Per_FAT = Sectors_Per_FAT;
		printf("Sectors Per Cluster = %d \n", FAT32_entry->sectors_per_cluster );
		printf("Number of Reserved Sectors = %d \n", Number_of_Reserved_Sectors);
		printf("Number of FATs = %d \n", Number_of_FATs);
		printf("Sectors Per FAT = %d	\n", FAT32_entry->Sectors_Per_FAT);
		printf("Root Directory First Cluster = %d \n", FAT32_entry->root_dir_first_cluster);
		printf("FAT BEGIN LBA = %.8x	\n", FAT32_entry->fat_begin_lba);
		printf("Cluster BEGIN LBA = %.8x	\n", FAT32_entry->cluster_begin_lba);
}

struct file_node *file_extract_directory(uint32_t cur_addr, uint8_t *sector_data, uint32_t cluster_begin_addr, uint32_t sectors_per_cluster, struct file_node *file_list, uint32_t *num){
		uint8_t i=0;
	while((i<16) && *num<128){
		 if(file_extract_checkfile(sector_data, cluster_begin_addr, sectors_per_cluster)){
			  file_list ->file  = file_extract_file(cur_addr, sector_data, cluster_begin_addr, sectors_per_cluster, i);
			// if( (file.Exten[0] == 'M' && file.Exten[1] == 'P' && file.Exten[2] == '3' ) || (file.Exten[0] == 'W' && file.Exten[1] == 'A' && file.Exten[2] == 'V' ) || (file.Exten[0] == 'P' && file.Exten[1] == 'U' && file.Exten[2] == 'F' ) ||
			//			(file.Exten[0] == 'P' && file.Exten[1] == 'D' && file.Exten[2] == 'F' )){
				file_list ->nxtfile_node = &g_file_list[*num+1];
				file_list = file_list->nxtfile_node;
				file_list->prevfile_node = &g_file_list[*num];
				*num = *num + 1;
			// }
		 }
		sector_data = sector_data + 0x20;
		i++;
  }
	return file_list;
}

file file_extract_file(uint32_t cur_addr, uint8_t *sector, uint32_t cluster_begin_addr, uint32_t sectors_per_cluster, uint8_t entry){
	file file1;
	uint8_t *tmp_ptr,  i;
	uint32_t File_size=0, First_Cluster_number=0;
	uint16_t First_Cluster_number_hi=0, First_Cluster_number_lo=0;
			tmp_ptr = sector;
			sector = sector + 0xB;

				sector = tmp_ptr;
	//			printf("\n");
				for(i=0;i<8;i++, sector++){
					file1.File_name[i] = *sector;
				}
				file1.File_name[8] = '\0';
				for(i=0;i<3;i++, sector++){
					file1.Exten[i] = *sector;
				}
				file1.Exten[3] = '\0';
//				printf("\n");
				sector = tmp_ptr + 0x14;
				for(i=0;i<2;i++){
					First_Cluster_number_hi |= *(sector++)<<(i*8);
			}		
				sector = tmp_ptr + 0x1A;
				for(i=0;i<2;i++){
					First_Cluster_number_lo |= *(sector++)<<(i*8);
			}		
					First_Cluster_number =  First_Cluster_number_hi << 16 | First_Cluster_number_lo;
			  file1.Name_Sector_address = cur_addr;
			  file1.Exten_index	= entry*0x20 + 0x8;
				sector = tmp_ptr + 0x1C;
				for(i=0;i<4;i++){
					File_size |= *(sector++)<<(i*8);
				}		
        file1.File_size = File_size;			
				file1.First_Cluster_number = First_Cluster_number;
				return file1;
}

uint8_t file_extract_checkfile(uint8_t *sector, uint32_t cluster_begin_addr, uint32_t sectors_per_cluster){
			if((*sector==0x00) || (*sector==0xe5))
				return 0;
			else	
			{
				sector = sector + 0xB;
				if((*sector) == 0x20)
					return 1;
				else
					return 0;
			}
}


