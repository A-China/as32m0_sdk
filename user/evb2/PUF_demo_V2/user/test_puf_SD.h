#ifndef __TEST_PUF_SD_H__
#define __TEST_PUF_SD_H__
#include "as32m0_des.h"
#include "test_file_system.h"
#include "test_SD_card.h"

void puf_encrypt_decrypt_file(struct file_node *file, uint8_t *buf, uint8_t *fat, uint32_t cluster_begin_addr, uint32_t fat_begin_addr, uint32_t sectors_per_cluster, uint8_t mode);
void puf_encrypt(uint64_t *buf);
void puf_decrypt(uint64_t *buf);
void buf64_to_buf8(uint8_t *buf, uint64_t *buf64 );
void buf8_to_buf64(uint8_t *buf, uint64_t *buf64 );
void clear_buf8(uint8_t *buf, uint32_t len );
void add_puf_sector(struct file_node *file, uint32_t cluster_begin_addr, uint32_t fat_begin_addr, uint32_t sectors_per_cluster, uint8_t *data_buf);
uint8_t del_puf_sector(struct file_node *file, uint32_t cluster_begin_addr, uint32_t fat_begin_addr, uint32_t sectors_per_cluster, uint8_t *data_buf);
struct file_node * music_file_list(struct file_node *file);
struct file_node * encrypt_file_list(struct file_node *file);
struct file_node * puf_file_list(struct file_node *file);
void show_puf_key(void);
#endif

