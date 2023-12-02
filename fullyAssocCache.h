#ifndef VICTIMCACHE_H
#define VICTIMCACHE_H
#include "cacheBlock.h"
#include "const.h"

class FullAssocCache
{
public:
    FullAssocCache();
	bool is_full();
	void insert_block(CacheBlock block, int addr);
	bool addr_hit(int addr);
    CacheBlock evict_block(int addr);
    CacheBlock evict_lru_block();
	void overwrite_with_block(CacheBlock newBlock, int addr);
private:
	void insert_block_into_nonfull_victim_cache(CacheBlock block);
	void insert_block_into_full_victim_cache(CacheBlock block);
	void set_block_as_mru(CacheBlock* block);
    CacheBlock* get_block_with_addr(int addr);
private:
	CacheBlock blocks[VICTIM_SIZE];
};

#endif