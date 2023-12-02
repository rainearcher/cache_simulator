#include "utils.h"
#include "const.h"
#include "cacheBlock.h"
int block_offset(int addr)
{
	return addr % BLOCK_SIZE;
}

int block_address(int addr)
{
    return addr / BLOCK_SIZE;
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

bool victim_cache_block_is_target(const CacheBlock &block, int addr)
{
    return block.tag == victim_tag(addr) && block.valid;
}

int get_addr_from_l1_tag_and_index(int tag, int index)
{
    return (tag * L1_CACHE_SETS + index) * BLOCK_SIZE;
}