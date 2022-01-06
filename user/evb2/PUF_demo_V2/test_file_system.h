#ifndef __TEST_FILE_SYSTEM_H__
#define __TEST_FILE_SYSTEM_H__
#include "as32m0.h"
#include "printf_config.h"
typedef struct 
{
    uint8_t BootFlag;
    uint32_t CHS_Begin;
    uint8_t TypeCode;
    uint32_t CHS_End;
	uint32_t LBA_Begin;
	uint32_t NumOfSector;
}File_Partition_entry;

typedef struct 
{
    uint32_t fat_begin_lba;
    uint32_t cluster_begin_lba;
    uint8_t sectors_per_cluster;
    uint32_t root_dir_first_cluster;
	  uint32_t Sectors_Per_FAT;
}FAT32_info;

typedef struct 
{
    uint8_t  File_name[9];
		uint8_t  Exten[4];
    uint32_t Name_Sector_address;
		uint16_t  Exten_index;
    uint32_t File_size;
	  uint32_t First_Cluster_number;
}file;

struct file_node
{
    file file;
	  struct file_node *nxtfile_node;
		struct file_node *prevfile_node;
};
uint8_t return_chosen_file_ptr(struct file_node *file_ptr);
void show_file_list(struct file_node *file_ptr);
void file_extract_parttition_table(File_Partition_entry *Partition, uint8_t *sector);
void file_extract_FAT32(FAT32_info *FAT32_entry, uint8_t *sector, uint32_t LBA_addr);
struct file_node *file_extract_directory(uint32_t cur_addr, uint8_t *sector_data, uint32_t cluster_begin_addr, uint32_t sectors_per_cluster,struct file_node *file_list, uint32_t *num);
file file_extract_file(uint32_t cur_addr, uint8_t *sector, uint32_t cluster_begin_addr, uint32_t sectors_per_cluster, uint8_t entry);
uint8_t file_extract_checkfile(uint8_t *sector, uint32_t cluster_begin_addr, uint32_t sectors_per_cluster);

#endif

