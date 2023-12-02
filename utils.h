#ifndef UTILS_H
#define UTILS_H
#include "cacheBlock.h"

int block_offset(int addr);
int block_address(int addr);
int victim_tag(int addr);
int l1_index(int addr);
int l1_tag(int addr);
int l1_tag_to_victim_tag(int l1Tag);
bool victim_cache_block_is_target(const CacheBlock &block, int addr);
int get_addr_from_l1_tag_and_index(int tag, int index);
#endif