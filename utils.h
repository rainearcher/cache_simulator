#ifndef UTILS_H
#define UTILS_H
#include "const.h"

int block_offset(int addr)
{
	return addr % BLOCK_SIZE;
}

int block_address(int addr)
{
    return addr - block_offset(addr);
}

int victim_tag(int addr)
{
    return block_address(addr);
}

int l1_index(int addr)
{
    return (addr / BLOCK_SIZE) % L1_CACHE_SETS;
}

int l1_tag(int addr)
{
    return (addr / BLOCK_SIZE) / L1_CACHE_SETS;
}

int l1_tag_to_victim_tag(int l1Tag)
{
    return l1Tag * L1_CACHE_SETS;
}

#endif