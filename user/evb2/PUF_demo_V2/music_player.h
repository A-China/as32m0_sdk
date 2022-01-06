#ifndef __MUSIC_PLAYER_H__
#define __MUSIC_PLAYER_H__
#include "as32m0.h"
#include "test_file_system.h"
#include "test_SD_card.h"
#include "vs10xx.h"
uint8_t music_player_func(struct file_node *ptr_file, struct file_node *head_ptr, uint8_t buf[512], uint8_t fat[512], FAT32_info F1);
uint8_t music_player_playfile(struct file_node *ptr_file, uint8_t buf[512], uint8_t fat[512], FAT32_info F1);
#endif

